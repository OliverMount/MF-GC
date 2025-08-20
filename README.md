# Mossy Fiber (MF) ->  Granule Cell (GRC) Feed-Forward Network

This repository provides a CoreNEURON-compatible computational model of the cerebellar feed-forward mossy fiber (MF) → granule cell (GRC) network. The model integrates structural connectivity data from the experimental electron microscopy (EM) study [EMdata](https://github.com/htem/cb2_project_analysis) [^1]. The network currently includes two major neuronal populations:

1. Mossy fibers (MF)
2. Granule cells (GRC)

### Connectivity

The connectivity extracted from [EMdata] includes

1. mf --> grc  (used in this model)
2. grc --> pc  (currently not simulated)

 
### How to run the code? 
 
Before running the simulations, please ensure the following:

#### Connectivity database

* The file mf_grc.db must be available in the `connectivity/` folder.

* This database contains the MF→GRC connectivity extracted from EMdata.

#### Scaling the network

* To adjust the network size, use the script `scripts/DatabaseScaling.py`.

* Set the scaling factor inside the script to determine the number of cells and synaptic connections. Example: A scaling factor of 280 yields approximately 1 million GRCs and 3.2 million synaptic connections.

* Note: Large databases (e.g., ~270 MB for the above case) cannot be stored directly on GitHub due to size limitations.

#### Apptainer Image and Run Scripts
* Ensure that the Apptainer image and required run files (Python scripts) are available at the locations specified in `src/run_levi*.sh`.

#### Compiling and Running

Move to `src/` folder and run the following files


```bash
# Compile mod files (edit paths inside if necessary)
./compile_mod_files 

# Run simulation with CoreNEURON on GPU
./run_levi_gpu.sh 

# Run simulation with CoreNEURON on CPU
./run_levi_cpu.sh 
```
 
### Notes for Beginners

#### NEURON vs. CoreNEURON:

* NEURON is the standard simulator for neuronal modeling.

* CoreNEURON is its optimized compute engine, designed for faster large-scale simulations (with CPU/GPU support).

#### Apptainer (formerly Singularity):

* This project uses Apptainer to ensure reproducibility and portability across systems. 
* You do not need to install CoreNEURON manually; it is already included in the Apptainer image.
 
 
  
[^1]: Nguyen, T. M., Thomas, L. A., Rhoades, J. L., Ricchi, I., Yuan, X. C., Sheridan, A., Hildebrand, D. G. C., Funke, J., Regehr, W. G., & Lee, W. A. (2023). Structured cerebellar connectivity supports resilient pattern separation. Nature, 613(7944), 543–549. https://doi.org/10.1038/s41586-022-05471-w
