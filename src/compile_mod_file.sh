export NMODL_PYLIB=/usr/lib/x86_64-linux-gnu/libpython3.12.so
export NMODLHOME=/home/oli/install/

rm -rf tmp/ x86_64/

#module load nvhpc-hpcx-cuda12/  cuda openmpi 

#export OMPI_CXX=nvc++
#export OMPI_CC=nvc
#export CC=mpicc
#export CXX=mpicxx

/home/oli/install/bin/nrnivmodl -coreneuron mod
#/home/olive/install/bin/nrnivmodl  mod
