#include<iostream>
using namespace std;

#define MAX_SIZE 100
template<typename T>
class Queue
{
    private:
        T* arr;
        int size;
        int front;
        int rear;

    public:
        // Default constructor
        Queue() : arr(new T[MAX_SIZE]), size(MAX_SIZE), front(-1), rear(-1) {}
        Queue(int n) : arr(new T[n]), size(n), front(-1), rear(-1) {}
        
        // Destructor
        ~Queue() 
        {
            delete[] arr;
        }

        bool isEmpty() { return front == -1 || front > rear; }
        bool isFull() { return rear == size - 1; }

        T getFront()
        {
            if(isEmpty())
            {
                cout << "Queue is empty" << endl;
                return;
            }
            return arr[front];
        }

        T getRear()
        {
            if(isEmpty())
            {
                cout << "Queue is empty\n";
                return; 
            }
            return arr[rear];
        }

        void enqueue(T val)
        {
            if(isFull())
            {
                cout << "Queue is full\n";
                throw runtime_error("Queue is empty");
            }

            if(isEmpty())
            {
                front = 0;
            }

            arr[++rear] = val;
            cout << " Element added into Queue: "<< val;
        }

        void dequeue()
        {
            if(isEmpty())
            {
                cout<<"Queue is Empty\n";
                return;
            }

            front++;
            
            // I should check whether deletion will be or not it can cause Memory L
        }
};