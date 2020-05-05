//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/22/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/27/2020	David Pretola			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* CSE 687 Object Oriented Programming Group Project
* This package provides a Thread and Process Safe Queue.
* Written by David Pretola
*/
#ifndef __Q_H
#define __Q_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;


/**************************************************************
  The Project Message Class
 *************************************************************/
struct Message
{
	unsigned int sourceAddr; /* message source address */
	unsigned int sourcePort; /* message source port */
	unsigned int destAddr; /* message destination address */
	unsigned int destPort; /* message destination port */
	string messageType; /* message type identifier */
	string functionName; /* function name */
	chrono::time_point<system_clock> timeStamp; /* The time for the message */
	string filePath; /* the full file path to the test DLL */
	unsigned int testNumber; /* the test number to execute */
	string message; /* generic message data */
};

/**************************************************************
The Interthread Queue
**************************************************************/
template <typename T>
class ThreadMessageQueue
{
private:
	queue<T> q; /* The standard lib queue class */
	mutex deq_mtx; /* The standard lib mutex for thread safety */
	mutex enq_mtx;
	condition_variable deq_cond; /* block the thread while the queue is empty */
public:
	ThreadMessageQueue<T>();
	ThreadMessageQueue<T>(ThreadMessageQueue&) = delete; /* no copy constructor */
	~ThreadMessageQueue() = default;
	ThreadMessageQueue<T>& operator=(const ThreadMessageQueue<T>&) = delete; /* no assignment operation */
	T Dequeue();
	void Enqueue(T);
};

/* The ThreadMessageQueue Constructor */
template <typename T>
ThreadMessageQueue<T>::ThreadMessageQueue() : q{}
{
}

template <typename T>
T ThreadMessageQueue<T>::Dequeue()
{
	unique_lock<std::mutex> lock{ deq_mtx }; /* lock the queue */
	if (q.empty()) /* then block until an element is enqued */
	{
		/* release the lock and wait for the qeue to be non empty, then relock */
		deq_cond.wait(lock, [this] {return !q.empty();});
	}
	T ele = q.front();
	q.pop();
	return ele;
}

template <typename T>
void ThreadMessageQueue<T>::Enqueue(T element)
{
	unique_lock<std::mutex> lock{ enq_mtx }; /* lock the queue */
	q.push(element);
	deq_cond.notify_one(); /* inform the waiting threads of an enqueue */
}

#endif
