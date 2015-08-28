#include "Redis_Connection.h"

#define RedisSCallBack(realCallBack, holder) std::bind(realCallBack, holder, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

Redis_Connection::Redis_Connection(event_base* base):libeventClient(base)
{

}

int Redis_Connection::set(int key, int value, redisCallbackFn fun)
{
	return redis_handler.redisAsyncCommand(fun, nullptr, "set %d %d", key, value);
}

void Redis_Connection::onRead(char* pdata, size_t length)
{
	int res = redis_handler.redisAsyncReaderFeed(pdata, length);
	if (res != REDIS_OK){
		return;
	}

	redis_handler.redisProcessCallbacks();
}

void Redis_Connection::onConnected()
{
	redis_handler.SetConnected(true);
	redis_handler.Init(std::bind(&libeventClient::send, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&libeventClient::onClose, this, std::placeholders::_1));

	int res = set(10, 1, RedisSCallBack(&Redis_Connection::redisCallBack, this));
	if (res < 0)
	{
		printf("set error\n");
	}
	res = get(10, RedisSCallBack(&Redis_Connection::redisCallBack, this));
	if (res < 0)
	{
		printf("get error\n");
	}
}

void Redis_Connection::redisCallBack(redisAsyncContext* predisAsyncContext, redisReply* predisReply, void* pData)
{
	if (!predisAsyncContext || !predisReply)
	{
		printf("redisCallback null reply\n");
		return;
	}
	switch (predisReply->type)
	{
	case REDIS_REPLY_STRING:
		{
			std::string res(predisReply->str, predisReply->len);
			printf("get %s\n", res.c_str());
		}
		break;
	case REDIS_REPLY_ARRAY:
		break;
	case REDIS_REPLY_INTEGER:
		printf("get %lld\n", predisReply->integer);
		break;
	case REDIS_REPLY_NIL:
		break;
	case REDIS_REPLY_STATUS:
		{
			std::string res(predisReply->str, predisReply->len);
			printf("get %s\n", res.c_str());
		}
		break;
	case REDIS_REPLY_ERROR:
		break;
	}

	printf("get redis reply\n");
}

int Redis_Connection::get(int key, redisCallbackFn fun)
{
	return redis_handler.redisAsyncCommand(fun, nullptr, "get %d", key);
}

redisAsyncContextWrapper& Redis_Connection::get_redisAsyncContextWrapper()
{
	return redis_handler;
}

