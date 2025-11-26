#include <iostream>
using namespace std;

// ============== STACK IMPLEMENTATION ==============
template<typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int topIndex;

public:
    Stack() : arr(nullptr), capacity(0), topIndex(-1) {}
    
    Stack(int cap) : capacity(cap), topIndex(-1) {
        arr = new T[capacity];
    }
    
    Stack(const Stack& other) : capacity(other.capacity), topIndex(other.topIndex) {
        arr = new T[capacity];
        for (int i = 0; i <= topIndex; i++) {
            arr[i] = other.arr[i];
        }
    }
    
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            topIndex = other.topIndex;
            arr = new T[capacity];
            for (int i = 0; i <= topIndex; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    
    ~Stack() {
        delete[] arr;
    }
    
    bool push(T value) {
        if (isFull()) {
            return false;
        }
        arr[++topIndex] = value;
        return true;
    }
    
    bool pop(T& result) {
        if (isEmpty()) {
            return false;
        }
        result = arr[topIndex--];
        return true;
    }
    
    bool peek(T& result) const {
        if (isEmpty()) {
            return false;
        }
        result = arr[topIndex];
        return true;
    }
    
    bool isEmpty() const {
        return topIndex == -1;
    }
    
    bool isFull() const {
        return topIndex == capacity - 1;
    }
    
    int size() const {
        return topIndex + 1;
    }
    
    int getCapacity() const {
        return capacity;
    }
};

// ============== QUEUE IMPLEMENTATION ==============
template<typename T>
class Queue {
private:
    T* arr;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;

public:
    Queue() : arr(nullptr), capacity(0), frontIndex(0), rearIndex(-1), count(0) {}
    
    Queue(int cap) : capacity(cap), frontIndex(0), rearIndex(-1), count(0) {
        arr = new T[capacity];
    }
    
