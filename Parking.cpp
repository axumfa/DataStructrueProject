#include <iostream>
#include "Array.h"
#include "Queue.h"
#include "Stack.h"

using namespace std;

// ============================================================================
// Car: Represents a parked car with ID
// ============================================================================
class Car {
public:
    int id;
    static int totalCars;

    Car() : id(0) {}

    Car(int carId) : id(carId) {}

    static Car createNewCar() {
        return Car(++totalCars);
    }

    // Overload << for easy printing
    friend ostream& operator<<(ostream& os, const Car& c) {
        os << c.id;
        return os;
    }
};

int Car::totalCars = 0;

// ============================================================================
// Parking: Manages multiple parking lanes using Array<Stack<Car>>
// ============================================================================
class Parking {
private:
    Array<Stack<Car>> parkingLanes; // Array of Stacks
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
        for (int i = 0; i < laneCount; ++i) {
            // Try to find car in each lane by iterating through the stack
            // Since Stack doesn't expose direct iteration, we'll use peek and pop temporarily
            Stack<Car> tempStack = parkingLanes[i];
            while (!tempStack.isEmpty()) {
                try {
                    Car c = tempStack.pop();
                    if (c.id == carId) {
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
        Queue<int> tempQueue(laneCapacity); // temporary queue for displaced car IDs

        int moves = 0;
        int countMoved = 0;
        int targetIndex = -1;

        // Pop cars until we find the target
        int* movedCarIds = new int[laneCapacity];
        int moveIdx = 0;

        try {
            while (!s.isEmpty()) {
                Car c = s.pop();
                movedCarIds[moveIdx++] = c.id;
                countMoved++;
                ++moves; // each pop is a move

                if (c.id == carId) {
                    targetIndex = moveIdx - 1;
                    break; // found and removed target
                }
            }
        } catch (...) {
            delete[] movedCarIds;
            return -1;
        }

        // Display cars moved
        cout << "\n--- Removal Details for Car " << carId << " ---\n";
        cout << "Cars moved temporarily (order moved out): ";
        if (countMoved == 0) {
            cout << "none\n";
        } else {
            for (int i = 0; i < countMoved; ++i) {
                cout << movedCarIds[i];
                if (i + 1 < countMoved) cout << ", ";
            }
            cout << "\n";

            // Re-push cars back in reverse order (excluding the target car)
            cout << "Re-parking cars: ";
            for (int i = targetIndex - 1; i >= 0; --i) {
                Car pushCar(movedCarIds[i]);
                try {
                    s.push(pushCar);
                    ++moves; // each push is a move
                    cout << movedCarIds[i];
                    if (i > 0) cout << ", ";
                } catch (...) {
                    cout << "\nError re-parking car " << movedCarIds[i] << "\n";
                }
            }
            cout << "\n";
        }

        cout << "Total movements (including removal): " << moves << "\n";
        delete[] movedCarIds;
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

// ============================================================================
// Main: Interactive Parking System
// ============================================================================
int main() {
    cout << "\n====== INHA University - Parking Lot Management System ======\n";
    cout << "Data Structures: Array<Stack<Car>> + Queue<int>\n";
    cout << "========================================================\n\n";

    int numLanes, capacity;
    cout << "Enter number of parking lanes: ";
    cin >> numLanes;
    if (numLanes <= 0) numLanes = 2;

    cout << "Enter capacity per lane: ";
    cin >> capacity;
    if (capacity <= 0) capacity = 5;

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

    return 0;
}