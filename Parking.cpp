#include <iostream>
#include<iomanip>
#include<climits>
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
    double totalRevenue;        // Track total money collected
    int totalCarsServed;        // Track total cars that exited

public:
    Parking(int numLanes, int capacity)
        : parkingLanes(numLanes), laneCount(numLanes), laneCapacity(capacity), 
          totalRevenue(0.0), totalCarsServed(0)
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
                c.markEntryTime();
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
            
            // Try to find car in each lane by poping through the stack
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

    // Check whether an ID is currently unused in the parking lot
    bool isCarIdAvailable(int carId) const {
        return findCarLane(carId) == -1;
    }

    // Remove a specific car and count total movements
    double removeCar(int carId) {
        int lane = findCarLane(carId);
        if (lane == -1) return -1.0; // not found

        Stack<Car>& laneStack = parkingLanes[lane];
        Queue<Car> tempQueue(laneCapacity); // temporary queue for displaced cars
        Stack<Car> tempStack(laneCapacity); // it is for tracking orders

        int moves = 0;
        bool found = false;
        Car removedCar(0);  // store the removed car for fee calculation

        cout << "\n--- Removal Process for Car " << carId << " from Lane " << (lane + 1) << " ---\n";

        try {
            // Popping cars until we find the correct one
            while (!laneStack.isEmpty()) 
            {
                Car c = laneStack.pop();
                ++moves; // each pop is a move

                if (c.getId() == carId) 
                {
                    found = true;
                    removedCar = c;  // save for fee calculation
                    cout << "\n Car " << carId << " found and removed!\n";
                    break; // found and removed target
                }
                
                tempStack.push(c);
                cout << c.getId() << " car pushed to temp stack\n";
        
            }

            if(!found)
            {
                cout << "\n Car not found in this lane.\n";
                return -1.0;
            }

            // Restoring cars from stack to queue 
            cout << "Storing cars into temp queue\n";
            while(!tempStack.isEmpty())
            {
                Car c = tempStack.pop();
                tempQueue.enqueue(c);
                ++moves;
                cout << c.getId() << " car moved into the temp Queue\n";
            }
            // it will help for order 
            while(!tempQueue.isEmpty())
            {
                Car c = tempQueue.dequeue();
                laneStack.push(c);
                ++moves;
                cout << c.getId() << " car moved into parking lane\n";

            }
            cout << "\n";

        } catch (...) {
            cout << "\n Error during car removal process.\n"; 
            return -1.0;
        }
        
        // Calculate and collect payment
        double fee = removedCar.computeFee();
        totalRevenue += fee;
        totalCarsServed++;
        
        cout << "Total movements (pops + pushes): " << moves << "\n";
        cout << "-------------------------------------------\n";
        cout << "Fee Collected: " << removedCar.getFormattedFee() << "\n";
        cout << "-------------------------------------------\n";
        return fee;
    }

    // Display all parking lanes
    void displayAllLanes() const {
        cout << "\n===== Current Parking Status =====\n";
        for (int i = 0; i < laneCount; ++i) {
            cout << "Lane " << (i + 1) << ": ";
            cout << parkingLanes[i];
            int occupation = getLaneOccupancy(i);
            cout << " (Occupied: " << occupation << ", Capacity: " << laneCapacity << ")\n";
        }
        cout << "=================================\n";
    }

    // displaying car details with search without nowing exact place and having instance object
    void displayCarDetails(int carId) const
    {
        int lane = findCarLane(carId);
        if(lane == -1)
        {
            cout << "Car " << carId << " not found\n";
            return;
        }

        Stack<Car> tempStack = parkingLanes[lane];
        while(!tempStack.isEmpty())
        {
            Car c = tempStack.pop();
            if(c.getId() == carId)
            {
                cout << "\n === Car Details ===\n";
                c.displayInfo();
                cout << "Lane: " << (lane + 1) << "\n";
                cout << "===========\n";
                return;
            }    
        }
    }

        // One Lane Occupancy
    int getLaneOccupancy(int laneIndex) const
    {
        if(laneIndex < 0 || laneIndex >= laneCount) { return -1; }
        
        Stack<Car> tempStack = parkingLanes[laneIndex];
        int cnt = 0;
        while(!tempStack.isEmpty())
        {
            tempStack.pop();
            ++cnt;
        }

        return cnt;
    }
    
    
    // getting total parked cars at one point
    int getTotalParkedCars() const
    {
        int cnt = 0;
        for(int i = 0; i < laneCount; ++i)
        {
            cnt += getLaneOccupancy(i);
        }
        return cnt;
    }

    int getLeastOccupiedLane() const{
        int minOccupancy = INT_MAX;
        int leastLane = 0;

        for(int i = 0; i < laneCount; ++i)
        {
            Stack<Car> tempStack = parkingLanes[i];
            int cnt = 0;

            while(!tempStack.isEmpty())
            {
                tempStack.pop();
                ++cnt;
            }

            if (cnt < minOccupancy)
            {
                minOccupancy = cnt;
                leastLane = i;
            }
        }
        return leastLane;
    }

    // Display financial report
    void displayFinancialReport() const
    {
        cout << "\n===== Financial Report =====\n";
        cout << "Total Cars Served: " << totalCarsServed << "\n";
        cout << "Total Revenue Collected: " << fixed << setprecision(2) << totalRevenue << " Sum\n";
        
        if (totalCarsServed > 0) {
            double avgFee = totalRevenue / totalCarsServed;
            cout << "Average Fee per Car: " << fixed << setprecision(2) << avgFee << " Sum\n";
        } else {
            cout << "Average Fee per Car: 0.00 Sum\n";
        }
        
        cout << "=============================\n";
    }

    void displayStatistics() const 
    {
        cout << "\n===== Parking Statistics =====\n";
        int totalCars = getTotalParkedCars();
        int capacity = laneCount * laneCapacity;
        double occupancy = (totalCars * 100.0) / capacity;
        
        cout << "Total Parked Cars: " << totalCars << " / " << capacity << "\n";
        cout << "Occupancy Rate: " << fixed << setprecision(2) << occupancy << "%\n";
        cout << "Active Lanes: " << laneCount << "\n";
        cout << "Capacity per Lane: " << laneCapacity << "\n";
        
        for (int i = 0; i < laneCount; ++i) 
        {
            double percent = (getLaneOccupancy(i) * 100.0) / laneCapacity;
            cout << "  Lane " << (i + 1) << ": "<< fixed << setprecision(2) << percent << "% full\n";
        }
        cout << "==============================\n";
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
        cout << "1) Park a Car\n";
        cout << "2) Remove a Specific Car (Collect Fee)\n";
        cout << "3) Display All Lanes\n";
        cout << "4) Display Statistics\n";
        cout << "5) View Financial Report\n";
        cout << "6) Check Car ID Availability\n";
        cout << "7) Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) 
        {
            int carId;
            cout << "Enter car ID to park: ";
            cin >> carId;

            if (!lot.isCarIdAvailable(carId)) {
                cout << "ERROR: Car " << carId << " is already parked in the lot.\n";
                continue;
            }

            int laneUsed;
            if (lot.parkCar(carId, laneUsed)) {
                cout << "Car " << carId << " successfully parked in Lane " << (laneUsed + 1) << ".\n";
            } else {
                cout << "ERROR: All lanes are full. Cannot park car " << carId << ".\n";
            }
        } 

        else if (choice == 2) 
        {
            int carId;
            cout << "Enter car ID to remove: ";
            cin >> carId;
            double fee = lot.removeCar(carId);
            if (fee < 0) {
                cout << "ERROR: Car " << carId << " not found in any lane.\n";
            }
        } 
        else if (choice == 3) { lot.displayAllLanes(); } 
        else if (choice == 4) { lot.displayStatistics(); }
        else if (choice == 5) { lot.displayFinancialReport(); }
        
        else if (choice == 6)
        {
            int carId;
            cout << "Enter car ID to check: ";
            cin >> carId;
            if (lot.isCarIdAvailable(carId)) {
                cout << "Car ID " << carId << " is available.\n";
            } else {
                cout << "Car ID " << carId << " is already in use.\n";
            }
        }
        
        else if (choice == 7) 
        { 
            cout << "Exiting Program\n";
            break; 
        }
    }
}

// main part 
int main() {
    menu();
}