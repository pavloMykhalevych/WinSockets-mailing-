#pragma once

#include "Includes.h"
#include "Weather.h"
#include "Currency.h"
#include "Shares.h"

void ServerThread(void* Param);

class Server
{
public:
	friend void ServerThread(void* Param);

	// Write seconds, later it will be multiplied by 1000.
	// time_1 - weather
	// time_2 - currency
	// time_3 - shares
	Server(int time_1 = 60, int time_2 = 10, int time_3 = 25);

	void Start();

	void ClientHandler(int index);

	~Server();

private:
	SOCKET m_sListen;
	SOCKADDR_IN m_addr;
	int m_sizeofaddr = sizeof(m_addr);

	std::vector<SOCKET> Connections;
	int Counter = 0;

	Weather* m_weatherMail;
	Currency* m_currencyMail;
	Shares* m_sharesMail;
};

struct ClientIndex {
	int index;
	Server* server;
};