package com.renren.thriftex.xmq;

//  ----------------------------------------
//              Message format
//  ----------------------------------------
// | opCode | SequenceID | DataLen | Data  |
// | 1 Byte |   8 Byte   | 2 Byte  | < 16M |

public class Message {
    public byte opCode;
    public long seqID;
    public byte[] data;

    public static final int Meta_Len = 11;
    
    public static byte[] packMessage(byte opCode, long seqID, 
            byte[] data, int len) {
        byte [] result = new byte[Meta_Len + len];

        int off = 0;
        // opCode
        result[off] = opCode;

        // seqID 
        result[off + 1] = (byte)(0xff & (seqID >> 56));
        result[off + 2] = (byte)(0xff & (seqID >> 48));
        result[off + 3] = (byte)(0xff & (seqID >> 40));
        result[off + 4] = (byte)(0xff & (seqID >> 32));
        result[off + 5] = (byte)(0xff & (seqID >> 24));
        result[off + 6] = (byte)(0xff & (seqID >> 16));
        result[off + 7] = (byte)(0xff & (seqID >> 8));
        result[off + 8] = (byte)(0xff & (seqID));
        
        // dataLen
        result[off + 9]  = (byte)(0xff & (len>> 8)); 
        result[off + 10] = (byte)(0xff & (len)); 
        
        // data
        off = 11;
        for (int i = 0; i < len; i++) {
            result[off++] = data[i];
        }
        
        return result;
    }
    
    public static Message unpackMessage(byte[] buf) {
        Message message = new Message();
        
        int off = 1;
        
        message.opCode = buf[0];

        message.seqID = ((long)(buf[off]   & 0xff) << 56) |
                ((long)(buf[off+1] & 0xff) << 48) |
                ((long)(buf[off+2] & 0xff) << 40) |
                ((long)(buf[off+3] & 0xff) << 32) |
                ((long)(buf[off+4] & 0xff) << 24) |
                ((long)(buf[off+5] & 0xff) << 16) |
                ((long)(buf[off+6] & 0xff) <<  8) |
                ((long)(buf[off+7] & 0xff));
        
        int dataLen = (short)(((buf[off+8] & 0xff) << 8) |
                ((buf[off+9] & 0xff))); 
        
        if (dataLen != buf.length - Meta_Len) {
            message.opCode = Constant.INVALID_MESSAGE;
            return message;
        }
        
        message.data = new byte[dataLen];
        for (int i = 0; i < dataLen; i++) {
            message.data[i] = buf[Meta_Len + i];
        }
        
        return message;
    }
    
    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer();
        
        sb.append("opcode: [").append(opCode);
        sb.append("] | seqID: [").append(seqID).append("]\n");
        
        return sb.toString();
    }
}