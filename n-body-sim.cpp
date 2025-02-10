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

using namespace std;

// Global variables.
double grav = 0.00000000006674;
double soft_factor = 0.000000000001;

void calc_force(particle arr[]) 
{
    int arr_size = sizeof(arr) / sizeof(particle);

    // 
    for (int i=0; i < arr_size; i++) {

        for (int j=0; j < arr_size; j++) {
            // Prevent interactions to itself.
            if (i != j) {
                double loc_diff = std::abs(arr[i].position - arr[j].position);
                double force = (grav * arr[i].mass * arr[j].mass) / pow(loc_diff, 2);
                arr[i].force += force_direction(arr[i].position, arr[j].position, force);
            }
        }

    }

}

// Calculating the direction of the force.
// Parameters: locations of the two particles and the gross force they make.
double force_direction(double loc1, double loc2, double force)  
{
    double location_diff = std::abs(loc1 - loc2) + soft_factor;
    double direction = ( (loc1 - loc2) / location_diff) * force;
    return direction;
}

// AVP = acceleration, velocity, positions.
// ALL PARAMS: accel, direction, mass, new_vel, old_vel, time_step, new_pos, old_pos
void calc_new_AVP(double direction, double mass, double old_vel, double time_step_size, double old_pos) 
{
    double accel = direction / mass;
    double new_vel = old_vel + (accel * time_step_size);
    double new_pos = old_pos + (new_vel * time_step_size);
}

// Initialize the particle 
struct particle 
{
    double mass;
    double position;
    double velocity;
    double force;
};

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

int main (int argc, char* argv[]) 
{
    // Initialize the command line arguments.

    // int num_particles = std::atoi(argv[1]);
    // int time_step_size = std::atoi(argv[2]);
    // int iterations = std::atoi(argv[3]);
    // int dump_rate = std::atoi(argv[4]);

    // TEST CASE!
    int num_particles = 3;
    struct particle arr[num_particles];
    struct particle* ptr = (particle*) malloc(sizeof(particle) * num_particles);

    // Print out if malloc fails.
    if (ptr == NULL) {
        std::cout << "Malloc failed!  Exiting program.";
        return -1;
    }

    test_case(arr);

    // Reset all the particle's forces.
    for (int f=0; f < num_particles; f++) {
        arr[f].force = 0;
        std::cout << arr[f].force << " ";
    }

    // Check if output worked.
    // for (int a=0; a < 3; a++) {
    //  std::cout << arr[a].mass << " " << arr[a].position << " " << arr[a].velocity << " ";
    //}

    // Deallocating and terminating.
    free(ptr);
    return 0;
}