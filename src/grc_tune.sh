rm grc_spike_rates.txt

mpicore=1
 
outfile="grc_spike_rates.txt" > $outfile  # Empty the file

repetitions=10    # Define repetitions here as a variable
 
echo -e "Running $repetitions GRC simulations..."

for ((i=1; i<=repetitions; i++)); do
    echo -e "Repetitions $i:  of $repetitions"
    rate=$(mpiexec -n ${mpicore} python ../develop/GrcSpontaneous.py )
    echo $rate >> $outfile
done

echo "All rates saved to $outfile"

#mpicore=1
#mpiexec -n ${mpicore} python ../develop/GrcSpontaneous.py

 
