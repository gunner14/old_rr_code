package com.xiaonei.xce.userpassport;
import java.security.MessageDigest;

public class MD5Util {

    public static String MD5(String inStr) {
	MessageDigest md5 = null;
	try {
	    md5 = MessageDigest.getInstance("MD5");
	} catch (Exception e) {
	    System.out.println(e.toString());
	    e.printStackTrace();
	    return "";
	}
	char[] charArray = inStr.toCharArray();
	byte[] byteArray = new byte[charArray.length];

	for (int i = 0; i < charArray.length; i++)
	    byteArray[i] = (byte) charArray[i];

	byte[] md5Bytes = md5.digest(byteArray);

	StringBuffer hexValue = new StringBuffer();

	for (int i = 0; i < md5Bytes.length; i++) {
	    int val = ((int) md5Bytes[i]) & 0xff;
	    if (val < 16)
		hexValue.append("0");
	    hexValue.append(Integer.toHexString(val));
	}

	return hexValue.toString();
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
	String target = "31db981a41300b2a4559958a504ae37a";
	System.out.println(MD5("dzf73716@163.com").equals(target));
    }

}
