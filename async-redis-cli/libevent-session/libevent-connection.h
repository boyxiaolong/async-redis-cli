#ifndef LIBEVENT_CONNECTIONH
#define LIBEVENT_CONNECTIONH

extern "C" {
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
} 
#include <string>

class libeventServer;

class libeventClient {
public:
	libeventClient(event_base* ioloop);

	int asynConnect(std::string ip, int port);
	bool send(const char* data, size_t numBytes);
	virtual void onRead(char* pdata, size_t length);
	virtual void onClose(int status);
	virtual void onConnected();

	static void writeCallback(struct bufferevent*, void* connection);
	static void readCallback(struct bufferevent*, void* connection);
	static void eventCallback(struct bufferevent*, short, void* connection);

private:
	libeventClient();
	void init_socket();
	void set_callback();

	struct bufferevent* bev;
	evutil_socket_t fd;
};

#endif