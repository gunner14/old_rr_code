/**************************************************************************************
 *  Function: Providing basic utility tools
 *  Developer: junwu.xiong@renren-inc.com
 *  Date: 2012-11-29  
 *************************************************************************************/

package com.renren.socialgraph.recommendfriend.fofrank;

import org.apache.hadoop.hbase.util.MD5Hash;

public class Tools {	

	
	/** 
	  * 将高字节数组转换为int 
	  * @param b byte[] 
	  * @return int 
	  */  
	public static int hBytesToInt(byte[] b) {  
	  int s = 0;  
	  for (int i = 0; i < 3; i++) {  
	    if (b[i] >= 0) {  
	    s = s + b[i];  
	    } else {  
	    s = s + 256 + b[i];  
	    }  
	    s = s * 256;  
	  }  
	  if (b[3] >= 0) {  
	    s = s + b[3];  
	  } else {  
	    s = s + 256 + b[3];  
	  }  
	  return s;  
	}   
	  
	/** 
	  * 将低字节数组转换为int 
	  * @param b byte[] 
	  * @return int 
	  */  
	public static int lBytesToInt(byte[] b) {  
	  int s = 0;  
	  for (int i = 0; i < 3; i++) {  
	    if (b[3-i] >= 0) {  
	    s = s + b[3-i];  
	    } else {  
	    s = s + 256 + b[3-i];  
	    }  
	    s = s * 256;  
	  }  
	  if (b[0] >= 0) {  
	    s = s + b[0];  
	  } else {  
	    s = s + 256 + b[0];  
	  }  
	  return s;  
	}   
	
	/** 
	  * 高字节数组转换为float 
	  * @param b byte[] 
	  * @return float 
	  */  
	@SuppressWarnings("static-access")
	public static float hBytesToFloat(byte[] b) {  
	  int i = 0;  
	  Float F = new Float(0.0);  
	  i = ((((b[0]&0xff)<<8 | (b[1]&0xff))<<8) | (b[2]&0xff))<<8 | (b[3]&0xff);  
	  return F.intBitsToFloat(i);  
	}   
	  
	/** 
	  * 低字节数组转换为float 
	  * @param b byte[] 
	  * @return float 
	  */  
	@SuppressWarnings("static-access")
	public static float lBytesToFloat(byte[] b) {  
	  int i = 0;  
	  Float F = new Float(0.0);  
	  i = ((((b[3]&0xff)<<8 | (b[2]&0xff))<<8) | (b[1]&0xff))<<8 | (b[0]&0xff);  
	  return F.intBitsToFloat(i);  
	}   

	public static byte[] ReversEndian(byte[] str, int len, boolean big) {
		byte b;
		byte res[] = new byte[len];
		for (int i = 0; i < len; i++) {
			res[i] = str[i];
		}
		if (big == false) {
			for (int i = 0; i < len; i++) {
				b = str[i];
				res[len - i - 1] = b;
			}
		}
		return res;
	}

	public static String GetHashedKey(String key) {
		String hash = MD5Hash.getMD5AsHex(key.getBytes());
		String rehash = MD5Hash.getMD5AsHex((hash + key).getBytes());
		return rehash.substring(0, 10).toUpperCase() + key;
	}

	public String GetReversedKey(String key) {
		StringBuffer sb = new StringBuffer(key);
		return sb.reverse().toString();
	}

	public static void main(String[] args) {
		String a = "256404211";
		System.out.println(GetHashedKey(a));
	}
}
