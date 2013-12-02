package com.renren.thriftex.protocol;

import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TType;

/**
 * Xoa protocol implementation .
 *
 */
public class XoaParseProtocol  {
  private String fileName_ = "BinaryFile";
  private DataInputStream in;
  
  private BufferedWriter bw;
  
  /**
   * Constructor
   */
  public XoaParseProtocol(String fileName) {
    fileName_ = fileName;
    try {
        in = new DataInputStream(new FileInputStream(fileName_)); //读二进制文件
    } catch(IOException e) {
        System.out.println("Error Binary File");
    }
  }

  /**
   * Reading methods.
   */
 private void parseFiled(byte type) throws IOException {
	 switch (type) {
	 case TType.BOOL:
		 boolean boolArg = in.readBoolean();
		 bw.write(" bool ");
		 bw.write(String.valueOf(boolArg));
		 break;
		  	    
	 case TType.BYTE:
		 byte byteArg = in.readByte();
		 bw.write(" byte ");
		 bw.write(String.valueOf(byteArg));
		 break;
			    	    
	 case TType.DOUBLE:
		 double doubleArg = in.readDouble();
		 bw.write(" double ");
		 bw.write(String.valueOf(doubleArg));	
		 break;
		    
	 case TType.I16:
		short shortArg = in.readShort();
		bw.write(" short ");
		byte[] buf = new byte[2];  
		buf[0] = (byte) ((shortArg >> 8) & 0xff);  
		buf[1] = (byte) (shortArg & 0xff);  
		bw.write(buf.toString()); 
		break;
			    
		    
	 case TType.I32:
		 int intArg = in.readInt();
		 bw.write(" int ");
		 bw.write(String.valueOf(intArg));	
		 break;
	
		
	 case TType.I64:
		 long longArg = in.readLong();
		 bw.write(" long ");
		 bw.write(String.valueOf(longArg));	
		 break;
	
		  
	 case TType.LIST:
		 byte listElemType = in.readByte();
		 int listSize = in.readInt();
		 bw.write(" list: ");
		 for(int i = 0; i < listSize; i++) {
			 parseFiled(listElemType); 
		 }
		 break;
			    
		    
	 case TType.MAP:
		 byte keyType = in.readByte();
		 byte valueType = in.readByte();
		 int mapSize = in.readInt();
		 bw.write(" map: ");
		 for(int i = 0; i < mapSize; i++) {
			 bw.write("< ");
			 parseFiled(keyType);
			 parseFiled(valueType);
			 bw.write(" > "); 
		 }
		 break;
			    
		  
	 case TType.SET:
		 byte setElemType = in.readByte();
		 int setSize = in.readInt();
		 bw.write(" set: ");
		 for(int i = 0; i < setSize; i++) {
			 parseFiled(setElemType); 
		 }
		 break;
			    
		   
	 case TType.STRING:
		 String  strName= parseString();
		 bw.write(" String: ");
		 bw.write(strName);
		 break;
			    
		 
	 case TType.STRUCT:
		 while(true) {
			 byte messageType=in.readByte();
			 if(messageType == TType.STOP) {
				 break;
			 }
			 in.skipBytes(2);
			 parseFiled(messageType);
		 }
		 break;
		    
	 default:
		 break;
	 } 
 }
 
 private void skipMessageBegin() throws TException {  
	 try { 
		 in.skipBytes(1);
		 in.skipBytes(4);
	 }  catch (IOException e) {
		 throw new TException(" skip Bytes Error");  
	 }
 }
 
 private String changeTimeFormat(long time) {
	 Date nowTime = new Date(time);
	 SimpleDateFormat sdFormatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SS");
	 String retStrFormatNowDate = sdFormatter.format(nowTime);
	 return retStrFormatNowDate;
 }
 
 private void parseNowTime() throws TException {
	 try {
		 long now = in.readLong();
		 String nowTime = changeTimeFormat(now);
		 bw.write("   now time:");
		 bw.write(nowTime);
	 }  catch (IOException e) {
		 throw new TException("Parse Now Time Error");
	 }
 }
 
 private void parseResult() throws TException {
	 try {
		 boolean result = in.readBoolean();
		 bw.write("   result:");    
		 bw.write(String.valueOf(result));
	 }  
	 catch (IOException e) {
		 throw new TException("Parse Result Error");
	 }
 }
 
 private void parseIp() throws IOException {
	 String  strName = parseString();
	 bw.write("    ip and port:");	 
	 bw.write(strName);
 }
 
 private void initBufferedWriter(String path) throws TException {
	 try {
		 bw=new BufferedWriter(new FileWriter(path, true));
	 } catch(IOException e) {
		 throw new TException("Init Buffered Write Error");
	 }
 }
 
 private String parseString() throws IOException {
	 String methodString;
	 int methodNameLength = in.readInt();
	 byte[] methodName = new byte[methodNameLength];
	 for(int i = 0; i < methodNameLength; i++) {
		 methodName[i] = in.readByte();
	 }	  
	 methodString = new String(methodName);	
	 return methodString;
 }
 
 private void closeBufferedWriter() throws TException{
	 try {
		 bw.close();
	 } catch(IOException e) {
		 throw new TException("Buffered Close Error");
	 }
 }
 
 private void closeDataInputStream() throws TException {
	 try {
		 in.close();
	 } catch(IOException e) {
		 throw new TException("DataInputStream Close Error");
     }
 }
 
 public boolean parseMessage(String path) throws TException {
	 initBufferedWriter(path);
	 while(true) {
		 try {
			 String methodString = parseString();
			 bw.write("method:");
			 bw.write(methodString);
			 bw.write("  args:( ");
			 
			 skipMessageBegin();
			 parseFiled(TType.STRUCT);
			 bw.write("  ) ");
			 
			 parseResult();
			 parseNowTime();
			 parseIp();
			 bw.flush();
			 bw.newLine();
		 } catch (EOFException e) {
		     closeBufferedWriter();
             closeDataInputStream();
			 break;
		 } catch(IOException e) {
			 throw new TException("Read Int Error");	
		 } 
	 }	
	 return true;
 }

}
