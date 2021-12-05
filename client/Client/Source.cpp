#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
	int msg_size;
	while (true) {
		int err = recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		if (err == SOCKET_ERROR) {
			std::cout << "\nServer turns off!\nExiting...";
			std::cout << "\nThanks for using our service!";
			break;
		}
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		if (!strcmp(msg, "Server turns off!\nExiting...")) {
			break;
		}
		delete[] msg;
	}
}

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		if (msg1 == "Exit") {
			break;
		}
		int msg_size = msg1.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		int err = send(Connection, msg1.c_str(), msg_size, NULL);
		if (err == SOCKET_ERROR) {
			std::cout << "\nServer turns off!\nExiting...";
			break;
		}
		Sleep(10);
	}
	std::cout << "\nThanks for using our service!";
	system("pause");
	return 0;
}