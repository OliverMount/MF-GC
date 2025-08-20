# Mossy Fiber (MF) ->  Granule Cell (GRC) Feed-Forward Network

This repository provides a CoreNEURON compatible computational model of the cerebellar mossyfiber (MF), integrating structural connectivity data in particular from the experimental study [EMdata](https://github.com/htem/cb2_project_analysis) [^1]. The model incorporates two major cell types, namely, 1. Mossy fibers (mf) 2. Granule cells (grc) and their experimentally identified synaptic connections.
 

The following (binary) connectivity is extracted from [EMdata]

1. mf --> grc
2. grc --> pc  (not used in the current simulation though)

 
### How to run the code? 
 
Before runnning, please check

1. mf_grc.db is available inside the connectivity folder. mf_grc.db is the connectivity data extracted from [EMdata](https://github.com/htem/cb2_project_analysis). 


2. Apptainer image and other necessary files (python run file) specified are available in the location specified in the src/run_levi*.sh 


3. Move to src/ folder and run

./compile_mod_files   #
./run_levi_gpu.sh     # For apptainer based network run on CoreNEURON-GPU
./run_levi_cpu.sh     # For apptainer based network run on CoreNEURON-CPU
 
 
  
[^1]: Nguyen, T. M., Thomas, L. A., Rhoades, J. L., Ricchi, I., Yuan, X. C., Sheridan, A., Hildebrand, D. G. C., Funke, J., Regehr, W. G., & Lee, W. A. (2023). Structured cerebellar connectivity supports resilient pattern separation. Nature, 613(7944), 543–549. https://doi.org/10.1038/s41586-022-05471-w
