#include<iostream>
#include<sstream>
#include<chrono>

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

    int getId() const { return id; }
    
    bool isParked() const { return parked; }
    void setParked(bool p) { parked = p; }
    
    static Car createNewCar() {
        return Car(++totalCars);
    }

    // Overload << for easy printing
    friend ostream& operator<<(ostream& os, const Car& c) {
        os << c.getId() << " | "<< c.getFormattedTime();
       
        return os;
    }
};

// static total cars
int Car::totalCars = 0;
