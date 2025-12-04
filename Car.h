#pragma once
#include<iostream>
#include<sstream>
#include<string>
#include<unordered_set>
#include<chrono>
#include<stdexcept>
#include<iomanip>
#include<ctime>

// iomanip for formatting
// ctime and chrono for usage of real time tracking


#define FEE_PER_HOUR 10000

// Car class
class Car {
private:
    int id;
    static int totalCars;

    // it will for not using same id ( it is done by set in this case)
    static std::unordered_set<int> usedIds;
    
    bool parked = false;
    std::chrono::system_clock::time_point entryTime;


public:    
    
    Car() : id(0) {}

    Car(int carId) 
    {
        // if there is already same id
        if(usedIds.count(carId) > 0)
        {
            throw std::invalid_argument("Duplicate Car ID: " + std::to_string(carId));
        }

        id = carId;
        usedIds.insert(id);

        if(carId > totalCars) { totalCars = carId; }
    }
    
    // copy constructor
    Car(const Car& other) : id(other.id), parked(other.parked), entryTime(other.entryTime) {}



    // marking entry time
    void markEntryTime()
    {
        parked = true;
        // setting entry time with use of chrono system_clock
        entryTime = std::chrono::system_clock::now();
    }
    
    // it will be on seconds kinda more logical it is neccesary for passing it into calculator function
    long long getParkedTimeSeconds() const
    {
        if(!parked)
        {
            return 0;
        }

        auto now = std::chrono::system_clock::now();
        auto duration = now - entryTime;

        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }

    // just for better looking output )
    std::string getFormattedTime() const
    {
        long long seconds = getParkedTimeSeconds();

        long long h = seconds / 3600;
        long long m = (seconds % 3600) / 60;
        long long s = seconds % 60;

        std::ostringstream out;
        out << h << "h "<< m << "m " << s<<"s ";
        return out.str(); 
    }

    // calculation of the car's cost
    double computeFee() const
    {
        long long seconds = getParkedTimeSeconds();
        return seconds * FEE_PER_HOUR / 3600.0;
    }

    // Get formatted fee with currency for output when we will remove car 
    std::string getFormattedFee() const
    {
        double fee = computeFee();
        std::ostringstream out;

        // formatting output with iomanip fixed 
        out << std::fixed << std::setprecision(2) << fee << " sum\n";
        return out.str();
    }

    // Get total parked time in hours (rounded up)
    long long getTotalTimeHours() const
    {
        long long seconds = getParkedTimeSeconds();
        return (seconds + 3599) / 3600; // round up to next hour
    }

    // Get total parked time in minutes
    long long getTotalTimeMinutes() const
    {
        long long seconds = getParkedTimeSeconds();
        return (seconds + 59) / 60; // round up to next minute
    }

    // Display full car information
    void displayInfo() const
    {
        std::cout << "Car ID: " << id << std::endl;
        std::cout << "Parked: " << (parked ? "Yes" : "No") << std::endl;
        if (parked) {
            std::cout << "Parked Time: " << getFormattedTime() << std::endl;
            std::cout << "Time (Hours): " << getTotalTimeHours() << "h" << std::endl;
            std::cout << "Time (Minutes): " << getTotalTimeMinutes() << "m" << std::endl;
            std::cout << "Fee: " << getFormattedFee() << std::endl;
        }
    }

    // Reset parked status (for removal)
    void resetParked()
    {
        parked = false;

        // Free up this ID so it can be reused after the car leaves the lot
        usedIds.erase(id);
    }

    // Get entry time as string (for logging)
    std::string getEntryTimeString() const
    {
        if (!parked) return "Not parked";
        
        // it will cast entry time into seconds it used to be sth like 12:34:45
        auto sctp = std::chrono::time_point_cast<std::chrono::seconds>(entryTime);
        
        // to_time_t will make it more readable 
        auto tt = std::chrono::system_clock::to_time_t(sctp);
        
        std::ostringstream oss;
        oss << std::ctime(&tt);
        std::string result = oss.str();
    
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        return result;
    }

    int getId() const { return id; }
    
    bool isParked() const { return parked; }
    void setParked(bool p) { parked = p; }
    
    static Car createNewCar() {
        return Car(++totalCars);
    }

    // Check if a given ID is currently in use (parked somewhere)
    static bool isIdInUse(int carId) {
        return usedIds.count(carId) > 0;
    }

    // Check if a given ID is currently available (not parked)
    static bool isIdAvailable(int carId) {
        return !isIdInUse(carId);
    }

    // Overload << for printing car information
    friend std::ostream& operator<<(std::ostream& os, const Car& c) {
        os << c.getId() << " | "<< c.getFormattedTime() << "| Fee: " << c.getFormattedFee() << std::endl;
        return os;
    }
};

// static total cars
int Car::totalCars = 0;

// empty userIds set
std::unordered_set<int> Car::usedIds;
