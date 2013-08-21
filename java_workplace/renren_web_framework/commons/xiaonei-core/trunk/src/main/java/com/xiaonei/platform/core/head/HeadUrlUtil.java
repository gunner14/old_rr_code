package com.xiaonei.platform.core.head;

import com.xiaonei.platform.storage.Allocator;
import com.xiaonei.platform.storage.Constants;

/**
 * 用户头像URL工具
 * @author lifs
 * 
 */
public class HeadUrlUtil {
	/**
	 * 得到用户头像的完整URL地址
	 * @param pattern
	 * @return
	 */
	public static String getHeadFullUrl(String pattern) {
		try {
			if(pattern == null || pattern.length() == 0) {
				return null;
			}
			String url = Allocator.getInstance().locate(pattern,
					Constants.NODE_TYPE_HEAD);
			if (url == null)
			{				
				if (pattern.indexOf("head.jpg") > 0)
					url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/0/0/men_head.gif";
				else if (pattern.indexOf("main.jpg") > 0)
					url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/0/0/men_main.gif";
				else if (pattern.indexOf("tiny.jpg") > 0)
					url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/0/0/men_tiny.gif";
				else
					url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/" + pattern;
			}
			return url;
		} catch (Exception e) {
			e.printStackTrace(System.err);
			return null;
		}
	}
	/**
	 * 从外站得到用户头像的完整URL地址，避免图片防盗链
	 * @param pattern
	 * @return
	 */
	public static String getHeadFullUrlFromOuterSite(String pattern) {
		try {
			String url = getHeadFullUrl(pattern);
			if (url == null)
				return com.xiaonei.platform.core.opt.OpiConstants.urlTinyHead + "/photos/" + pattern;
			else{
				int pos = url.indexOf(com.xiaonei.platform.core.opt.OpiConstants.domainMain.toString());
				String part2 = url.substring(pos);
				return  "http://tiny.head." + part2;
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
			return null;
		}
	}
	public static void main(String[] ar){
		String u = "http://hd23.xiaonei.com/photos/hd23/20080102/14/01/tiny_3011k169.jpg";
		int pos = u.indexOf("xiaonei.com");
		String part2 = u.substring(pos);
		String newUrl = "http://tiny.head." + part2;
		
		System.out.println(newUrl);
	}
}
