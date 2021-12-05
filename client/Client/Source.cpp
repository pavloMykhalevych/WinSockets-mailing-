#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

#include "Client.h"

int main(int argc, char* argv[]) {
	Client client;
	client.Start();

	std::cout << "\nThanks for using our service!";
	system("pause");
	return 0;
}