#include "pch.h"
#include "ProcessMessageQueue.h"
#include "ThreadMessageQueue.h"
#include <iostream>

using namespace std;
#pragma comment(lib, "Ws2_32.lib") /* this was required to link properly! */

/* local prototypes */
unique_ptr<MessageShort> binarySerialize(Message msg);
Message binaryUnSeralize(MessageShort* ele);

/*
 * Functor for the Server Listen Thread. Listen for connections on the ls socket and when
 * a connection is made, accept the connection and assign the connection to the qs socket.
 * Then notify the condition variable sock_cond listeners for the connection.
*/
void ServerListenThread::operator()(SOCKET* ls, SOCKET* qs, condition_variable* sock_cond)
{
	int status = listen(*ls, 1);
	if (status < 0)
	{
		int error = errno;
		char msg[256];
		std::cout << "Error in Listen call! " << strerror_s(msg, sizeof(msg), error) << " " << error << std::endl;
	}
	*qs = accept(*ls, nullptr, nullptr);
	if (*qs < 0)
	{
		int error = errno;
		char msg[256];
		std::cout << "Error in Accept call! " << strerror_s(msg, sizeof(msg), error) << " " << error << std::endl;
	}
	sock_cond->notify_one(); /* inform the waiting threads of a connection */
	cout << "connected!" << endl; //DEBUG
}

/* The MessageQueue Constructor */
ProcessMessageQueue::ProcessMessageQueue()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Windows Sockets start from the example */
	{
		cout << "Error loading the winsocket library!" << endl;
	}
}

/* The Queue Destructor */
ProcessMessageQueue::~ProcessMessageQueue()
{
	if (QueueSocket > 0) /* then shutdown the socket */
	{
		shutdown(QueueSocket, SD_BOTH);
		closesocket(QueueSocket);
	}
	if (ListenSocket > 0) /* then shutdown the listen socket */
	{
		shutdown(ListenSocket, SD_BOTH);
		closesocket(ListenSocket);
	}
	WSACleanup(); /* unload the winsocket library */
}

