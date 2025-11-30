#include <iostream>
#include "Array.h"
#include "Queue.h"
#include "Stack.h"
#include "Car.h"

using namespace std;


// Parking class which will contain all logic and parkinglanes 
class Parking {
private:
    // array of stack consisting car object
    Array<Stack<Car>> parkingLanes;
    int laneCount;
    int laneCapacity;

public:
    Parking(int numLanes, int capacity)
        : parkingLanes(numLanes), laneCount(numLanes), laneCapacity(capacity)
    {
        // Initialize each lane with a stack of given capacity
        for (int i = 0; i < numLanes; ++i) {
            parkingLanes[i] = Stack<Car>(capacity);
        }
    }

    // Park a car in the first available lane
    bool parkCar(int carId, int& laneUsed) {
        for (int i = 0; i < laneCount; ++i) {
            if (!parkingLanes[i].isFull()) {
                Car c(carId);
                parkingLanes[i].push(c);
                laneUsed = i;
                return true;
            }
        }
        return false;
    }

    // Find which lane contains a specific car ID
    int findCarLane(int carId) const {
        // it will do linear search so it will be O(N)
        for (int i = 0; i < laneCount; ++i) {
            // Try to find car in each lane by iterating through the stack
            // Since Stack doesn't expose direct iteration, we'll use peek and pop temporarily
            Stack<Car> tempStack = parkingLanes[i];
            while (!tempStack.isEmpty()) {
                try {
                    Car c = tempStack.pop();
                    if (c.getId() == carId) {
                        return i;
                    }
                } catch (...) {
                    break;
                }
            }
        }
        return -1;
    }

    // Remove a specific car and count total movements
    int removeCar(int carId) {
        int lane = findCarLane(carId);
        if (lane == -1) return -1; // not found

        Stack<Car>& s = parkingLanes[lane];
        Queue<Car> tempQueue(laneCapacity); // temporary queue for displaced cars

        int moves = 0;
        bool found = false;

        cout << "\n--- Removal Process for Car " << carId << " from Lane " << (lane + 1) << " ---\n";
        cout << "Cars moved temporarily (in order): ";

        try {
            // Popping cars until we find the correct one
            while (!s.isEmpty()) 
            {
                Car c = s.pop();
                ++moves; // each pop is a move

                if (c.getId() == carId) 
                {
                    found = true;
                    cout << "\n✓ Car " << carId << " found and removed!\n";
                    break; // found and removed target
                }
                tempQueue.enqueue(c);
                cout << c.getId() << " ";
            }

            if(!found)
            {
                cout << "\n✗ Car not found in this lane.\n";
                return -1;
            }

            // Restoring cars from queue back to stack
            cout << "Re-parking cars back (from queue): ";
            while(!tempQueue.isEmpty())
            {
                Car c = tempQueue.dequeue();
                s.push(c);
                ++moves;
                cout << c.getId() << " ";
            }
            cout << "\n";

        } catch (...) {
            cout << "\n✗ Error during car removal process.\n"; 
            return -1;
        }
        
        cout << "Total movements (pops + pushes): " << moves << "\n";
        cout << "-------------------------------------------\n";
        return moves;
    }

    // Display all parking lanes
    void displayAllLanes() const {
        cout << "\n===== Current Parking Status =====\n";
        for (int i = 0; i < laneCount; ++i) {
            cout << "Lane " << (i + 1) << ": ";
            cout << parkingLanes[i];
            cout << " (Size: " << parkingLanes[i].getSize() << ", Capacity: " << laneCapacity << ")\n";
        }
        cout << "=================================\n";
    }
};

void menu()
{
    cout << "\n====== INHA University - Parking Lot Management System ======\n";
    cout << "========================================================\n\n";

    int numLanes, capacity;
    cout << "Enter number of parking lanes: ";
    cin >> numLanes;
    if (numLanes <= 0) numLanes = 2;

    cout << "Enter capacity per lane: ";
    cin >> capacity;
    if (capacity <= 0) capacity = 5;

    // making instance object with numlanes and capacity of lanes
    Parking lot(numLanes, capacity);

    int choice;
    while (true) {
        cout << "\n====== MENU ======\n";
        cout << "1) Park a Car\n";
        cout << "2) Remove a Specific Car\n";
        cout << "3) Display All Lanes\n";
        cout << "4) Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int carId;
            cout << "Enter car ID to park: ";
            cin >> carId;
            int laneUsed;
            if (lot.parkCar(carId, laneUsed)) {
                cout << "✓ Car " << carId << " successfully parked in Lane " << (laneUsed + 1) << ".\n";
            } else {
                cout << "✗ ERROR: All lanes are full. Cannot park car " << carId << ".\n";
            }
        } 
        else if (choice == 2) {
            int carId;
            cout << "Enter car ID to remove: ";
            cin >> carId;
            int moves = lot.removeCar(carId);
            if (moves == -1) {
                cout << "✗ ERROR: Car " << carId << " not found in any lane.\n";
            }
        } 
        else if (choice == 3) {
            lot.displayAllLanes();
        } 
        else if (choice == 4) {
            cout << "\nThank you for using the Parking System. Goodbye!\n";
            break;
        } 
        else {
            cout << "✗ Invalid option. Please try again.\n";
        }
    }
}

// main part 
int main() {
    menu();
}