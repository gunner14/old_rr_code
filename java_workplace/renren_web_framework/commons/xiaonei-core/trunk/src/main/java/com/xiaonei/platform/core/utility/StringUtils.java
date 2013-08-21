/*
 * Created on 2003-12-9
 */
package com.xiaonei.platform.core.utility;

import java.io.File;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;


/**
 * �ַ����߰�
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class StringUtils {
	
	
	
	
	/**
	 * ����Ƿ��зǷ���
	 * 
	 * @param origin
	 * @param forbids
	 * @return true if not illegal
	 */
	public static boolean checkForbiddenWords(String origin, String[] forbids) {
		return false;
	}

	/**
	 * ���˷Ƿ��ʣ����ǣ��ּ�ո�����һͬ���ˣ��Ƿ����õȳ��ȵ�"*"�滻
	 * 
	 * @param origin
	 * @param forbids
	 * @return
	 */
	public static String filterForbiddenWords(String origin, String[] forbids) {
		return filterForbiddenWords(origin, forbids, "*");
	}

	/**
	 * ���˷Ƿ��ʣ����ǣ��ּ�ո�����һͬ���ˣ��Ƿ����õȳ��ȵ�replace�滻
	 * 
	 * @param origin
	 * @param forbids
	 * @return ���˺���ַ�
	 */
	public static String filterForbiddenWords(String origin, String[] forbids, String replace) {
		return null;
	}
	
	/**
	 * ��str�е������ַ���'\n'��'<'�ȣ�ת��Ϊ����ʾ��ҳ���ϵ�HTML����
	 * 
	 * @param str
	 * @return string after encode
	 */
	public static String filterHtmlEncode(String str){
    if(str == null)
      return "";
		StringBuffer sb = new StringBuffer(str.length() + str.length()/4);
		for (int i=0;i<str.length();i++){
			char ch = str.charAt(i);
			switch (ch) {
				case '\n':
					sb.append("<br>");
					break;
				case '<':
					sb.append("&lt;");
					break;
				case '>':
					sb.append("&gt;");
					break;
				case '&':
					sb.append("&amp;");
					break;
				case '\'':
					sb.append("&apos;");
					break;
				case '\"':
					sb.append("&quot;");
					break;
				case ' ':
					sb.append("&nbsp;");
					break;
				default:
					sb.append(ch);
			}
		}
		return sb.toString();
	}

	/**
	 * filter null string to ""
	 * @param s
	 * @return
	 */
	public static String filterNullString(String s)  {
		return filterNullString(s, "");
	}

	/**
	 * filter null string to def
	 * @param s
	 * @param def
	 * @return
	 */
	public static String filterNullString(String s, String def)  {
		if(s == null || "".equals(s)) return def;
		return s;
	}

	/**
	 * Convert date format to specified pattern.
	 * @param date
	 * @param pattern
	 * @return string expression of date according to pattern
	 */
	public static String formatDate(Date date, String pattern) {
		SimpleDateFormat sdf = new SimpleDateFormat(pattern);
		return sdf.format(date);
	}

	/**
	 * Convert date format to specified pattern.
	 * @param date
	 * @param pattern
	 * @return string expression of date according to pattern
	 */
	public static Date parseDate(String str, String pattern) {
		SimpleDateFormat sdf = new SimpleDateFormat(pattern);
		try {
			return sdf.parse(str);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return new Date();
	}

	/**
	 * ��ȡһ�������ַ�����ҳ����ʾ�����ȳ����ȡ���֣���"..."��β
	 * 
	 * @param str
	 * @param start
	 * @param end
	 * @param postfix
	 * @return string after substring
	 */
	public static String substring(String str, int start, int end) {
		return substring(str, start, end, "...");
	}

	/**
	 * ��ȡһ�������ַ�����ҳ����ʾ�����ȳ����ȡ���֣���postfix��β
	 * 
	 * @param str
	 * @param start
	 * @param end
	 * @param postfix
	 * @return string after substring
	 */
	public static String substring(String str, int start, int end, String postfix) {
		int length = str.length();
		if (length <= start) {
			return str;
		} else if (length <= end){
			return str.substring(start, length);
		} else {
			return str.substring(start, end) + postfix;
		}
	}
	
	/**
	 * ��GBK�����ַ�ת��ΪISO8859_1����
	 * @param origin
	 * @return
	 */
	public static String toASCIIString(String origin) {
		if ( null == origin ) return "";

		try {
			return new String(origin.getBytes("GBK"), "ISO8859_1");
		} catch ( Exception e ) {
			return "";
		}
	}

	/**
	 * ��ISO8859_1�����ַ�ת��ΪGBK���� 
	 * @param origin
	 * @return
	 */
	public static String toGBKString(String origin) {
		if ( null == origin ) return "";

		try {
			return new String(origin.getBytes("ISO8859_1"), "GBK");
		} catch ( Exception e ) {
			return "";
		}
	}
	
	/**
	 * ����ļ����·��ȡ�ú�׺��
	 * @param filename
	 * @return
	 */
	public static String parseFileExt(String filename) {
		int pos = filename.lastIndexOf('.');
		if (pos >= 0) {
			return filename.substring(pos + 1);
		} else {
			return "";
		}
	}
	
	/**
	 * ����ļ�·��ȡ���ļ���
	 * @param filename
	 * @return
	 */
	public static String parseFileName(String filename) {
		int pos = filename.lastIndexOf(File.separator);
		if (pos >= 0) {
			return filename.substring(pos + 1);
		} else {
			return filename;
		}
	}

}
