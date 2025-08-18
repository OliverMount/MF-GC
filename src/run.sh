#echo -e "Compiling the mod files"
#source compile_mod_file.sh

mpicore=1

mpiexec -n ${mpicore} python ../develop/mf_grc_network.py -tstop 10000
