import argparse

parser = argparse.ArgumentParser() 

parser.add_argument("-runtype",
                    choices=["coreneuron_gpu", "coreneuron_cpu",  "neuron_serial", "neuron_parallel"],
                    default="coreneuron_gpu",
                    help="Simulation execution mode (options: coreneuron_gpu, coreneuron_cpu, neuron_serial, neuron_parallel)",
                    dest='runtype')
 
parser.add_argument("-tstop",
                    metavar='float',
                    help="stop time (ms) (default 100.0)",
                    type=float,
                    default=100)  

parser.add_argument("-trial", 
                   type=int, 
                   default=1,
                   metavar='T',
                   help='Trial number') 