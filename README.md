# async-redis-cli
一个异步的redis客户端。基于微软开源的redis-cli做了修改，删除了其中的socket部分，代码在redis-cli文件夹下.  
通过libevent建立一个tcp session连接redis服务器，redid-cli中的接收和发送回调就是真正在这里实现的，  代码主要在libevent-session中.   
但是现在封装了redis的部分操作，没有封装的部分可以设置回调来处理。  
windows下直接用VS2010 打开就行了；mac/linux下运行scons编译。  
Enjoy it！欢迎提bug，哈哈
