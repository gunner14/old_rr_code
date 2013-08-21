package com.xiaonei.platform.core.utility;

import java.util.Calendar;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.utility.enumeration.IdcEnum;
import com.xiaonei.platform.storage.Allocator;

public class AlbumUtil {

	public static String getPhotoURL(String path) {
		String url = "";
		try {
			url = Allocator.getInstance().locate(path);
			if (url == null) {
				if (path.startsWith("5q")) {
					String flag = path.substring(3);
					String domain = getPhotoDomain(flag);
					url = domain + "/" + flag;
				} else {
					if (path.startsWith("pic001")) {
						url = OpiConstants.urlPic001 + "/" + path;
					} else if (path.startsWith("pic002")) {
						url = OpiConstants.urlPic002 + "/" + path;
					} else {
						url = OpiConstants.urlImg + "/photos/" + path;
						try {

							String abc = path.substring(0, 8);
							if (Integer.parseInt(abc) >= 20070101) {
								url = OpiConstants.urlHead + "/photos/" + path;
							}
						} catch (Exception e) {
						}
					}
				}
			}
		} catch (Exception e) {

		}

		return url;
	}

	/**
	 * 通过传入idc，拼出带有idc的url
	 * 
	 * @param path
	 * @param idcEnum
	 * @return
	 */
	public static String getPhotoURLWithIdc(String path, IdcEnum idcEnum) {
		String fullUrl = getPhotoURL(path);

		if (idcEnum != null
				&& StringUtils.isNotBlank(fullUrl)
				&& (fullUrl.startsWith("http://fmn.rrimg.com") || fullUrl
						.startsWith("http://fmn.rrfmn.com"))) {
			return fullUrl.replaceFirst("http://", "http://" + idcEnum.getIdc()
					+ ".");
		}

		return fullUrl;
	}

	private static String getPhotoDomain(String flag) {
		if (flag.startsWith("c02"))
			return OpiConstants.urlPhoto2Album;
		else
			return OpiConstants.urlPhotoAlbum;
	}

	/**
	 * TODO 需要为此过期方法提供替代的解决方案
	 * 
	 * @deprecated
	 * @param hostName
	 * @return
	 */
	public static String getCommonFileName(String hostName) {
		String time = "" + System.currentTimeMillis();
		String fileName = time.substring(time.length() - 7)
				+ (int) (Math.random() * 90000 + 10000)
				+ hostName.substring(0, 3);

		return fileName;
	}

	public static String getDiv1(Calendar cl) {
		String ys, ms;
		ys = cl.get(Calendar.YEAR) + "";
		int month = cl.get(Calendar.MONTH) + 1;
		if (month < 10) {
			ms = "0" + month;
		} else {
			ms = month + "";
		}
		int day = cl.get(Calendar.DATE);
		if (day < 10) {
			return ys + ms + "0" + day;
		} else {
			return ys + ms + "" + day;
		}
	}

	public static String getDiv2(Calendar cl) {

		return getDiv2(cl, 5);
		/*
		 * String div2; if (cl.get(Calendar.HOUR_OF_DAY) < 10) { div2 = "0" +
		 * cl.get(Calendar.HOUR_OF_DAY) + ""; } else { div2 =
		 * cl.get(Calendar.HOUR_OF_DAY) + ""; } div2 += "/"; int min =
		 * cl.get(Calendar.MINUTE); min = min / 5 * 5; if (min < 10) div2 +=
		 * "0"; div2 += min;
		 * 
		 * 
		 * int first = min/10 ; int second = min%10 ;
		 * 
		 * if(second>=5) div2 += first+"5" ; else div2 += first+"0" ;
		 * 
		 * 
		 * 
		 * switch (min / 10) { case 0: div2 = div2 + "00"; break; case 1: div2 =
		 * div2 + "10"; break; case 2: div2 = div2 + "20"; break; case 3: div2 =
		 * div2 + "30"; break; case 4: div2 = div2 + "40"; break; case 5: div2 =
		 * div2 + "50"; break; }
		 * 
		 * return div2;
		 */
	}

	/**
	 * 可以指定创建目录的分钟间隔
	 * 
	 * @param cl
	 * @param mins
	 *            60分钟以内，如30 半分钟，5 每5分钟
	 * @return
	 */
	public static String getDiv2(Calendar cl, int mins) {
		String div2;
		if (cl.get(Calendar.HOUR_OF_DAY) < 10) {
			div2 = "0" + cl.get(Calendar.HOUR_OF_DAY) + "";
		} else {
			div2 = cl.get(Calendar.HOUR_OF_DAY) + "";
		}
		div2 += "/";
		int min = cl.get(Calendar.MINUTE);
		min = min / mins * mins;
		if (min < 10)
			div2 += "0";
		div2 += min;

		/*
		 * int first = min/10 ; int second = min%10 ;
		 * 
		 * if(second>=5) div2 += first+"5" ; else div2 += first+"0" ;
		 */

		/*
		 * switch (min / 10) { case 0: div2 = div2 + "00"; break; case 1: div2 =
		 * div2 + "10"; break; case 2: div2 = div2 + "20"; break; case 3: div2 =
		 * div2 + "30"; break; case 4: div2 = div2 + "40"; break; case 5: div2 =
		 * div2 + "50"; break; }
		 */
		return div2;
	}
}
