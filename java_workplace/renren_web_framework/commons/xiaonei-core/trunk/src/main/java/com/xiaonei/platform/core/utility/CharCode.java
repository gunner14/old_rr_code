package com.xiaonei.platform.core.utility;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

/**
 *  * Process mail related things
 *   * 
 *    * Created on 2004-2-18
 *     *
 *      * @author Freebat Wangh<wanghuiwen@tsinghua.org.cn>
 *       */
public class CharCode {
	private static int defaultCode = 20411;
	private static int beginCode = 12295;
//	private static int iBeginFrom = 20000;
	public static int endCode = 64045;
	private static int length=51751; 
	private static int[] data = new int[length];
	private static String buf;
	/** the following Chinese is encoded in unicode, so use gedit to look at it */
	static {
	    try {
	        for (int i = 0; i < length; i++) {
	            data[i] = defaultCode;
	        }
	        //add resource of final.txt under META-INF,optional chosen by client.
	        BufferedReader br = new BufferedReader(new InputStreamReader(CharCode.class.getClassLoader().getResourceAsStream("final.txt")));
//	        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("final.txt"), "UTF-8"));
	        do {
	            buf = br.readLine();
	            if (buf != null) {
	                data[Integer.parseInt(buf.substring(0, buf.indexOf("\t")))] = Integer.parseInt(buf.substring(buf.indexOf("\t")+1));	
	            }
	        } while (buf != null);
	        br.close();
	    } catch (FileNotFoundException e) {
            e.printStackTrace(System.err);
	    } catch (IOException e) {
            e.printStackTrace(System.err);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
	}

	public static long getPosition(String kw) throws UnsupportedEncodingException {
		if (kw == null || kw.length() == 0) {
			return 0;
		}
		if (kw.length() == 1) {
			return (long) CharCode.getPosition(CharCode.getCharCode(kw.charAt(0))) * (long)65536;
		}
		if (kw.length() > 1) {
			return (long)CharCode.getPosition(CharCode.getCharCode(kw.charAt(0))) * (long)65536 + (long)CharCode.getPosition(CharCode.getCharCode(kw.charAt(1)));
		}

		return 0;
	}

	public static int getCharCode(char c) throws UnsupportedEncodingException {
		/** 这种实现办法经初步测试没问题 */
		//byte[] code = new String(new char[]{c}).getBytes("UNICODE");
		byte[] code = new String(new char[]{c}).getBytes("UTF-16");	//校内、开心有差异，根据老洪的意见，统一采用UTF-16
		if (code.length == 4) {
			int a = 0, b = 0;
			if (code[2] < 0) {
				a = code[2] + 256;
			} else {
				a = code[2];
			}
			if (code[3] < 0) {
				b = code[3] + 256;
			} else {
				b = code[3];
			}
			return a + b * 256;
		}
		return endCode + 1;
	}

	public static int getPosition(int cc) {
		if (cc >= beginCode && cc <= endCode) {
			return data[cc-beginCode];
		} else if (cc < beginCode){
			return cc + endCode - beginCode+ 1;
		} else {
		    return cc;
		}
	}
	
	public static boolean isQualifiedName(String st) {
		int count = 0;
		for (int i = 0; i < st.length(); i++) {
			char c = st.charAt(i);
			int n;
			try {
				n = CharCode.getCharCode(c);
			} catch (UnsupportedEncodingException e) {
				return false;
			}
			if (n >= beginCode && c <= endCode) {
				count++;
				if (count > 4) {
					return false;
				}
			} else {
				return false;
			}
		}
		if (count == 2 || count == 3 || count == 4) {
			return true;
		} else {
			return false;
		}
	}
		
    public static boolean isLengthExeed(String st, int lengthEng) throws UnsupportedEncodingException {
    	int count = 0;
    	for (int i = 0; i < st.length(); i++) {
    		char c = st.charAt(i);
    		int n = CharCode.getCharCode(c); 
    		if (n >= beginCode && c <= endCode) {
    			count = count + 2;
    		} else {
    			count = count + 1;
    		}
    	}
    	if (count > lengthEng) {
    		return true;
    	} else {
    		return false;
    	}
    }
    
    //用粗暴一些的办法，非ascii码即汉字之类
    public static boolean isLengthExeedForChinese(String st, int lengthEng) throws UnsupportedEncodingException {
    	int count = 0;
    	for (int i = 0; i < st.length(); i++) {
    		if (st.codePointAt(i) > 255) {
    			count = count + 2;
    		} else {
    			count = count + 1;
    		}
    	}
    	if (count > lengthEng) {
    		return true;
    	} else {
    		return false;
    	}
    }    
}