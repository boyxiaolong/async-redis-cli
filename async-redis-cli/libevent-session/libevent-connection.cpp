#include "libevent-connection.h"
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
libeventClient::libeventClient(event_base* ioloop)
{
	bev = bufferevent_socket_new(ioloop, -1, BEV_OPT_CLOSE_ON_FREE);

	init_socket();
}

bool libeventClient::send(const char* data, size_t numBytes) {
	if(bufferevent_write(bev, data, numBytes) == -1) {
		printf("Error while sending in Connection::send()\n");
	}
	return true;
}

void libeventClient::onRead(char* pdata, size_t length)
{
	if (!pdata || length < 1)
	{
		printf("onRead error\n");
		return;
	}
}

int libeventClient::asynConnect(std::string ip, int port)
{
	sockaddr_in sockaddr_;
	memset(&sockaddr_, 0, sizeof(sockaddr_));
	sockaddr_.sin_family = AF_INET;  
	sockaddr_.sin_port = htons(port);  
#ifdef _WIN32
	sockaddr_.sin_addr.S_un.S_addr = inet_addr(ip.c_str());   
#else
	inet_pton(AF_INET, ip.c_str(), &sockaddr_.sin_addr);
#endif

	if (bufferevent_socket_connect(bev,
		(struct sockaddr *)&sockaddr_, sizeof(sockaddr_)) < 0) {
			/* Error starting connection */
			bufferevent_free(bev);
			printf("error connect");
			return -1;
	}

	return 0;
}

void libeventClient::init_socket()
{
	set_callback();
}

void libeventClient::writeCallback(struct bufferevent* bev, void* connection)
{
	struct evbuffer* output = bufferevent_get_output(bev);
	if(evbuffer_get_length(output) == 0) {

	}
	printf("write callback.\n");
}

void libeventClient::readCallback(struct bufferevent* bev, void* connection)
{
	libeventClient* conn = static_cast<libeventClient*>(connection);
	struct evbuffer* buf = bufferevent_get_input(bev);
	char readbuf[1024];
	size_t read = 0;

	while( (read = evbuffer_remove(buf, &readbuf, sizeof(readbuf))) > 0) {
		conn->onRead(readbuf, read);
	}
}

static std::string send_data("set 1 1");

void libeventClient::eventCallback(struct bufferevent* bev, short events, void* connection)
{
	libeventClient* conn = (libeventClient*)connection;
	if (!conn)
	{
		printf("error\n");
		return;
	}
	if(events & BEV_EVENT_EOF) {
		conn->onClose(events);
		printf("error");
	}
	else if(events & BEV_EVENT_ERROR) {
		conn->onClose(events);
		printf("Got an error on the connection");
	}
	else if(events & BEV_EVENT_CONNECTED) {
		printf("connected \n");
		conn->onConnected();
	}
	else {
		printf("unhandled.\n");
	}
}

void libeventClient::onClose(int status)
{

}

void libeventClient::onConnected()
{

}

void libeventClient::set_callback()
{
	bufferevent_setcb(bev, libeventClient::readCallback, libeventClient::writeCallback, libeventClient::eventCallback, (void*)this);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);
}
