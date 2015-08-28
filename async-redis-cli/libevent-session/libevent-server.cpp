#include "libevent-server.h"

libeventServer::libeventServer(event_base* pbase)
	:base(pbase)
	,listener(NULL)
	,signal_event(NULL)
	, listen_port(0)
{
}

void libeventServer::update() {
	if(base != NULL) {
		event_base_loop(base, EVLOOP_NONBLOCK);
		//printf("begin server on port:%d\n", listen_port);
		//event_base_dispatch(base);
	}
}

void libeventServer::addConnection(evutil_socket_t fd, libeventClient* connection) {
	connections.insert(std::pair<evutil_socket_t, libeventClient*>(fd, connection));
}

void libeventServer::removeConnection(evutil_socket_t fd) {
	connections.erase(fd);
}

void libeventServer::sendToAllClients(const char* data, size_t len) {
	map<evutil_socket_t, libeventClient*>::iterator it = connections.begin();
	while(it != connections.end()) {
		it->second->send(data, len);
		++it;
	}
}

// ------------------------------------

void libeventServer::listenerCallback(
struct evconnlistener* listener
	,evutil_socket_t fd
	,struct sockaddr* saddr
	,int socklen
	,void* data
	)
{
	libeventServer* server = static_cast<libeventServer*>(data);
	struct event_base* base = (struct event_base*) server->base;
	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if(!bev) {
		event_base_loopbreak(base);
		printf("Error constructing bufferevent!\n");
		return;
	}

	libeventClient* conn = new libeventClient(fd, bev, server);
	server->addConnection(fd, conn);

	printf("get client fd:%d\n", fd); 
}


libeventServer::~libeventServer() {
	if(signal_event != NULL) {
		event_free(signal_event);
	}

	if(listener != NULL) {
		evconnlistener_free(listener);
	}
}

bool libeventServer::setup(const unsigned short& port) {
	if(!base) {
		printf("Server: cannot create base.\n");
		return false;
	}

	listen_port = port;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	listener = evconnlistener_new_bind(
		base
		,libeventServer::listenerCallback
		,(void*)this
		,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE
		,-1
		,(struct sockaddr*)&sin
		,sizeof(sin)
		);

	if(!listener) {
		printf("Cannot create listener.\n");
		return false;
	}
	//signal_event = evsignal_new(base, SIGINT, signalCallback, (void*)this);
	//if(!signal_event || event_add(signal_event, NULL) < 0) {
	//	
	//	printf("Cannog create signal event.\n");
	//	return false;
	//}
	return true;
}

event_base* libeventServer::get_ioloop()
{
	return base;
}
