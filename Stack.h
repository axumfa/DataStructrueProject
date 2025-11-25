#include<iostream>

using namespace std;
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
        Stack() : arr(new T[size]), size(DEFAULT_SIZE), top(-1) {}    

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

        friend ostream& operator<<(ostream& os, const Stack<T>& s)
        {
            if(s.isEmpty())
            {
                os <<  "Stack is empty";
                return os;
            }

            os << "Stack (top to bottom): ";
            for(int i = s.top; i >= 0; i--)
            {
                os << s.arr[i];
                if(i > 0) os << " ";
            }
            return os;
        }
        Stack& operator=(const Stack& other)
        {
            if(this != &other)
            {
                Stack temp(other);

                swap(arr, temp.arr);
                swap(size, temp.size);
                swap(top, temp.top);
            }

            return *this;
        }
        
        void push(const T& a)
        {
            if(isFull())
            {
                throw overflow_error("Overflow, Stack is Full\n");
            }
            // ++top because we will start with -1 but our first element should be 0
            arr[++top] = a;
        }

        bool isEmpty() const { return top == -1;  }
        bool isFull() const { return top >= size - 1; }
        int getSize() const { return size; }
        
        // I will used for default constructor
        void setSize()
        {
            int a;
            cout << "Enter size of the Stack: ";
            cin >> a;
            size = a; 
        }


        T pop()
        {
            if(isEmpty())
            {
                throw underflow_error("Underflow, Stack is Empty\n");
            }
            return arr[top--];
        }

        T& peek()
        {
            if(isEmpty())
            {
                throw underflow_error("Underflow, Stack is Empty\n");
            }
            return arr[top];
        }
        
        const T& peek() const{
            if(isEmpty())
            {
                throw underflow_error("Underflow, Stack is Empty\n");
            }
            return arr[top];
        }
};