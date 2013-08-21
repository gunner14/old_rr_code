package com.xiaonei.reg.common;

import java.math.BigInteger;
import java.net.URLDecoder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 15, 2010 11:02:40 AM
 * 工具类
 */
public class RegTools {
    
    private static final RegLogger logger = RegLogger.getLoggerWithType(RegTools.class);
    // all the hash codes are not longer than 25.
    private static final int MD5_HASHLENGTH = 25;

    private static final String CHARSET = "UTF-8";

    private static final String stripEscapeRegex = "%(?=[^0-9a-fA-F])|%(?=[0-9a-fA-F][^0-9a-fA-F])|%$";

    private static final Pattern imageUrlPattern = Pattern.compile("<img[^>]*src=\"([^\"]*)",
            Pattern.CASE_INSENSITIVE);

    // compute the md5 hash code of the input string.
    public static String computeHash(String input) {
        String hash;
        BigInteger bi;
        if (input == null) {
        	input = "";
        	bi = null;
        }

        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            md.update(input.getBytes());
            // force to positive
            bi = new BigInteger(1, md.digest());

            // or break out statement
            hash = bi.toString(Character.MAX_RADIX);//36
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("Failed to compute Hash...");
        }

        if (hash == null) throw new RuntimeException("Failed to compute Hash...");

        if (hash.length() == MD5_HASHLENGTH)
        // this check first
        return hash;
        else if (hash.length() < MD5_HASHLENGTH)
        // because hashes map to a database column that always right-pads
        // the value, we do it here to maintain consistency
        return rightPad(hash, MD5_HASHLENGTH, ' ');
        else
        // Should this ever happen? If not, should we be throwing an
        // Error instead of trimming?
        return hash.substring(0, MD5_HASHLENGTH);
    }

    // Right pads a StringBuffer with the padChar so that it is the given length
    private static String rightPad(String s, int length, char padChar) {
        if (s.length() >= length) return s;

        StringBuffer buf = new StringBuffer(length);
        buf.append(s);
        buf.ensureCapacity(length);

        while (buf.length() < length)
            buf.append(padChar);

        return buf.toString();
    }

    /**
     * get the host info of a URL, but base on the URL string.
     * 
     * @param curi
     * @return
     */
    public static String getHost(String url) {
        String host = null;
        String urlInfo = null;

        // strip "http://"
        int indexHttp = url.indexOf("//");
        if (indexHttp != -1) urlInfo = url.substring(indexHttp + 2);
        else urlInfo = url;

        int index = urlInfo.indexOf("/");
        if (index == -1) urlInfo += "/";

        String urlHostSeg = urlInfo.substring(0, urlInfo.indexOf("/"));
        // process the first URL segment, get userInfo, port and host.
        int indexUserInfo = urlHostSeg.indexOf("@");
        if (indexUserInfo != -1) host = urlHostSeg.substring(indexUserInfo + 1);
        else host = urlHostSeg;

        return host;
    }

    /**
     * Returns a substring of the input String such that it does not exceed
     * maxUTF8_Bytes when encoded as UTF-8.
     */
    public static String truncateToMaximumUTF8Bytes(String input, int maxUTF8_Bytes) {
        if (input == null) return null;

        try {
            byte[] byteArray = input.getBytes("UTF-8");
            if (byteArray.length <= maxUTF8_Bytes) return input;

            int limit = maxUTF8_Bytes;
            while (((byteArray[limit] >> 6) & 0x03) == 2)
                limit--;

            return new String(byteArray, 0, limit, "UTF-8");
        } catch (Exception ignore) {
            if (input.length() <= maxUTF8_Bytes) return input;
            else return input.substring(0, maxUTF8_Bytes - 1);
        }
    }

    /**
     * URLDecoder will decode "+" to "%20" which means whitespace, avoid
     * mis-processes like it. And also treat "%XX" as special symbols, some
     * strings like "%2i" and "%ki" will be decoded either, which will
     * course Exception. And also replace the "%" and "%X" in the end be
     * "%25", which will course Exception either.
     * 
     * @param url
     * @return the decode URL with lowercase
     */
    public static String decode(String url) {
        if (url == null) return null;
        if (url.endsWith("/")) url = url.substring(0, url.length() - 1);

        String decodeUrl = url.replaceAll("\\+", "%2B");
        decodeUrl = decodeUrl.replaceAll(stripEscapeRegex, "%25");
        if (decodeUrl.matches(".*%.$")) {
            int index = decodeUrl.lastIndexOf("%");
            String last = "%25" + decodeUrl.substring(index + 1, decodeUrl.length());
            decodeUrl = decodeUrl.substring(0, index) + last;
        }

        try {
            decodeUrl = URLDecoder.decode(decodeUrl, CHARSET);
            return decodeUrl.toLowerCase();
        } catch (Exception e) {
            logger.info("this url: " + url + " \ncan not be decoded successfully, give it up.");
            return url.toLowerCase();
        }
    }

    /**
     * 
     * @Title: getImageUrls
     * @Description: TODO
     * @param @param content
     * @param @return
     * @return List<String>
     * @throws
     */
    public static List<String> getImageUrls(String content) {
        List<String> urls = new ArrayList<String>();
        Matcher m = null;
        String url = null;
        m = imageUrlPattern.matcher(content);
        while (m.find()) {
            url = m.group(1);
            if ((url != null) && url.length() > 1) {
                if (url.trim().startsWith("http://s.xnimg.cn/imgpro")) {
                    continue;
                }
                if (url.trim().startsWith("http://s.xnimg.cn//imgpro")) {
                    continue;
                }
                urls.add(url);
            }
        }
        return urls;
    }
}
