#ifndef REDIS_CONNECTIONH
#define REDIS_CONNECTIONH

#include "libevent-connection.h"
#include "async.h"

class Redis_Connection : public libeventClient
{
public:
	Redis_Connection(event_base* base);
	int set(int key, int value, redisCallbackFn fun);
	int get(int key, redisCallbackFn fun);
	redisAsyncContextWrapper& get_redisAsyncContextWrapper();
	virtual void onRead(char* pdata, size_t length);
	virtual void onClose();
	virtual void onConnected();

	void redisCallBack(redisAsyncContext* predisAsyncContext, redisReply* predisReply, void* pData);
private:
	redisAsyncContextWrapper redis_handler;
};

#endif