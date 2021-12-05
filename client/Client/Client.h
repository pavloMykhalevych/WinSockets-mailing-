#pragma once

#include "Includes.h"

void ClientThread(void* Param);

class Client
{
	friend void ClientThread(void* Param);
public:
	Client();

	~Client() = default;

	void Start();
private:
	void ClientClassThread();

	SOCKET m_connection;

	SOCKADDR_IN m_addr;
	int m_sizeofaddr = sizeof(m_addr);
};

