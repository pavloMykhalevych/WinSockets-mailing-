#include "Mailing.h"

Mailing::Mailing(int time) : m_period(time) {

}

void Mailing::AddSubscribed(SOCKET newClient) {
	m_subscribed.insert(newClient);
}

void Mailing::SendMail(std::string mes) {
	for (const auto& connection : m_subscribed) {
		SendString(mes, connection);
	}
}

bool Mailing::Find(SOCKET connection) {
	if (m_subscribed.find(connection) != m_subscribed.end()) {
		return true;
	}
	else {
		return false;
	}
}

std::string Mailing::Delete(SOCKET connection) {
	if (Find(connection)) {
		SendString("Are you sure?(y/n)", connection);
		int msg_size;
		while (true) {
			int err = recv(connection, (char*)&msg_size, sizeof(int), NULL);
			if (err == SOCKET_ERROR) {
				std::cout << currentDateTime() << "Client has disconnected.\n";
				break;
			}
			char* msg = new char[msg_size + 1];
			msg[msg_size] = '\0';
			recv(connection, msg, msg_size, NULL);
			if (!strcmp(msg, "y")) {
				break;
			}
			else {
				return "";
			}
		}
		m_subscribed.erase(connection);
		return "You have been successfully unsubscribed. Sorry for annoying you ;(";
	}
	else {
		return "You haven't been subscribed on our service.";
	}
}