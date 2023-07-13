#include <iostream>
#include <thread>

using namespace std;

// Function to be executed in the first thread
void programOne() {
    // Run program one here...
    std::cout << "Program One" << std::endl;
    cout<<1;
}

// Function to be executed in the second thread
void programTwo() {
    // Run program two here...
    std::cout << "Program Two" << std::endl;
    cout<<3;
}

int main() {
    // Create threads for each program
    std::thread threadOne(programOne);
    std::thread threadTwo(programTwo);

    // Wait for both threads to finish
    threadOne.join();
    threadTwo.join();

    return 0;
}
