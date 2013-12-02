package com.renren.thriftex.protocol;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TType;

/**
 * Xoa protocol implementation .
 *
 */
public class XoaParseByteProtocol  {
  
  private int count;
  
  /**
   * Constructor
   */
  public XoaParseByteProtocol() {
    count = 0;
  }

  /**
   * Reading methods.
   */
  
 private boolean toBoolean(byte[] buf) {
	 boolean b = (buf[count++] == 1) ? true: false;
	 return b;
 }
  
 private byte toByte(byte[] buf) {
	 byte b = buf[count++];
	 return b;
 }
 
 private short toShort(byte[] buf) {
	 short shortArg =0;
	 short s0 =(short)(buf[count+1]&0xff);// 最低位   
	 short s1 =(short)(buf[count+0]&0xff);   
	 s1 <<=8;
	 shortArg =(short)(s0 | s1);  
	 count += 2;
	 return shortArg;
 }
 
 private int toInt(byte[] buf) {
	 int intArg =0;   
	 int i0 = buf[count+3]&0xff;// 最低位   
	 int i1 = buf[count+2]&0xff;   
	 int i2 = buf[count+1]&0xff;   
	 int i3 = buf[count+0]&0xff;   
	 i3 <<=24;   
	 i2 <<=16;   
	 i1 <<=8;   
	 intArg = i0 | i1 | i2 | i3;   
     
	 count += 4;
	 return intArg;
 }
 
 private long toLong(byte[] buf) {
	 long longArg =0;   
	 long l0 = buf[count+7]&0xff;// 最低位   
	 long l1 = buf[count+6]&0xff;   
	 long l2 = buf[count+5]&0xff;   
	 long l3 = buf[count+4]&0xff;   
	 long l4 = buf[count+3]&0xff;  
	 long l5 = buf[count+2]&0xff;   
	 long l6 = buf[count+1]&0xff;   
	 long l7 = buf[count+0]&0xff;   

     // l0不变   
	 l1 <<=8;   
	 l2 <<=16;   
	 l3 <<=24;   
	 l4 <<=8*4;   
	 l5 <<=8*5;   
	 l6 <<=8*6;   
	 l7 <<=8*7;   
	 longArg = l0 | l1 | l2 | l3 | l4 | l5 | l6 | l7;   
     
	 count += 8;
	 return longArg;
 }
 
 private double toDouble(byte[] buf) {
	 long l = toLong(buf);
	 return Double.longBitsToDouble(l);
     
 }
 
 private void parseFiled(byte type, StringBuilder sb, byte[] buf) throws IOException {
	 switch (type) {
	 case TType.BOOL:
		 boolean boolArg = toBoolean(buf);
		 sb.append(" bool  ");
		 sb.append(String.valueOf(boolArg));
		 break;
		  	   
	 case TType.BYTE:
		 byte byteArg = toByte(buf);
		 sb.append(" byte ");
		 sb.append(String.valueOf(byteArg));
		 break;
			    
	 case TType.DOUBLE:
		 double doubleArg;
		 doubleArg = toDouble(buf);
		 String stringDouble = String.valueOf(doubleArg);
		 sb.append(" double ");	
		 sb.append(stringDouble);	    
		 break;

	 case TType.I16:
		 short shortArg = toShort(buf);
		 sb.append(" short ");		 
		 sb.append(String.valueOf(shortArg));		
		 break;		
		 		 
	 case TType.I32:
		 int intArg =toInt(buf);   
		 sb.append(" int ");
		 sb.append(String.valueOf(intArg));
		 break;
		 
	 case TType.I64:
		 long longArg = toLong(buf);
		 sb.append(" long ");
		 sb.append(String.valueOf(longArg));
		 break;
		 
	 case TType.LIST:
		 byte listElemType = toByte(buf);
		 int listSize = toInt(buf);
		 sb.append(" list: ");
		 for(int i = 0; i < listSize; i++) {
			 parseFiled(listElemType, sb, buf); 
		 }
	
		 break;
			 
	 case TType.MAP:
		 byte keyType = toByte(buf);
		 byte valueType = toByte(buf);
		 int mapSize = toInt(buf);
		 sb.append(" map: ");
		 for(int i = 0; i < mapSize; i++) {
			 sb.append("< ");
			 parseFiled(keyType, sb, buf);
			 parseFiled(valueType, sb, buf);
			 sb.append(" > ");
		 }
		 break;
		
	 case TType.SET:
		 byte setElemType = toByte(buf);
		 int setSize = toInt(buf);	 
		 sb.append(" set: ");	 
		 for(int i = 0; i < setSize; i++) {
			 parseFiled(setElemType, sb, buf);    
		 }
		 break;
 
	 case TType.STRING:
		 String  strName= parseString(buf);
		 sb.append(" String: ");
		 sb.append(strName);
		 break;
		
	 case TType.STRUCT:
		 while(true) {
			 byte messageType=toByte(buf);
			 if(messageType == TType.STOP) {
				 break;
			 }
			 skipCount(2);
			 parseFiled(messageType, sb, buf);
		 }
		 break;
		 
	 default:
		 break;
	 } 
 }
 
 private void skipCount(int n) {
	 count += n;
 }

 private void skipMessageBegin()  {
	 skipCount(1);
	 skipCount(4);
 }
 
 private String changeTimeFormat(long time) {
	 Date nowTime = new Date(time);
	 SimpleDateFormat sdFormatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SS");
	 String retStrFormatNowDate = sdFormatter.format(nowTime);
	 return retStrFormatNowDate;
 }
 
 private void parseNowTime(StringBuilder sb, byte[] buf) throws TException {
	 long now = toLong(buf);
	 String nowTime = changeTimeFormat(now);
	 sb.append("   now time:");
	 sb.append(nowTime);
 }
 
 private void parseResult(StringBuilder sb, byte[] buf) throws TException {
	 boolean result = toBoolean(buf);
	 sb.append("   result:");
	 sb.append(String.valueOf(result));
 }

 private void parseIp(StringBuilder sb, byte[] buf) throws IOException {
	 String  strName = parseString(buf);
	 sb.append("    ip and port:");
	 sb.append(strName);
 }
 
 private String parseString(byte[]  buf) throws IOException {
	 String methodString;
	 int methodNameLength = toInt(buf);
	 byte[] methodName = new byte[methodNameLength];
	 for(int i = 0; i < methodNameLength; i++) {
		 methodName[i] = buf[count + i];
	 }
	
	 count += methodNameLength;
	 methodString = new String(methodName);
	 return methodString;
 }
 
 public String parseMessage(byte[] parseByte, int offset) throws TException{
	 count = offset;
	 return parseMessage(parseByte);
 }

 /**
  * 该方法能够把所传入的byte数组解析成可读的String字符串并返回,
  * byte数组必须由thrift所定义的格式生成
  */
 
 public String parseMessage(byte[] parseByte) throws TException {
	 int len = parseByte.length;
	 StringBuilder sb = new StringBuilder();
	 while(count < len) {
		 try {
			 String methodString = parseString(parseByte);
			 sb.append("method:");
			 sb.append(methodString);
			 sb.append(" args:( ");
			 skipMessageBegin();

			 parseFiled(TType.STRUCT, sb, parseByte);
			 sb.append("  ) ");
			 parseResult(sb, parseByte);
			 parseNowTime(sb, parseByte);
			 parseIp(sb, parseByte);
			 sb.append(" \n");
		 } catch(IOException e) {
			 throw new TException("Read Int Error");	
		 }
	 }
	 return sb.toString();
 }
 
}
