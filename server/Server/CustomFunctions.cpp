#include "CustomFunctions.h"

void SendString(std::string mes, SOCKET Connection) {
	int msg_size = mes.size();
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, mes.c_str(), msg_size, NULL);
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm*  tstruct = new tm();
    char       buf[80];
    localtime_s(tstruct, &now);
    strftime(buf, sizeof(buf), "[%Y-%m-%d  %X]", tstruct);

    return buf;
}