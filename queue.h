#include <iostream>
using namespace std;

// exception class for a full queue
class FullQueue {};

// exception class for an empty queue
class EmptyQueue {};

// structure for each node in the linked list queue
template <class ItemType>
struct NodeType {
    ItemType info; // holds the value of the current node
    NodeType *next; // pointer to the next node in the queue
};

// template class to implement a queue using linked list
template <class ItemType>
class Queue {
    private:
        NodeType<ItemType> *front; // pointer to the front of the queue
        NodeType<ItemType> *rear; // pointer to the rear of the queue
    public:
        Queue(); // constructor to initialize the queue
        ~Queue(); // destructor to clean up the queue
        void Purge(); // removes all elements from the queue
        void Enqueue(ItemType); // adds an element to the rear of the queue
        void Dequeue(ItemType &); // removes an element from the front of the queue
        bool IsEmpty() const; // checks if the queue is empty
        bool IsFull() const; // checks if the queue is full
        ItemType Front() const; // retrieves the front element without removing it
        int Size() const; // calculates and returns the size of the queue
        void EnqueueToFront(ItemType &); // adds an element to the front of the queue
};

// constructor: initializes the queue as empty
template <class ItemType>
Queue<ItemType>::Queue() {
    front = NULL; // set front pointer to null
    rear = NULL; // set rear pointer to null
}

// destructor: clears the queue and releases memory
template <class ItemType>
Queue<ItemType>::~Queue() {
    Purge(); // calls purge to delete all nodes
}

// purges all elements in the queue and frees memory
template <class ItemType>
void Queue<ItemType>::Purge() {
    NodeType<ItemType> *tempPtr; // temporary pointer to hold nodes for deletion
    while (front != NULL) { // iterate while there are nodes in the queue
        tempPtr = front; // store the current front node
        front = front->next; // move front pointer to the next node
        delete tempPtr; // delete the stored node
    }
    rear = NULL; // set rear pointer to null after clearing the queue
}

// checks if the queue is empty
template <class ItemType>
bool Queue<ItemType>::IsEmpty() const {
    return (front == nullptr); // returns true if front is null
}

// checks if the queue is full
template <class ItemType>
bool Queue<ItemType>::IsFull() const {
    NodeType<ItemType> *location; // pointer to check for memory allocation
    try {
        location = new NodeType<ItemType>; // attempt to allocate a new node
        delete location; // delete the node immediately after allocation
        return false; // return false if allocation succeeded
    } catch (bad_alloc exception) { // catch allocation failure
        return true; // return true if memory allocation failed
    }
}

// adds a new element to the rear of the queue
template <class ItemType>
void Queue<ItemType>::Enqueue(ItemType item) {
    if (IsFull()) // check if the queue is full
        throw FullQueue(); // throw exception if queue is full

    NodeType<ItemType> *tmpNode; // create a new node
    tmpNode = new NodeType<ItemType>; // allocate memory for the new node
    tmpNode->info = item; // set the value of the node
    tmpNode->next = NULL; // set next pointer to null as it's the last node

    if (rear == NULL) // check if the queue is currently empty
        front = tmpNode; // set front to the new node
    else
        rear->next = tmpNode; // link the new node to the end of the queue

    rear = tmpNode; // update the rear pointer to the new node
}

// removes an element from the front of the queue
template <class ItemType>
void Queue<ItemType>::Dequeue(ItemType &item) {
    if (IsEmpty()) // check if the queue is empty
        throw EmptyQueue(); // throw exception if queue is empty

    NodeType<ItemType> *tempPtr; // temporary pointer to hold the node being removed
    tempPtr = front; // store the front node in the temporary pointer
    item = front->info; // retrieve the value of the front node
    front = front->next; // move front pointer to the next node

    if (front == NULL) // check if the queue is now empty
        rear = NULL; // set rear to null if queue is empty

    delete tempPtr; // delete the removed node
}

// retrieves the front element without removing it
template <typename ItemType>
ItemType Queue<ItemType>::Front() const {
    if (IsEmpty()) { // check if the queue is empty
        throw EmptyQueue(); // throw exception if queue is empty
    }
    return front->info; // return the value of the front node
}

// calculates and returns the size of the queue
template <typename ItemType>
int Queue<ItemType>::Size() const {
    int count = 0; // counter to track the number of nodes
    NodeType<ItemType> *p = front; // pointer to traverse the queue
    while (p != nullptr) { // iterate through the queue
        count++; // increment the counter for each node
        p = p->next; // move to the next node
    }
    return count; // return the total count
}

// adds a new element to the front of the queue
template <typename ItemType>
void Queue<ItemType>::EnqueueToFront(ItemType &newItem) {
    NodeType<ItemType> *newNode = new NodeType<ItemType>; // create a new node
    newNode->info = newItem; // set the value of the new node
    newNode->next = front; // link the new node to the current front
    front = newNode; // update the front pointer to the new node

    if (rear == nullptr) { // if the queue was empty
        rear = newNode; // update rear to the new node
    }
}
