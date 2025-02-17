// Evan Stark - February 7th 2025 - ITCS 4145 001
// This program is the implementation of a basic N-Body Simulation problem.

// SOURCES USED
// https://www.geeksforgeeks.org/abs-labs-llabs-functions-cc/?ref=gcse_outind
// https://www.geeksforgeeks.org/structures-in-cpp/?ref=header_outind
// https://www.geeksforgeeks.org/cpp-malloc/?ref=header_outind
// https://www.geeksforgeeks.org/power-function-c-cpp/

// CHATGPT conversation about debugging a malloc issue:
// https://chatgpt.com/share/67b2cb1a-ffe0-8001-89fd-370cab75d53b


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
    double position[3];
    double velocity[3];
    double force[3];
};

void calc_force(particle p1, particle p2) 
{
    for (int i=0; i < 3; i++) {
        double loc_diff = std::abs(p1.position[i] - p2.position[i]) + soft_factor;
        double force = (grav * p1.mass * p2.mass) / pow(loc_diff, 2);
        //p1.force[i] = force;
        p1.force[i] = p1.force[i] + force;
    }
}

// Calculating the direction of the force.
void force_direction(particle p1, particle p2)  
{
    for (int i = 0; i < 3; i++) {
        double loc_diff = std::abs(p1.position[i] - p2.position[i]) + soft_factor;
        double direction = (p1.position[i] - p2.position[i]) / loc_diff * p1.force[i];
        p1.force[i] = p1.force[i] + direction;
    }
}

// AVP = acceleration, velocity, positions.
void calc_new_AVP(particle p, double time_step_size) 
{
    for (int i=0; i < 3; i++) {
        double accel = p.force[i] / p.mass;
        
        double velo_add = p.velocity[i] + (accel * time_step_size);
        p.force[i] =  p.force[i] + velo_add;

        double pos_add = p.position[i] + (p.velocity[i] * time_step_size);
        p.position[i] = p.position[i] + pos_add;
    }
}

// Generating x, y, and z data for velocity, position, and force.
particle generate_data ()
{
    struct particle data;
    srand(time(0));

    // Mass of the particle --> CANNOT BE NEGATIVE AND ONLY ONE VALUE NEEDED.
    data.mass = (double) (rand() % 100) / 100;

    for (int i=0; i < 3; i++) {
        // First gets x values, then y, and then finally z.
        data.position[i] = (double) (rand() % 100) / 100;
        data.velocity[i] = (double) (rand() % 100) / 100;
        data.force[i] = (double) (rand() % 100) / 100;

        // Random chance to generate negative values.
        int neg = rand() % 2;
        if (neg == 1) {
            data.position[i] = -data.position[i];
            data.velocity[i] = -data.velocity[i];
            data.force[i] = -data.force[i];
        }
    }
    
    return data;
}

// Predefined scenario for testing purposes.
void test_case(particle arr[]) 
{
    // Initializing the three particles;
    // Using generate_data to get initial values for position, velocity, force.
    struct particle p1 = generate_data();
    struct particle p2 = generate_data();
    struct particle p3 = generate_data();

    // Build the array.
    arr[0] = p1;
    arr[1] = p2;
    arr[2] = p3;
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
    for (int i=0; i < num_particles; i++) {
        arr[i] = generate_data();
    }

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
        // Resetting all forces to 0 for each dimension.
        for(int p=0; p < num_particles; p++) {
            for (int a=0; a < 3; a++) {
                arr[p].force[a] = 0;
            }
        }

        // Calculate forces between each combination of particles.
        for (int i=0; i < num_particles; i++) {

            for (int j=0; j < num_particles; j++) {
                
                // Prevent interactions to itself.
                if (i != j) {
                    calc_force(arr[i], arr[j]);
                    force_direction(arr[i], arr[j]);
                    calc_new_AVP(arr[i], time_step_size);
                }
            
            }
        
        }
        
        // For every [dump_rate] interval, print the output to the log file.
        if (iteration_count % dump_rate == 0) {
            // Display the number of particles in the system.
            std::cout << num_particles << "\t";
            
            // Particle attributes.
            for (int a=0; a < num_particles; a++) {
                std::cout << arr[a].mass 
                << "\t" << arr[a].position[0] << "\t" << arr[a].position[1] << "\t" << arr[a].position[2]
                << "\t" << arr[a].velocity[0] << "\t" << arr[a].velocity[1] << "\t" << arr[a].velocity[2] 
                << "\t" << arr[a].force[0] << "\t" << arr[a].force[1] << "\t" << arr[a].force[2] << "\t";
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