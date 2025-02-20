**N-Body Simulation assignment for a parallel programming class.**

NOTE: This project is unfinished due to incorrect implementation during the first attempt, and my overall lack of any general knowledge.

PROBLEMS
Values seem to not update even after operations performed on them in n-body-sim.cpp

Output keeps getting overwritten in solar.tsv.  Due to this, only one state is ever on solar.tsv and is plotted on solar.pdf.


How to run:
1. On your terminal, type in the following line:

   g++ -o output_file n-body-sim.cpp

3.  To run the program and pass in your own parameters, type in the following line:

    ./output_file <num_particles> <timestep_size> <iterations> <dump_rate>

    The file will automatically print out how fast it took to run that case in milliseconds.

4. Print out the following line to see the dumped states of the particles:

   cat solar.tsv

Printing the chart:
1. Type in the following to pass the .tsv file through the plotting program and have it converted to a .pdf file.

   python plot.py solar.tsv solar.pdf 10000

3. To view the pdf, simply locate the folder this repository is saved to and open the pdf from there.


Benchmarking on Centaurus:
1. On your terminal, type in the following command

   ssh uncc_id@hpc-student.uncc.edu

3. From there, type in your UNC-Charlotte credentials and authenicate.

4. When logged onto Centaurus, clone the repository and add a SSH key to automatically authenticate yourself.

5. Run the following command to queue the job.

   sbatch n-body-script.sh

   Note --> you may need to do chmod +x on n-body-script.sh to grant yourself execution permissions.

6. Once your code is finished on the partition, you can print the output via the following:

   ./slurm-<job_num>.out
