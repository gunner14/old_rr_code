
package com.xiaonei.platform.core.utility;

import java.security.Provider;
import java.security.Security;
import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

/**
 * Algorithms :
 * MD5
 * Random
 * DES
 * 
 * 
 * @author <a href="yunlong.bai@opi-corp.com">yunlong.bai</a>
 */
public class MathUtils {

	public static Random random = new Random(System.currentTimeMillis());

	public static int randomInt(int max) {
		return random.nextInt(max);
	}

	public static long randomLong() {
		return random.nextLong();
	}
	
	public static double randomDouble() {
		return random.nextDouble();
	}

	public static String MD5(String str) {
		Provider sunJce = new com.sun.crypto.provider.SunJCE();
		Security.addProvider(sunJce);
		
		try {
			// Generate secret key for HMAC-MD5
			KeyGenerator kg = KeyGenerator.getInstance("HmacMD5");
			SecretKey sk = kg.generateKey();

			// Get instance of Mac object implementing HMAC-MD5, and 
			// initialize it with the above secret key
			Mac mac = Mac.getInstance("HmacMD5");
			mac.init(sk);
			byte[] result = mac.doFinal(str.getBytes());
			
			return dumpBytes(result);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	
	public static byte[] desEncrypt(String msg, String salt) {
		if (msg == null) msg = "";
		if (salt == null) {
			salt = "dudusalt";
		}
		byte[] keyBytes = new byte[8];
		int saltLen = salt.length();
		byte[] saltBytes = salt.getBytes();
		for (int i=0; i<8; i++) {
			keyBytes[i] = saltBytes[i % saltLen];
		}
		
		try {
			DESKeySpec keySpec = new DESKeySpec(keyBytes);
			SecretKey key = SecretKeyFactory.getInstance("DES").generateSecret(keySpec);
			Cipher desCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
			desCipher.init(Cipher.ENCRYPT_MODE, key);
			byte[] text = msg.getBytes();
			byte[] ciphertext = desCipher.doFinal(text);
			
			return ciphertext;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static String desDecrypt(byte[] msg, String salt) {
		if (msg == null) return null;
		if (salt == null) {
			salt = "dudusalt";
		}
		byte[] keyBytes = new byte[8];
		int saltLen = salt.length();
		byte[] saltBytes = salt.getBytes();
		for (int i=0; i<8; i++) {
			keyBytes[i] = saltBytes[i % saltLen];
		}
		
		try {
			DESKeySpec keySpec = new DESKeySpec(keyBytes);
			SecretKey key = SecretKeyFactory.getInstance("DES").generateSecret(keySpec);
			Cipher desCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
			desCipher.init(Cipher.DECRYPT_MODE, key);
			byte[] deciphertext = desCipher.doFinal(msg);
			
			return new String(deciphertext);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static String dumpBytes(byte[] bytes) {
		StringBuffer sb = new StringBuffer();
		for (int i=0; i<bytes.length; i++) {
			/*
			if (i%32 == 0 && i!=0) {
				sb.append("\n");
			}
			*/
			String s = Integer.toHexString(bytes[i]);
			if (s.length() < 2) {
				s = "0"+s;
			}
			if (s.length() > 2) {
				s = s.substring(s.length()-2);
			}
			sb.append(s);
		}
		return sb.toString();
	}
	
	public static byte[] parseBytes(String str) {
		int len = str.length()/2;
		if (len <= 2) {
			return new byte[]{Byte.parseByte(str)};
		}
		byte[] arr = new byte[len];
		for (int i=0; i<arr.length; i++) {
			arr[i] = (byte) Integer.parseInt( str.substring(i*2, i*2 + 2), 16 );
		}
		return arr;
	}
	
	public static String encryptAsString(String encrypt_value,String encrypt_key){
		return dumpBytes(desEncrypt(encrypt_value,encrypt_key)) ;
	}
	
	public static String desEncryptAsString(String encrypt_value,String encrypt_key){
	    return desDecrypt(parseBytes(encrypt_value),encrypt_key) ;	
	}
	public static void main(String[] args) 
	{
//		int id = 200000074 ;
//		long time = System.currentTimeMillis() ;
//		String en = id+","+time ;
//		String enstr = MathUtils.encryptAsString(en,"%*friends");
//		System.out.println(enstr) ;
//		String descstr = MathUtils.desEncryptAsString(enstr,"%*friends");
//		String des[] = descstr.split(",") ;
//		System.out.println(des[0]) ;
		
		System.out.println(MathUtils.MD5("5693890577585211")) ;
		
		
//		String enstr = MathUtils.encryptAsString("12345678","1");
//		System.out.println(enstr);
	}
	
}