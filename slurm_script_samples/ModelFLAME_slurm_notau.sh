#!/bin/bash
#SBATCH --partition=nodo.q
#SBATCH --nodes=4
#SBATCH --ntasks=48                  # Number of MPI ranks
#SBATCH --exclusive
#SBATCH --time=48:00:00
#SBATCH --output=output/outputslurm.%N.%j.out
#SBATCH --error=output/outputslurm.%N.%j.err
#SBATCH --mail-type=END
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=amoreno@euss.cat
 
pwd; /usr/bin/hostname; /usr/bin/date
 
echo "Running Demo_03_Step_03 of Repast on  $SLURM_JOB_NUM_NODES nodes with $SLURM_NTASKS tasks, each with $SLURM_CPUS_PER_TASK cores."


# Enable modules
#source /opt/Modules/3.2.9/init/Modules4bash.sh
 
# Load modules
module load openmpi/3.0.0
# module load gcc/4.9.1
# module load mpich/3.2
#module load openmpi/1.8.8_gcc-4.9.1
 
# Program to execute
#for a in 1 2 3 4 5 6 7 8 9 10; 
for a in 1 ; 
do mpiexec -n 48 ./main 100 0.xml -r > ./output/sortida$a.txt ; 
done
/usr/bin/date

