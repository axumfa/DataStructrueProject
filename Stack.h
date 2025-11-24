#include<iostream>

using namespace std;

template<typename T>
class Stack
{
    public:
        T* arr;
        int size;
        int top;

        Stack
        
        Stack(int n)
        {
            arr = new T[n];
            size = n;
            top = -1;
        }

        Stack(const Stack& other)
        {
            size = other.size;
            if(size > 0)
            {
                arr = new T[size];
                for(int i = 0; i < size; i++)
                {
                    arr[i] = other.arr[i];
                }
                top = other.top;
            }
            else
            {
                arr = nullptr;
            }            
        }

        ~Stack()
        {
            delete[] arr;
        }
        
        void push(T a)
        {
            if(isFull())
            {
                throw overflow_error("Overflow, Stack is Full\n");
            }
            // ++top because we will start with -1 but our first element should be 0
            arr[++top] = a;
        }

        bool isEmpty() { return top == -1;  }
        bool isFull() { return top >= size - 1; }

        T pop()
        {
            if(isEmpty())
            {
                throw underflow_error("Underflow, Stack is Empty\n");
            }
            return arr[top--];
        }

        T peek()
        {
            if(isEmpty())
            {
                throw underflow_error("Underflow, Stack is Empty\n");
            }
            return arr[top];
        }      
};