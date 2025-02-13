// Evan Stark - February 7th 2025 - ITCS 4145 001
// This program is the implementation of a basic N-Body Simulation problem.

// SOURCES USED
// https://www.geeksforgeeks.org/abs-labs-llabs-functions-cc/?ref=gcse_outind
// https://www.geeksforgeeks.org/structures-in-cpp/?ref=header_outind
// https://www.geeksforgeeks.org/cpp-malloc/?ref=header_outind
// https://www.geeksforgeeks.org/power-function-c-cpp/

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <random>
#include <chrono>

using namespace std;

// Global variables.
double grav = 0.00000000006674;
double soft_factor = 0.000000000001;

// Initialize particle parameters.
struct particle 
{
    double mass;
    double position;
    double velocity;
    double force;
};

double calc_force(particle p1, particle p2) 
{
    double loc_diff = std::abs(p1.position - p2.position);
    double force = (grav * p1.mass * p2.mass) / pow(loc_diff, 2);
    return force;
}

// Calculating the direction of the force.
double force_direction(particle p1, particle p2, double force)  
{
    double location_diff = std::abs(p1.position - p2.position) + soft_factor;
    double direction = ( (p1.position - p2.position) / location_diff) * (force);
    return direction;
}

// AVP = acceleration, velocity, positions.
double* calc_new_AVP(particle p, double time_step_size) 
{
    double* output_array = (double*) malloc(2 * sizeof(double) );
    double accel = p.force / p.mass;
    // New velocity value.
    output_array[0] = p.velocity + (accel * time_step_size);
    // New position value.
    output_array[1] = p.position + (p.velocity * time_step_size);
    return output_array;
}

// Predefined scenario for testing purposes.
void test_case(particle arr[]) 
{
    struct particle p1 {10.0, 5.5, 2.0, 0.0};
    struct particle p2 = {4.75, -2.5, -1.25, 0.0};
    struct particle p3 = {4.75, 1.75, -3.0, 0.0};

    // Build the array.
    arr[0] = p1;
    arr[1] = p2;
    arr[2] = p3;
}

particle* generate_data (particle arr[], int num_particles) 
{
    // Ensure random number generation.
    srand(time(0));
    
    for(int i=0; i < num_particles; i++) {
        arr[i].mass = (double) (rand() % 100) / 100;
        arr[i].position = (double) (rand() % 100) / 100;
        arr[i].velocity = (double) (rand() % 100) / 100;

        // Random chance to produce a negative number.
        int neg = rand();
        if (neg % 2 == 1){
            arr[i].position = -arr[i].position;
            arr[i].velocity = -arr[i].velocity;
        }

    }

    return arr;
}

int main (int argc, char* argv[]) 
{
    // Initialize the command line arguments.
    int num_particles = std::atoi(argv[1]);
    int time_step_size = std::atoi(argv[2]);
    int iterations = std::atoi(argv[3]);
    int dump_rate = std::atoi(argv[4]);

    struct particle* arr = (particle*) malloc(num_particles * sizeof(particle));

    // Ensure malloc succeeds.
    if (arr == NULL) {
        std::cout << "Malloc failed for arr!";
        return -1;
    }

    // Populate array.
    arr = generate_data(arr, num_particles);

    // Run the program for x number of iterations.
    int iteration_count = 0;
    
    // Libraries to use for timing the algorithm's efficiency.
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    // Get the time before the simulation runs.
    auto start = high_resolution_clock::now();
    while(iteration_count <= iterations) 
    {
        // Reset all the particle's forces.
        for (int f=0; f < num_particles; f++) {
            arr[f].force = 0;
        }

        // Calculate forces between each combination of particles.
        for (int i=0; i < num_particles; i++) {

            for (int j=0; j < num_particles; j++) {
                // Prevent interactions to itself.
                if (i != j) {
                    double temp_force = calc_force(arr[i], arr[j]);
                    arr[i].force += force_direction(arr[i], arr[j], temp_force);
                }
            }

            // After all forces added up, update acceleration, velocity, position.
            double* new_values =(double*) malloc(2 * sizeof(double) );

            if(new_values == NULL) {
                std::cout << "Malloc failed for new_values!";
                return -1;
            }

            new_values = calc_new_AVP(arr[i], time_step_size);
            arr[i].velocity += new_values[0];
            arr[i].position += new_values[1];
            free(new_values);
        }
        
        // For every [dump_rate] interval, print the output to the log file.
        if (iteration_count % dump_rate == 0) {
            // Display the number of particles in the system.
            std::cout << num_particles << "\t";
            for (int a=0; a < num_particles; a++) {
                std::cout << arr[a].mass << "\t" << arr[a].position << "\t" << arr[a].velocity << "\t" << arr[a].force << "\t";
            }
            
            // Buffer line for formatting.
            std::cout << "\n";
        }
        
        iteration_count++;
    }

    // Get the time for when the simulation ends.
    auto end = high_resolution_clock::now();

    // Calculate the time taken in milliseconds.
    duration <double, std::milli> time_elapsed = (end - start);

    using namespace std;
    std::cout << "Time taken: " << time_elapsed.count() << " ms";

    // Deallocating and terminating.
    free(arr);
    return 0;
}