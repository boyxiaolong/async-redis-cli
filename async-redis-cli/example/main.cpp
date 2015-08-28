#include "stdio.h"
#include "Redis_Connection.h"

int main()
{
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);

	event_base* base = event_base_new();

	Redis_Connection conn(base);
	conn.asynConnect("192.168.106.60", 6379);

	while(true)
	{
		Sleep(1);
		event_base_loop(base, EVLOOP_NONBLOCK);
	}
}