#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"

void ServerThread(void* Param) {
	ClientIndex* structClient = (ClientIndex*)Param;
	structClient->server->ClientHandler(structClient->index);
}

Server::Server(int time_1, int time_2, int time_3) {
	m_weatherMail = new Weather(time_1 * 1000);
	m_currencyMail = new Currency(time_2 * 1000);
	m_sharesMail = new Shares(time_3 * 1000);

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error: WSAStartup() fails.\n" << std::endl;
		exit(1);
	}

	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_sListen = socket(AF_INET, SOCK_STREAM, NULL);
}

void Server::Start() {
	bind(m_sListen, (SOCKADDR*)&m_addr, sizeof(m_addr));
	listen(m_sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(m_sListen, (SOCKADDR*)&m_addr, &m_sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error: can't connect client.\n";
		}
		else {
			std::cout << currentDateTime() <<"Client Connected.\n";
			SendString("Welcome to the mailing service)\nIf you want to subscribe on some services, choose one from this:\n"
				"1) Weather mailing;\n2) Currency mailing;\n3) Shares mailing;\n"
				"Note: If you will need to unsubscribe from one of services write: \"Delete\" and number of service.", newConnection);
			Connections.push_back(newConnection);
			Counter++;
			ClientIndex structClient = ClientIndex{ i, this };
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerThread, (void*)(&structClient), NULL, NULL);
		}
	}
}

void Server::ClientHandler(int index) {
	int msg_size;
	while (true) {
		int err = recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		if (err == SOCKET_ERROR) {
			std::cout << currentDateTime() << "Client has disconnected.\n";
			break;
		}
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		if (!strcmp(msg, "1")) {
			if (!m_weatherMail->Find(Connections[index])) {
				m_weatherMail->AddSubscribed(Connections[index]);
				SendString("You have been subscribed on weather mailing. Thanks!", Connections[index]);
			}
			else {
				SendString("You have been already subscribed on weather mailing. There is no point to do it again.", Connections[index]);
			}
		}
		else if (!strcmp(msg, "2")) {
			if (!m_currencyMail->Find(Connections[index])) {
				m_currencyMail->AddSubscribed(Connections[index]);
				SendString("You have been subscribed on currency mailing. Thanks!", Connections[index]);
			}
			else {
				SendString("You have been already subscribed on currency mailing. There is no point to do it again.", Connections[index]);
			}
		}
		else if (!strcmp(msg, "3")) {
			if (!m_sharesMail->Find(Connections[index])) {
				m_sharesMail->AddSubscribed(Connections[index]);
				SendString("You have been subscribed on shares mailing. Thanks!", Connections[index]);
			}
			else {
				SendString("You have been already subscribed on shares mailing. There is no point to do it again.", Connections[index]);
			}
		}
		else if (!strcmp(msg, "Delete 1")) {
			SendString(m_weatherMail->Delete(Connections[index]), Connections[index]);
		}
		else if (!strcmp(msg, "Delete 2")) {
			SendString(m_currencyMail->Delete(Connections[index]), Connections[index]);
		}
		else if (!strcmp(msg, "Delete 3")) {
			SendString(m_sharesMail->Delete(Connections[index]), Connections[index]);
		}
		else if (!strcmp(msg, "help")) {
			SendString("Welcome to the mailing service)\nIf you want to subscribe on some services, choose one from this:\n"
				"1) Weather mailing;\n2) Currency mailing;\n3) Shares mailing;\n"
				"Note: If you will need to unsubscribe from one of services write: \"Delete\" and number of service.", Connections[index]);
		}
		else if (!strcmp(msg, "Exit")) {
			m_weatherMail->Delete(Connections[index]);
			m_currencyMail->Delete(Connections[index]);
			m_sharesMail->Delete(Connections[index]);
		}
		else {
			SendString("Unknown command, try to write it again.", Connections[index]);
		}
		delete[] msg;
	}
}

Server::~Server() {
	for (auto& con : Connections) {
		SendString("Server turns off!\nExiting...", con);
		closesocket(con);
	}
	closesocket(m_sListen);
	WSACleanup();
}