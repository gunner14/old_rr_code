namespace java com.renren.xmq

service XmqDemo {
        oneway void incr(1: i32 amount);
        oneway void sendData(1: binary data)
        i32 get();
}
