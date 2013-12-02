package com.renren.thriftex.protocol;

import java.io.EOFException;

import org.apache.thrift.TException;
import org.junit.Test;


/**
 * 把二进制文件当成转换成二进制流，再转换成可读的文件并保存
 * @author zhe.long  2012.9.1
 *
 */

public class TestBinaryToStringXoaProtocol {
    
    @Test
    public void TestBinaryToStringXoaProtocol() {
        String binaryFile = "src/test/com/renren/thriftex/protocol/BinaryFile";
        
        XoaParseProtocol parseProtocol = new XoaParseProtocol(binaryFile);
        try {
            parseProtocol.parseMessage("src/test/com/renren/thriftex/protocol/StringFile");
            System.out.println("Parse String File OK");
        } catch (TException e) {
            System.out.println("Parse String File Error");
        }
    }
}
