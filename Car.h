#pragma once
#include<iostream>
#include<sstream>
#include<chrono>
#include<stdexcept>
#include<iomanip>
#include<ctime>

#define RATE_PER_HOUR 10000

// Car class
class Car {
private:
    int id;
    static int totalCars;
    bool parked = false;
    chrono::system_clock::time_point entryTime;


public:    
    
    Car() : id(0) {}

    Car(int carId) : id(carId) {}

    void markEntryTime()
    {
        parked = true;
        entryTime = chrono::system_clock::now();
    }
    
    // it will be on seconds so while showing demo it will more logical
    long long getParkedTimeSeconds() const
    {
        if(!parked)
        {
            return 0;
        }

        auto now = chrono::system_clock::now();
        auto duration = now - entryTime;

        return chrono::duration_cast<chrono::seconds>(duration).count();
    }

    string getFormattedTime() const
    {
        long long seconds = getParkedTimeSeconds();

        long long h = seconds / 3600;
        long long m = (seconds % 3600) / 60;
        long long s = seconds % 60;

        ostringstream out;
        out << h << "h "<< m << "m " << s<<"s ";
        return out.str(); 
    }

    double computeFee() const
    {
        long long seconds = getParkedTimeSeconds();
        double ratePerSecond = RATE_PER_HOUR / 3600.0;
        return seconds * ratePerSecond;
    }

    // Get formatted fee with currency
    string getFormattedFee() const
    {
        double fee = computeFee();
        ostringstream out;
        out << std::fixed << std::setprecision(2) << fee << " units";
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
    }

    // Get entry time as string (for logging)
    std::string getEntryTimeString() const
    {
        if (!parked) return "Not parked";
        
        auto sctp = std::chrono::time_point_cast<std::chrono::seconds>(entryTime);
        auto tt = std::chrono::system_clock::to_time_t(sctp);
        std::ostringstream oss;
        oss << std::ctime(&tt);
        std::string result = oss.str();
        // Remove trailing newline
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

    // Overload << for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Car& c) {
        os << c.getId() << " | "<< c.getFormattedTime() << "| Fee: " << c.getFormattedFee();
        return os;
    }
};

// static total cars
int Car::totalCars = 0;
