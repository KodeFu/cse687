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
// 5/26/2020	David Pretola			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* CSE 687 Object Oriented Programming Group Project
* This package provides an interprocess queue utilizing sockets.
* Written by David Pretola
*/
#ifndef _PROCESS_MESSAGE_QUEUE_H
#define _PROCESS_MESSAGE_QUEUE_H

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <future>
#include <errno.h>
#include <winsock2.h>
#include <iostream>
#include <cstdint>
#include "ThreadMessageQueue.h"

using namespace std;
using namespace std::chrono;

struct ServerListenThread
{
	void operator()(SOCKET* ls, SOCKET* qs, condition_variable * sock_cond);
};

/**************************************************************
The Interprocess Queue
**************************************************************/
class ProcessMessageQueue
{
private:
	mutex deq_mtx; /* The standard lib mutex for thread safety */
	mutex enq_mtx;
	condition_variable deq_cond; /* block the thread while the queue is empty */
	WSADATA wsaData{ 0 };
	SOCKET QueueSocket{ 0 };
	SOCKET ListenSocket{ 0 };
	condition_variable sock_cond; /* block the thread while the queue socket is not connected */
public:
	ProcessMessageQueue();
	ProcessMessageQueue(ProcessMessageQueue&) = delete; /* no copy constructor */
	~ProcessMessageQueue();
	ProcessMessageQueue& operator=(const ProcessMessageQueue&) = delete; /* no assignment operation */
	Message Dequeue();
	void Enqueue(Message);
	bool isEmpty();	
	void ClientConnect(string ip, int port);
	void ServerListen(string ip, int port);
};

/* temporary message short transfer struct */
struct MessageShort
{
	unsigned int sourceAddr; /* message source address */
	unsigned int sourcePort; /* message source port */
	unsigned int destAddr; /* message destination address */
	unsigned int destPort; /* message destination port */
	char messageType[90]; /* message type identifier */
	char functionName[90]; /* function name */
	long long timeStamp; /* The time for the message */
	char filePath[90]; /* the full file path to the test DLL */
	unsigned int testNumber; /* the test number to execute */
	char message[90]; /* generic message data */
};
#endif
