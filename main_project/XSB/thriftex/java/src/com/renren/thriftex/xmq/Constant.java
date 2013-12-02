package com.renren.thriftex.xmq;

public class Constant {
    // Producer
    public final static byte PRODUCER_LOGIN_ACK  = 0x00;
    public final static byte PRODUCER_LOGOUT_ACK = 0x01;
    public final static byte GATEWAY_DATA_ACK    = 0x02;
    public final static byte GATEWAY_DATA_REJ    = 0x03;
    
    // GateWay Service
    public final static byte PRODUCER_LOGIN_CODE  = 0x00;
    public final static byte PRODUCER_LOGOUT_CODE = 0x01;
    public final static byte PRODUCER_SEND_DATA   = 0x02;

    // Writer service
    public final static byte CONSUMER_LOGIN_CODE  = 0x00;
    public final static byte CONSUMER_LOGOUT_CODE = 0x01;
    public final static byte CONSUMER_GET_DATA    = 0x02;
    public final static byte CONSUMER_GET_PUB     = 0x03;    

    // Consumer
    public final static byte CONSUMER_LOGIN_ACK      = 0x00;
    public final static byte CONSUMER_LOGOUT_ACK     = 0x01;    
    public final static byte CONSUMER_DATA           = 0x02;
    public final static byte CONSUMER_DATA_NOT_EXIST = 0x03;
    public final static byte CONSUMER_PUB            = 0x04;    

    // ERROR
    public final static byte INVALID_MESSAGE = (byte)0xff;
}
