#!/bin/bash
#SBATCH --job-name=PI_MPI       #Job name
#SBATCH --output=PI_MPI_%j.log	#log file name
#SBATCH --partition=compute	#use computing cluster
#SBATCH --mem=1gb		#job mem request
#SBATCH --nodes=8		#number of compute nodes
#SBATCH --time=00:02:00		#time limit HH:MM:SS
. /etc/profile.d/modules.sh
module load openmpi/2.1.2
/opt/openmpi-2.1.2/bin/mpirun ./pi_mpi
