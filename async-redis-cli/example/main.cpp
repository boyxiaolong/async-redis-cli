#include "stdio.h"
#include "Redis_Connection.h"

#ifndef _WIN32
#include <unistd.h>
#endif
int main()
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	event_base* base = event_base_new();

	Redis_Connection conn(base);
	conn.asynConnect("192.168.106.60", 6379);

	while(true)
	{
		#ifdef _WIN32
		Sleep(1);
		#else
		sleep(1);
		#endif
		event_base_loop(base, EVLOOP_NONBLOCK);
	}
}