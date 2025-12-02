#pragma once
#include<iostream>
#include<ostream>
#include<utility>
#include<stdexcept>

const int DEFAULT_SIZE = 10;
template<typename T>
class Stack
{
    private:
        T* arr;
        int size;
        int top;
    
    public:
        // We can not allocate memory for empty(withuot size) stack 
        Stack() : arr(new T[DEFAULT_SIZE]), size(DEFAULT_SIZE), top(-1) {}    

        // Parametrized Constructor
        Stack(int n) : arr(new T[n]), size(n), top(-1) {}

        // Copy constructor
        Stack(const Stack& other) : arr(new T[other.size]), size(other.size), top(other.top)
        {
            try
            {
                for(int i = 0; i <= top; i++)
                    arr[i] = other.arr[i];
            } catch(...)
            {
                delete[] arr;
                throw;
            }

        }
        ~Stack()
        {
            delete[] arr;
        }

        // overloading = operator
        Stack& operator=(const Stack& other)
        {
            if(this != &other)
            {
                // changing  this stack it is better
                Stack temp(other);

                // swap will just swap for safety temp is used 
                std::swap(arr, temp.arr);
                std::swap(size, temp.size);
                std::swap(top, temp.top);
            }

            return *this;
        }
        
        void push(const T& a)
        {
            if(isFull())
            {
                throw std::overflow_error("Overflow! Stack is full");
            }
            
            // ++top because we will start with -1 but our first element should be 0
            arr[++top] = a;
        }

        bool isEmpty() const { return top == -1;  }
        bool isFull() const { return top >= size - 1; }
        int getSize() const { return size; }

        T pop()
        {
            if(isEmpty())
            {
                throw std::underflow_error("Underflow! Stack is empty");
            }
            return arr[top--];
        }

        T& peek()
        {
            if(isEmpty())
            {
                throw std::underflow_error("Underflow! Stack is empty");
            }
            return arr[top];
        }
        
        const T& peek() const{
            if(isEmpty())
            {
                throw std::underflow_error("Underflow! Stack is empty");
            }
            return arr[top];
        }
};

// it is helpfull cases when we will have stack<car> and another maybe array consist of sth 
// and it will be helpful for printing stack like cout << lane[i]
template<typename U>
std::ostream& operator<<(std::ostream& os, Stack<U> s)
{
    if (s.isEmpty()) {
        os << "Stack is empty";
        return os;
    }

    os << "Stack (top to bottom): ";
    bool first = true;
    while (!s.isEmpty()) {
        if (!first) os << " ";
        os << s.pop();
        first = false;
    }
    return os;
}