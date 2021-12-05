#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

#include "Server.h"


int main() {
	Server server(30, 20, 10);
	server.Start();

	system("pause");
	return 0;
}
