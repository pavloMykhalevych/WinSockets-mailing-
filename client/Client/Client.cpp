#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

#include "Client.h"

Client::Client() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_connection = socket(AF_INET, SOCK_STREAM, NULL);
}

void Client::Start() {
	if (connect(m_connection, (SOCKADDR*)&m_addr, sizeof(m_addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return;
	}
	std::cout << "Connected!\n";
	HANDLE clientThread;
	clientThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, (void *)this, NULL, NULL);

	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		if (msg1 == "Exit") {
			int msg_size = msg1.size();
			send(m_connection, (char*)&msg_size, sizeof(int), NULL);
			int err = send(m_connection, msg1.c_str(), msg_size, NULL);
			TerminateThread(clientThread,NULL);
			break;
		}
		int msg_size = msg1.size();
		send(m_connection, (char*)&msg_size, sizeof(int), NULL);
		int err = send(m_connection, msg1.c_str(), msg_size, NULL);
		if (err == SOCKET_ERROR) {
			std::cout << "\nServer turns off!\nExiting...";
			break;
		}
		//Sleep(10);
	}
	std::cout << "\nThanks for using our service!";
}

void Client::ClientClassThread() {
	int msg_size;
	while (true) {
		int err = recv(m_connection, (char*)&msg_size, sizeof(int), NULL);
		if (err == SOCKET_ERROR) {
			//std::cout << "\nServer turns off!\nExiting...";
			//std::cout << "\nThanks for using our service!";
			break;
		}
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(m_connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		if (!strcmp(msg, "Server turns off!\nExiting...")) {
			break;
		}
		delete[] msg;
	}
}

void ClientThread(void* Param) {
	Client* This = (Client*)Param;
	This->ClientClassThread();
}