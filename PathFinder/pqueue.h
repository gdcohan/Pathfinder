/*
 * File: pqueue.h
 * --------------
 * Defines the interface for the priority queue class. 
 * 
 * Julie Zelenski, CS106, Fall 2007
 */
 
 
#ifndef _pqueue_h
#define _pqueue_h

#include "genlib.h"
#include "vector.h"
#include "disallowcopy.h"
#include "cmpfn.h"
#include <iostream>


/*
 * Class: PQueue
 * -------------
 * This is the class for a priority queue.  This is not
 * simple FIFO queue, it is a priority queue, where elements are
 * retrieved in order of priority, not just by longevity in queue.
 * The elements are integers and the integer is assumed to represent 
 * the priority (larger integer is higher priority).
 */
template <typename ElemType>
class PQueue 
{
  public:

	/*
	 * Constructor: PQueue
	 * Usage: PQueue pq;
	 *        PQueue *ppq = new PQueue;
	 * ---------------------------------
	 * Initializes a new pqueue to have no elements.  
	 */
	PQueue(int (cmpFn)(ElemType, ElemType) = OperatorCmp);


	/*
	 * Destructor: ~PQueue
	 * Usage: delete ppq;
	 * ------------------
	 * Deallocates all the storage associated with this pqueue.
	 */
	~PQueue();


	/*
	 * Member function: isEmpty
	 * Usage: if (pq.isEmpty()) . . .
	 * -------------------------------
	 * Returns true if this pqueue contains no elements.
	 */
	bool isEmpty();


	/*
	 * Member function: size
	 * Usage: nElemes = pq.size();
	 * ---------------------------
	 * Returns number of elements contained in this pqueue.
	 */
	int size();


	/*
	 * Member function: enqueue
	 * Usage: pq.enqueue(val);
	 * -----------------------
	 * Adds the specified element to this pqueue. No effort is made to
	 * avoid duplicates. 
	 */
	void enqueue(ElemType newElem);


	/*
	 * Member function: eequeueMax
	 * Usage: maxElem = pq.dequeueMax();
	 * ---------------------------------
	 * Removes the largest priority element from this pqueue and returns it.
	 * If this pqueue is empty, this function raises an error.
	 */
	ElemType dequeueMax();


	/*
	 * Member function: bytesUsed
	 * Usage: numBytes = pq.bytesUsed();
	 * ----------------------------------
	 * This function would not usually be included as part of the class,
	 * but this is here as part of evaluating the tradeoffs betweem
	 * implementations. Given a pqueue, this function counts up
	 * and return the total amount of space used given its
	 * current contents.
	 */
	int bytesUsed();
	
	
	/*
	 * Member function: implementationName
	 * Usage: cout << pq.implementationName();
	 * ---------------------------------------
	 * This operation would not usually be included as part of the class
	 * class, but is included to help with analyzing and reporting results.
	 * This member function returns a string that describes the PQueue
	 * implementation strategy ("sorted linked list", "vector", etc.).
	 */
	string implementationName();


	/*
	 * Member function: printDebuggingInfo
	 * Usage: pq.printDebuggingInfo();
	 * -------------------------------
	 * This operation would not usually be included as part of the class,
	 * but is included to give you a hook to put any helpful debugging
	 * print code (for example, something that prints the goopy details of
	 * the internal structure). You don't need to implement this routine and can
	 * ignore it entirely, but you may find it helpful to use as you are
	 * doing development.
	 */
	void printDebuggingInfo();

  private:
  
	// If implemented using Vector data mamber, default memberwise copy 
	// works fine, but if implemented as linked list, (ie pointer data member)
	// copying would create unintended sharing.
	// It's fine is to disallow copying for all implementations as
	// a precaution
	DISALLOW_COPYING(PQueue)
	int (*cmpFn)(ElemType, ElemType);

  	/* This is the representation for the unsorted vector.
  	 * You will need to update this as you change representations. */
	Vector<ElemType> entries;

};


/*
 * File: pqvector.cpp
 * ------------------
 * This file implements the priority queue class as a vector
 * of integers stored in no particular order.  This makes it easy
 * enqueue new elements, but hard to dequeue the max (have to search for it).
 *
 * Julie Zelenski, CS106
 */

/* Implementation notes: PQueue class
 * ----------------------------------
 * The private section for the pqvector looks like this:
 *
 *	Vector<int> entries;
 */

template <typename ElemType>
PQueue<ElemType>::PQueue(int (cmp)(ElemType, ElemType))
{
	cmpFn = cmp;
}

template <typename ElemType>
PQueue<ElemType>::~PQueue()
{
}

template <typename ElemType>
bool PQueue<ElemType>::isEmpty()
{
    return (entries.isEmpty());  
}

template <typename ElemType>
int PQueue<ElemType>::size()
{
	return (entries.size());
}


/* Implementation notes: enqueue
 * -----------------------------
 * Since we're keeping the vector in no particular order, we just append this
 * new element to the end.  It's the easiest/fastest thing to do.
 */
template <typename ElemType>
void PQueue<ElemType>::enqueue(ElemType newValue)
{
	entries.add(newValue);
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * Since we're keeping the vector in no particular order, we have to search to
 * find the largest element.  Once found, we remove it from the vector and
 * return that value.
 */
template <typename ElemType>
ElemType PQueue<ElemType>::dequeueMax()
{	
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");
	
	int maxIndex = 0;	// assume first element is largest until proven otherwise
	ElemType maxValue = entries[0];
	for (int i = 1; i < entries.size(); i++) {
		if (cmpFn(entries[i], maxValue) == 1) {
			maxValue = entries[i];
			maxIndex = i;
		}
	}
	entries.removeAt(maxIndex);	// remove entry from vector
	return maxValue;
}

template <typename ElemType>
int PQueue<ElemType>::bytesUsed() 
{
	return sizeof(*this) + entries.bytesUsed();
}

template <typename ElemType>
string PQueue<ElemType>::implementationName()
{
	return "unsorted vector";
}

template <typename ElemType>
void PQueue<ElemType>::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << entries.size() << " entries" << endl;
	for (int i = 0; i < entries.size(); i++) 
		cout << entries[i] << " ";
	cout << endl;
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}



#endif