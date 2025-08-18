#!/bin/bash

# Script to tune g_pas parameter to achieve 0.3-0.9 Hz firing rate

CELL_FILE="../utils/Cell.py"
ORIGINAL_G_PAS=0.003
TARGET_MIN=0.3
TARGET_MAX=0.9
TRIALS_PER_VALUE=10

# Array of g_pas values to test (start higher since current rates are too high)
G_PAS_VALUES=(0.0018 0.00181 0.00185)

echo "Tuning g_pas parameter for GRC firing rate"
echo "Target range: ${TARGET_MIN} - ${TARGET_MAX} Hz"
echo "Testing ${TRIALS_PER_VALUE} trials per g_pas value"
echo ""
echo "g_pas    Mean_Rate  Min_Rate   Max_Rate   In_Range   All_Rates"
echo "----------------------------------------------------------------"

# Function to update g_pas in Cell.py
update_g_pas() {
    local new_value=$1
    sed -i "s/seg\.g_pas\s*=\s*[0-9.]\+/seg.g_pas=${new_value}/" "$CELL_FILE"
}

# Function to restore original g_pas
restore_g_pas() {
    update_g_pas $ORIGINAL_G_PAS
}

# Function to check if rate is in target range
in_range() {
    local rate=$1
    if (( $(echo "$rate >= $TARGET_MIN && $rate <= $TARGET_MAX" | bc -l) )); then
        echo 1
    else
        echo 0
    fi
}

# Test each g_pas value
for g_pas in "${G_PAS_VALUES[@]}"; do
    echo -e "Testing g_pas = $g_pas ... "
    
    # Update g_pas value
    update_g_pas $g_pas
    
    # Run multiple trials
    rates=()
    for ((trial=1; trial<=TRIALS_PER_VALUE; trial++)); do
        # Run simulation and extract rate
        output=$(mpiexec -n 1 python ../develop/GrcSpontaneous.py 2>/dev/null)
        
        # Extract the rate (last number in the output)
        if [[ $output =~ ([0-9]+\.[0-9]+)$ ]]; then
            rate=${BASH_REMATCH[1]}
            rates+=($rate)
        fi
    done
    
    # Calculate statistics if we have data
    if [ ${#rates[@]} -gt 0 ]; then
        # Calculate mean, min, max
        sum=0
        min_rate=${rates[0]}
        max_rate=${rates[0]}
        
        for rate in "${rates[@]}"; do
            sum=$(echo "$sum + $rate" | bc -l)
            if (( $(echo "$rate < $min_rate" | bc -l) )); then
                min_rate=$rate
            fi
            if (( $(echo "$rate > $max_rate" | bc -l) )); then
                max_rate=$rate
            fi
        done
        
        mean_rate=$(echo "scale=3; $sum / ${#rates[@]}" | bc -l)
        
        # Check if all rates are in target range
        all_in_range=1
        for rate in "${rates[@]}"; do
            if [ $(in_range $rate) -eq 0 ]; then
                all_in_range=0
                break
            fi
        done
        
        # Format rates for display
        rates_str=""
        for rate in "${rates[@]}"; do
            rates_str="$rates_str $rate"
        done
        
        # Print results
        printf "%-8s %-10s %-10s %-10s %-10s %s\n" \
            "$g_pas" \
            "$(printf "%.3f" $mean_rate)" \
            "$(printf "%.3f" $min_rate)" \
            "$(printf "%.3f" $max_rate)" \
            "$([ $all_in_range -eq 1 ] && echo "YES" || echo "NO")" \
            "$rates_str"
        
        # If we found a good value, highlight it
        if [ $all_in_range -eq 1 ]; then
            echo "*** OPTIMAL g_pas found: $g_pas ***"
        fi
        
    else
        echo "FAILED - no data"
    fi
done

echo ""
echo "Tuning complete!"
echo ""
echo "Restoring original g_pas value ($ORIGINAL_G_PAS)..."
restore_g_pas

echo ""
echo "To set the optimal g_pas value manually, use:"
echo "sed -i 's/seg\.g_pas=.*$/seg.g_pas=YOUR_VALUE/' $CELL_FILE"