/*
 * Listen for a socket connection on the specified ip address and port number.
 * This is accomplished by launching a listening thread that listens for new
 * connections to the ListenSocket.
*/
void ProcessMessageQueue::ServerListen(string ip_str, int port_num)
{
	/* configure the address to connect to */
	struct sockaddr_in addr { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_str.c_str());
	addr.sin_port = htons(port_num);

	SOCKET socket_s = socket(AF_INET, SOCK_STREAM, 0); /* create the socket with no flags */
	if (socket_s < 0) /* then an error occured */
	{
		int error = errno;
		char msg[256];
		std::cout << "Error Creating Socket: " << strerror_s((char*)&msg, sizeof(msg), error) << " " << error << std::endl;
	}
	int foo = ::bind(socket_s, (sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (foo < 0)
	{
		int error = errno;
		char msg[256];
		std::cout << "Error Binding Socket: " << strerror_s((char*)&msg, sizeof(msg), error) << " " << error << std::endl;
	}
	ListenSocket = socket_s;

	/* call the thread to listen */
	ServerListenThread listener;
	thread t = thread{ listener,&ListenSocket,&QueueSocket,&sock_cond };
	t.detach();
}

/*
 * Connect to the specified ip address and port.
*/
void ProcessMessageQueue::ClientConnect(string ip_str, int port_num)
{
	/* configure the address to connect to */
	struct sockaddr_in addr { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_str.c_str());
	addr.sin_port = htons(port_num);

	SOCKET socket_s = socket(AF_INET, SOCK_STREAM, 0); /* create the socket with no flags */
	if (socket_s < 0) /* then an error occured */
	{
		int error = errno;
		char msg[256];
		std::cout << "Error Creating Socket: " << strerror_s((char*)&msg, sizeof(msg), error) << " " << error << std::endl;
	}
	int status = connect(socket_s, (const sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (status < 0) /* then an error occured */
	{
		int error = errno;
		char msg[256];
		std::cout << "Error Connecting Socket: " << strerror_s((char*)&msg, sizeof(msg), error) << " " << error << std::endl;
	}
	QueueSocket = socket_s;
	sock_cond.notify_one(); /* inform the waiting threads of a connection */
	cout << "Connected!" << endl; //DEBUG
}

Message ProcessMessageQueue::Dequeue()
{

	unique_lock<std::mutex> lock{ deq_mtx }; /* lock the queue */
	sock_cond.wait(lock, [this] {return QueueSocket != 0;}); /* wait until the socket is connected */
	MessageShort ele;
	int size = recv(QueueSocket, (char*)&ele, sizeof(ele), 0);
	if (size < 0)
	{
		int error = errno;
		char msg[256];
		std::cout << "Error: " << strerror_s((char*)&msg, sizeof(msg), error) << error << std::endl;
	}
	std::cout << "received " << size << " bytes" << std::endl;
	/* deserialize the T type instance */
	Message return_message = binaryUnSeralize(&ele);
	return return_message;
}

void ProcessMessageQueue::Enqueue(Message element)
{
	unique_lock<std::mutex> lock{ enq_mtx }; /* lock the send call */
	sock_cond.wait(lock, [this] {return QueueSocket != 0;}); /* wait until the socket is connected */
	/* serialize the T type instance */
	unique_ptr<MessageShort> buffer = binarySerialize(element);
	int size = send(QueueSocket, (char*)buffer.get(), sizeof(MessageShort), 0);
	printf("after sending\n"); //DEBUG
	if (size < 0)
	{
		int error = errno;
		char msg[256];
		std::cout << "Error " << strerror_s((char*)&msg, sizeof(msg), error) << error << std::endl;
	}
	std::cout << "send " << size << " bytes " << std::endl; //DEBUG
}

bool ProcessMessageQueue::isEmpty()
{
	unique_lock<std::mutex> lock1{ enq_mtx }; /* lock for the check */
	unique_lock<std::mutex> lock2{ deq_mtx };
	sock_cond.wait(lock2, [this] {return QueueSocket != 0;}); /* wait until the socket is connected */
	unsigned long number_of_bytes{ 0 };
	ioctlsocket(QueueSocket, FIONREAD, &number_of_bytes); /* are any bytes available? */
	if (number_of_bytes > 0) /* then the "queue" is not empty */
	{
		return false;
	}
	else
	{
		return true;
	}
}

unique_ptr<MessageShort> binarySerialize(Message msg)
{
	MessageShort * element = new MessageShort{};
	element->destAddr = msg.destAddr; /* message source address */
    element->sourcePort = msg.sourcePort; /* message source port */
	element->destAddr; /* message destination address */
	element->destPort; /* message destination port */
	strcpy_s(element->messageType,90,msg.messageType.c_str()); /* message type identifier */
	strcpy_s(element->functionName,90,msg.functionName.c_str()); /* function name */
	element->timeStamp = msg.timeStamp.time_since_epoch().count(); /* The time for the message */
	strcpy_s(element->filePath,90,msg.filePath.c_str()); /* the full file path to the test DLL */
	element->testNumber = msg.testNumber; /* the test number to execute */
	strcpy_s(element->message,90,msg.message.c_str()); /* generic message data */
	unique_ptr<MessageShort> return_msg{ element };
	return return_msg;
}

Message binaryUnSeralize(MessageShort * ele)
{
	Message msg;
	msg.sourceAddr = ele->sourceAddr;
	msg.sourcePort = ele->sourcePort;
	msg.destAddr = ele->destAddr;
	msg.messageType = ele->messageType;
	msg.functionName = ele->functionName;
	msg.filePath = ele->filePath;
	msg.testNumber = ele->testNumber;
	msg.message = ele->message;
	return msg;
}