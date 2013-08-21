package com.renren.xoa.commons.binaryparam;


public interface Encoder {

    public byte[] encode(Object value);
 
    public Object decode(byte[] data);
    
}
