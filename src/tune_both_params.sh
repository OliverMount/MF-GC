#!/bin/bash

# Script to tune both g_pas and synaptic weight to achieve 0.3-0.9 Hz with no outliers

CELL_FILE="../utils/Cell.py"
GRC_FILE="../develop/GrcSpontaneous.py"
ORIGINAL_G_PAS=0.003
ORIGINAL_WEIGHT=0.008
TARGET_MIN=0.3
TARGET_MAX=0.9
TRIALS_PER_COMBO=5

echo "Tuning both g_pas and synaptic weight for GRC firing rate"
echo "Target range: ${TARGET_MIN} - ${TARGET_MAX} Hz"
echo "Testing ${TRIALS_PER_COMBO} trials per parameter combination"
echo ""
echo "g_pas    weight   Mean_Rate  Min_Rate   Max_Rate   Outliers   In_Range"
echo "--------------------------------------------------------------------"

# Function to update g_pas in Cell.py
update_g_pas() {
    local new_value=$1
    sed -i "s/seg\.g_pas\s*=\s*[0-9.]\+/seg.g_pas=${new_value}/" "$CELL_FILE"
}

# Function to update synaptic weight in GrcSpontaneous.py
update_weight() {
    local new_value=$1
    sed -i "s/nc\.weight\[0\]\s*=\s*[0-9.]\+/nc.weight[0] = ${new_value}/" "$GRC_FILE"
}

# Function to restore original values
restore_original() {
    update_g_pas $ORIGINAL_G_PAS
    update_weight $ORIGINAL_WEIGHT
}

# Function to count outliers (rates > 5 Hz or < 0.1 Hz)
count_outliers() {
    local rates=("$@")
    local outlier_count=0
    for rate in "${rates[@]}"; do
        if (( $(echo "$rate > 5.0 || $rate < 0.1" | bc -l) )); then
            ((outlier_count++))
        fi
    done
    echo $outlier_count
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

# Test combinations of g_pas and weight
# Higher g_pas values to reduce excitability
G_PAS_VALUES=(0.01 0.015 0.02 0.025)
# Lower weight values to reduce synaptic strength
WEIGHT_VALUES=(0.003 0.004 0.005 0.006)

best_combo=""
best_score=999

for g_pas in "${G_PAS_VALUES[@]}"; do
    for weight in "${WEIGHT_VALUES[@]}"; do
        echo -n "Testing g_pas=$g_pas, weight=$weight ... "
        
        # Update both parameters
        update_g_pas $g_pas
        update_weight $weight
        
        # Run multiple trials
        rates=()
        for ((trial=1; trial<=TRIALS_PER_COMBO; trial++)); do
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
            
            # Count outliers and rates in range
            outlier_count=$(count_outliers "${rates[@]}")
            
            all_in_range=1
            for rate in "${rates[@]}"; do
                if [ $(in_range $rate) -eq 0 ]; then
                    all_in_range=0
                    break
                fi
            done
            
            # Calculate a "badness score" (outliers + out-of-range)
            out_of_range=$((${#rates[@]} - $(for r in "${rates[@]}"; do in_range $r; done | grep -c 1)))
            badness_score=$((outlier_count + out_of_range))
            
            # Print results
            printf "%-8s %-8s %-10s %-10s %-10s %-10s %-8s\n" \
                "$g_pas" \
                "$weight" \
                "$(printf "%.3f" $mean_rate)" \
                "$(printf "%.3f" $min_rate)" \
                "$(printf "%.3f" $max_rate)" \
                "$outlier_count/${#rates[@]}" \
                "$([ $all_in_range -eq 1 ] && echo "YES" || echo "NO")"
            
            # Track best combination (lowest badness score)
            if [ $badness_score -lt $best_score ]; then
                best_score=$badness_score
                best_combo="g_pas=$g_pas, weight=$weight"
            fi
            
            # If we found a perfect combination, highlight it
            if [ $all_in_range -eq 1 ] && [ $outlier_count -eq 0 ]; then
                echo "*** PERFECT COMBINATION: g_pas=$g_pas, weight=$weight ***"
            fi
            
        else
            echo "FAILED - no data"
        fi
    done
done

echo ""
echo "Tuning complete!"
echo "Best combination found: $best_combo (badness score: $best_score)"
echo ""
echo "Restoring original values..."
restore_original

echo ""
echo "To set the optimal parameters manually:"
echo "sed -i 's/seg\.g_pas=.*$/seg.g_pas=YOUR_G_PAS_VALUE/' $CELL_FILE"
echo "sed -i 's/nc\.weight\[0\] = .*$/nc.weight[0] = YOUR_WEIGHT_VALUE/' $GRC_FILE"
