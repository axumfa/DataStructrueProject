#include<iostream>

using namespace std;


// this going to be file for Array class
template<typename T>
class Array
{
public:
    T* arr;
    int size = 0;
    int cur = 0; // it is for keeping place pointer
    
    // Default Contructor
    Array() : arr(nullptr), size(0) {}
    
    // Parametrized constructor
    Array(int n) 
    {
        arr = new T[n];
        size = n;
    }

    // Destructor
    ~Array()
    {
        delete[] arr;
    }

/*Operator overloading*/
    //equal when we want to assign two arrays into each other
    Array& operator=(const Array& other)
    {
        if(this != &other)
        {
            delete[] arr;
            size = other.size;
            arr = new T[size];
            cur = other.cur;

            for(int i = 0; i < size; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    T& operator[](int index)
    {
        if(index < 0 || index >= size)
        {
            cout<<"Index Out of Bounds\n";
            exit(1);
        }

        return arr[index];
    }

    int getSize()
    {
        return size;
    }

    T& getLane(int index)
    {
        if(index < 0 || index >= size)
        {
            cout << "Error: Index Out of Bounds\n";
            throw exception("Index Out of Bounds\n");
        }

        return arr[index];
    }

    friend ostream& operator<<(ostream& out, const Array<T>& a)
    {
        for(int i = 0; i < a.size; i++)
        {
            out << i << " = "<< a.arr[i] << endl;
        }

        return out;
    }

    friend istream& operator>>(istream& in, Array<T>& a)
    {
        int laneNumber;
        auto cardId = 0;
        cout << "Enter lane number"
    } 
};
