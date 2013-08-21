package com.dodoyo.invite.httpservice.auth;

//import sun.misc.BASE64Decoder;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @author Chris Gao
 */
public class EncryptUtil {
    private static EncryptUtil encryptUtil = new EncryptUtil();

    public static EncryptUtil getInstance() {
        return encryptUtil;
    }

    public String md5Str(String str) {
        if (str == null || str.length() == 0) {
            throw new IllegalArgumentException("String to encript cannot be null or zero length");
        }

        StringBuffer hexString = new StringBuffer("");

        try {

            MessageDigest md = MessageDigest.getInstance("MD5");

            md.update(str.getBytes());

            byte[] hash = md.digest();

            for (int i = 0; i < hash.length; i++) {
                if ((0xff & hash[i]) < 0x10) {
                    hexString.append("0" + Integer.toHexString((0xFF & hash[i])));
                } else {
                    hexString.append(Integer.toHexString(0xFF & hash[i]));
                }
            }
        }
        catch (NoSuchAlgorithmException e) {
            e.printStackTrace(System.err);
        }

        return hexString.toString();
    }

    public String md5Bytes(byte[] bytes) {
        if (bytes == null || bytes.length == 0) {
            throw new IllegalArgumentException("bytes to encript cannot be null or zero length");
        }

        StringBuffer hexString = new StringBuffer("");

        try {
            MessageDigest md = MessageDigest.getInstance("MD5");

            md.update(bytes);

            byte[] hash = md.digest();

            for (int i = 0; i < hash.length; i++) {
                if ((0xff & hash[i]) < 0x10) {
                    hexString.append("0" + Integer.toHexString((0xFF & hash[i])));
                } else {
                    hexString.append(Integer.toHexString(0xFF & hash[i]));
                }
            }
        }
        catch (NoSuchAlgorithmException e) {
            e.printStackTrace(System.err);
        }

        return hexString.toString();
    }

//    public String getBase64String(String s) {
//        if (s == null)
//            return null;
//        try {
//            String ret = (new sun.misc.BASE64Encoder()).encode(s.getBytes("UTF-8"));
//            Pattern p = Pattern.compile("\\s*|\t|\r|\n");
//            Matcher m = p.matcher(ret);
//            String after = m.replaceAll("");
//            return after;
//        }
//        catch (Exception err) {
//            err.printStackTrace();
//            return null;
//        }
//    }
//
//    public String getBase64StringBySysEnc(String s) {
//        if (s == null)
//            return null;
//        try {
//            String ret = (new sun.misc.BASE64Encoder()).encode(s.getBytes());
//            Pattern p = Pattern.compile("\\s*|\t|\r|\n");
//            Matcher m = p.matcher(ret);
//            String after = m.replaceAll("");
//            return after;
//        }
//        catch (Exception err) {
//            err.printStackTrace();
//            return null;
//        }
//    }
//
//    public String decodeBase64(String s) {
//        s = s.replaceAll(" ", "");
//        if (s == null)
//            return null;
//        BASE64Decoder decoder = new BASE64Decoder();
//        try {
//            byte[] b = decoder.decodeBuffer(s);
//            String ret = new String(b, "UTF-8");
//            return ret;
//        }
//        catch (Exception e) {
//            return null;
//        }
//    }
//
//    public String decodeBase64ForSysEnc(String s) {
//        if (s == null)
//            return null;
//        BASE64Decoder decoder = new BASE64Decoder();
//        try {
//            byte[] b = decoder.decodeBuffer(s);
//            String ret = new String(b);
//            return ret;
//        }
//        catch (Exception e) {
//            return null;
//        }
//    }

    public static void main(String args[]) {
//        System.out.print(EncryptUtil.getInstance().decodeBase64("5rWL6K+V"));
    }
}