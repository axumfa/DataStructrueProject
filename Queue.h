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
        
        //copy constructor
        Queue(const Queue& other) : size(other.size), front(other.front), rear(other.rear) 
        {
            arr = new T[size];
            for(int i = 0; i < size; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        
        // Copy assignment operator
        Queue& operator=(const Queue& other)
        {
            if(this != &other)
            {
                delete[] arr;
                size = other.size;
                front = other.front;
                rear = other.rear;
                arr = new T[size];

                for(int i = 0; i < size; i++)
                    arr[i] = other.arr[i];
            }

            return *this;
        }

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
                throw runtime_error("Queue is empty");
            }
            return arr[front];
        }

        T getRear()
        {
            if(isEmpty())
            {
                throw runtime_error("Queue is empty");
            }
            return arr[rear];
        }

        void enqueue(T val)
        {
            if(isFull())
            {
                cout << "Queue is full\n";
                return;
            }

            if(isEmpty())
                front = 0;
            

            arr[++rear] = val;
            cout << " Element added into Queue: "<< val;
            
        }

        T dequeue()
        {
            if(isEmpty())
            {
                
                throw runtime_error("Queue is empty");
            }

            T ans = arr[front++];


            // I should check whether deletion will be or not it can cause Memory L
            if (isEmpty())
                front = rear = -1;
            
            return ans;
        }

        void display()
        {
            if (isEmpty()) {
                cout << "Queue is empty" << endl;
                return;
            }
            cout << "Queue:  ";
            for (int i = front; i <= rear; i++) {
                cout << arr[i] << " ";
            }

            cout << endl;
        }
};