    Queue(const Queue& other) : capacity(other.capacity), frontIndex(other.frontIndex), 
                                 rearIndex(other.rearIndex), count(other.count) {
        arr = new T[capacity];
        for (int i = 0; i < capacity; i++) {
            arr[i] = other.arr[i];
        }
    }
    
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            frontIndex = other.frontIndex;
            rearIndex = other.rearIndex;
            count = other.count;
            arr = new T[capacity];
            for (int i = 0; i < capacity; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    
    ~Queue() {
        delete[] arr;
    }
    
    bool enqueue(T value) {
        if (isFull()) {
            return false;
        }
        rearIndex = (rearIndex + 1) % capacity;
        arr[rearIndex] = value;
        count++;
        return true;
    }
    
    bool dequeue(T& result) {
        if (isEmpty()) {
            return false;
        }
        result = arr[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return true;
    }
    
    bool isEmpty() const {
        return count == 0;
    }
    
    bool isFull() const {
        return count == capacity;
    }
    
    int size() const {
        return count;
    }
};

// ============== CAR STRUCTURE ==============
struct Car {
    int carID;
    int entryHour;
    
    Car() : carID(0), entryHour(0) {}
    Car(int id, int hour) : carID(id), entryHour(hour) {}
};

// ============== PARKING LOT SYSTEM ==============
class ParkingLot {
private:
    Stack<Car>* lanes;
    int laneCount;
    int laneCapacity;
    int pricePerHour;

public:
    ParkingLot(int numLanes, int capacity, int price = 20) 
        : laneCount(numLanes), laneCapacity(capacity), pricePerHour(price) {
        lanes = new Stack<Car>[laneCount];
        for (int i = 0; i < laneCount; i++) {
            lanes[i] = Stack<Car>(laneCapacity);
        }
    }
    
    ~ParkingLot() {
        delete[] lanes;
    }
    
    // Park a car
    bool parkCar(int carID, int entryHour) {
        for (int i = 0; i < laneCount; i++) {
            if (!lanes[i].isFull()) {
                Car car(carID, entryHour);
                lanes[i].push(car);
                cout << "\n✓ Car " << carID << " parked in Lane " << (i + 1) 
                     << " at hour " << entryHour << endl;
                return true;
            }
        }
        cout << "\n✗ Parking Full! All lanes are occupied." << endl;
        return false;
    }
    
    // Remove a specific car
    bool removeCar(int carID, int exitHour) {
        for (int i = 0; i < laneCount; i++) {
            if (lanes[i].isEmpty()) continue;
            
            Queue<Car> holdingLane(laneCapacity);
            int moves = 0;
            int movedCount = 0;
            bool found = false;
            Car targetCar;
            
            cout << "\n--- Searching in Lane " << (i + 1) << " ---" << endl;
            
            // Pop cars until we find the target
            while (!lanes[i].isEmpty()) {
                Car currentCar;
                lanes[i].pop(currentCar);
                moves++;
                
                if (currentCar.carID == carID) {
                    found = true;
                    targetCar = currentCar;
                    cout << "→ Car " << currentCar.carID << " (TARGET) removed from lane" << endl;
                    break;
                } else {
                    holdingLane.enqueue(currentCar);
                    movedCount++;
                    cout << "→ Car " << currentCar.carID << " moved to holding lane" << endl;
                }
            }
            
            // Restore cars from holding lane back to parking lane
            if (movedCount > 0) {
                cout << "\n--- Restoring cars back to Lane " << (i + 1) << " ---" << endl;
            }
            
            Stack<Car> tempStack(laneCapacity);
            while (!holdingLane.isEmpty()) {
                Car car;
                holdingLane.dequeue(car);
                tempStack.push(car);
            }
            
            while (!tempStack.isEmpty()) {
                Car car;
                tempStack.pop(car);
                lanes[i].push(car);
                moves++;
                if (found) {
                    cout << "→ Car " << car.carID << " moved back to lane" << endl;
                }
            }
            
            if (!found) {
                continue; // Search next lane
            }
            
            // Calculate parking fee
            int hoursParked = exitHour - targetCar.entryHour;
            int fee = hoursParked * pricePerHour;
            
            cout << "\n========================================" << endl;
            cout << "✓ Car " << carID << " successfully exited from Lane " << (i + 1) << endl;
            cout << "========================================" << endl;
            cout << "Cars moved temporarily: " << movedCount << endl;
            cout << "Total movements: " << moves << endl;
            cout << "Entry time: " << targetCar.entryHour << ":00" << endl;
            cout << "Exit time: " << exitHour << ":00" << endl;
            cout << "Hours parked: " << hoursParked << endl;
            cout << "Parking fee: " << fee << " units (@ " << pricePerHour << " units/hour)" << endl;
            cout << "========================================\n" << endl;
            
            return true;
        }
        
        cout << "\n✗ Car " << carID << " not found in any lane!" << endl;
        return false;
    }
    
    // Display all lanes
    void displayLanes() {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║       PARKING LOT STATUS              ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        
        for (int i = 0; i < laneCount; i++) {
            cout << "\nLane " << (i + 1) << " [" << lanes[i].size() 
                 << "/" << laneCapacity << "]: ";
            
            if (lanes[i].isEmpty()) {
                cout << "EMPTY" << endl;
                continue;
            }
            
            // Store cars temporarily to display them
            Car* buffer = new Car[laneCapacity];
            int count = 0;
            
            while (!lanes[i].isEmpty() && count < laneCapacity) {
                lanes[i].pop(buffer[count]);
                count++;
            }
            
            cout << "TOP → ";
            for (int j = 0; j < count; j++) {
                cout << "Car " << buffer[j].carID;
                if (j + 1 < count) cout << " → ";
            }
            cout << " → BOTTOM" << endl;
            
            // Restore cars back to lane
            for (int j = count - 1; j >= 0; j--) {
                lanes[i].push(buffer[j]);
            }
            
            delete[] buffer;
        }
        cout << "\n" << endl;
    }
    
    // Show statistics
    void showStatistics() {
        int totalCars = 0;
        int totalCapacity = laneCount * laneCapacity;
        
        for (int i = 0; i < laneCount; i++) {
            totalCars += lanes[i].size();
        }
        
        cout << "\n--- Parking Statistics ---" << endl;
        cout << "Total lanes: " << laneCount << endl;
        cout << "Capacity per lane: " << laneCapacity << endl;
        cout << "Total capacity: " << totalCapacity << endl;
        cout << "Currently parked: " << totalCars << endl;
        cout << "Available spots: " << (totalCapacity - totalCars) << endl;
        cout << "Occupancy rate: " << (totalCars * 100 / totalCapacity) << "%" << endl;
        cout << endl;
    }
};

// ============== MAIN FUNCTION ==============
int main() {
    int laneCount, laneCapacity, pricePerHour;
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║  PARKING LOT MANAGEMENT SYSTEM        ║" << endl;
    cout << "║  INHA UNIVERSITY IN TASHKENT          ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    
    cout << "Enter number of parking lanes: ";
    cin >> laneCount;
    
    cout << "Enter capacity per lane: ";
    cin >> laneCapacity;
    
    cout << "Enter price per hour (default 20): ";
    cin >> pricePerHour;
    
    ParkingLot lot(laneCount, laneCapacity, pricePerHour);
    
    while (true) {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║           MAIN MENU                   ║" << endl;
        cout << "╠════════════════════════════════════════╣" << endl;
        cout << "║  1. Park a car                        ║" << endl;
        cout << "║  2. Remove a car                      ║" << endl;
        cout << "║  3. Display all lanes                 ║" << endl;
        cout << "║  4. Show statistics                   ║" << endl;
        cout << "║  5. Exit                              ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "Choose option: ";
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n✗ Invalid input! Please enter a number." << endl;
            continue;
        }
        
        if (choice == 1) {
            int carID, entryHour;
            cout << "\nEnter car ID: ";
            cin >> carID;
            cout << "Enter entry hour (0-23): ";
            cin >> entryHour;
            lot.parkCar(carID, entryHour);
            
        } else if (choice == 2) {
            int carID, exitHour;
            cout << "\nEnter car ID to remove: ";
            cin >> carID;
            cout << "Enter exit hour (0-23): ";
            cin >> exitHour;
            lot.removeCar(carID, exitHour);
            
        } else if (choice == 3) {
            lot.displayLanes();
            
        } else if (choice == 4) {
            lot.showStatistics();
            
        } else if (choice == 5) {
            cout << "\n✓ Thank you for using the Parking Lot Management System!" << endl;
            cout << "Exiting...\n" << endl;
            break;
            
        } else {
            cout << "\n✗ Invalid choice! Please select 1-5." << endl;
        }
    }
    
    return 0;
}