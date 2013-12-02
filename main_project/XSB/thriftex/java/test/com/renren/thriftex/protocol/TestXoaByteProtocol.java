package com.renren.thriftex.protocol;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.apache.thrift.TException;
import org.junit.Test;


/**
 * 把二进制文件当成转换成二进制流，然后转换成可读的字符串
 * @author zhe.long
 *
 */
public class TestXoaByteProtocol {

	@Test
	public void TestXoaParseProtocol() {
		String binaryFile = "src/test/com/renren/thriftex/protocol/BinaryFile";
		DataInputStream dis = null;
		long filelen =  0;
		try {
			File file = new File(binaryFile);
			filelen = file.length();
		    dis=new DataInputStream(new FileInputStream(file));  
	    } catch(IOException e) {
	        System.out.println("Error Binary File");
	    }
	    ByteArrayOutputStream baos = new ByteArrayOutputStream((int)filelen);
	    byte[] bytebuf  = new byte[(int)filelen];
	    try {
		    while(dis.read(bytebuf) !=  -1 ) {
		    	baos.write(bytebuf);
		    }
		    dis.close();
	    } catch (IOException e) {
	    	System.out.println("Read Binary Error or Write Bytebuf Error!!!");
	    }
	    
	    XoaParseByteProtocol parseProtocol = new XoaParseByteProtocol();
	    String parseString  = new String();
	    try {
	    	parseString = parseProtocol.parseMessage(baos.toByteArray());
	    } catch (TException e) {
	    	System.out.println("Parse String Error!!!");
	    }
	    System.out.println(parseString);
	}
	
}
