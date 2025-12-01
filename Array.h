#pragma once
#include<iostream>
#include<stdexcept>


// this going to be file for Array class
template<typename T>
class Array
{
public:
    T* arr;
    int size = 0;
    
    // Default Contructor
    Array() : arr(nullptr), size(0) {}
    
    // Parametrized constructor
    Array(int n) 
    {
        arr = new T[n];
        size = n;
    }

    // Copy constructor
    Array(const Array& other)
    {
        size = other.size;
        if(size > 0)
        {
            arr = new T[size];
            for(int i = 0; i < size; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        else
        {
            arr = nullptr;
        }
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
            // Deleting from dynamic memory incase of copying from another array
            if(arr != nullptr)
            {
                delete[] arr;
            }

            size = other.size;
            arr = new T[size];

            for(int i = 0; i < size; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    // for non constant elements
    T& operator[](int index)
    {
        if(index < 0 || index >= size)
        {
            throw std::out_of_range("Error: Out of Range");
        }

        return arr[index];
    }

    // for constant elements
    const T& operator[](int index) const
    {
        if(index < 0 || index >= size)
        {
            throw std::out_of_range("Error: Out of Range");
        }

        return arr[index];
    }

    int getSize() const
    {
        return size;
    }
    
    // if i want to print elements
    friend std::ostream& operator<<(std::ostream& out, const Array<T>& a)
    {
        for(int i = 0; i < a.size; i++)
        {
            out << "Lane : " << i << ": "<< a.arr[i] << std::endl;
        }

        return out;
    }
};
