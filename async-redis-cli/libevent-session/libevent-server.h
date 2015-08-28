#ifndef LIBEVENT_SERVERH
#define LIBEVENT_SERVERH

extern "C" {
	#include <event2/bufferevent.h>
	#include <event2/buffer.h>
	#include <event2/listener.h>
	#include <event2/util.h>
	#include <event2/event.h>
}

#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include "libevent-connection.h"

#include <map>
using std::map;

class libeventClient;

class libeventServer {
public:

	libeventServer(event_base* pbase);
	~libeventServer();
	
	bool setup(const unsigned short& port) ;
	void update();
	void sendToAllClients(const char* data, size_t len);
	void addConnection(evutil_socket_t fd, libeventClient* connection);
	void removeConnection(evutil_socket_t fd);
		
	static void listenerCallback(
		 struct evconnlistener* listener
		,evutil_socket_t socket
		,struct sockaddr* saddr
		,int socklen
		,void* server
	);
	
	event_base* get_ioloop();

	struct sockaddr_in sin;
	struct event_base* base;
	struct event* signal_event;
	struct evconnlistener* listener;
private:
	map<evutil_socket_t, libeventClient*> connections;
	int listen_port;
};




#endif