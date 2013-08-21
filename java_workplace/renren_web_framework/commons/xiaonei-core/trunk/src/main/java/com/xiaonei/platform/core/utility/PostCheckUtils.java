package com.xiaonei.platform.core.utility;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;

public class PostCheckUtils {
	private static PostCheckUtils instance;
	static {
		PostCheckUtils.instance = new PostCheckUtils();
	}

	public static PostCheckUtils getInstance() {
		return PostCheckUtils.instance;
	}

	/**
	 * 此方法不检查Referer了，已无使用意义
	 * @deprecated
	 * @param request
	 * @param response
	 * @return
	 */
	public boolean needPostAndRightRefererForAjax(HttpServletRequest request,
			HttpServletResponse response) {
		// 需要Post提交
		if (!request.getMethod().toLowerCase().equals("post")) {
			return false;
		}

		// // 需要正确的Referer
		// String referer = request.getHeader("Referer");
		// if ((referer != null) && (referer.trim().length() > 0)) {
		// URL url = null;
		// try {
		// referer = referer.replaceAll("#", "");
		// url = new URL(referer);
		// } catch (MalformedURLException e) {
		// e.printStackTrace(System.err);
		// }
		// if (url != null) {
		// String hosturl = url.getHost().toLowerCase();
		// if (hosturl.endsWith("xiaonei.com")
		// || hosturl.endsWith("5q.com")) {
		// return true;
		// } else {
		// return false;
		// }
		// }
		// }
		return true;
	}

	/**
	 * 此方法不检查Referer了，已无使用意义
	 * @deprecated
	 * @param request
	 * @param response
	 * @return
	 */
	public boolean needPostAndRightReferer(HttpServletRequest request,
			HttpServletResponse response) {
		// 需要Post提交
		if (!request.getMethod().toLowerCase().equals("post")) {
			redirectPage(response);
			return false;
		}

		// // 需要正确的Referer
		// String referer = request.getHeader("Referer");
		// if ((referer != null) && (referer.trim().length() > 0)) {
		// URL url = null;
		// try {
		// referer = referer.replaceAll("#", "");
		// url = new URL(referer);
		// } catch (MalformedURLException e) {
		// e.printStackTrace(System.err);
		// }
		// if (url != null) {
		// String hosturl = url.getHost().toLowerCase();
		// if (hosturl.endsWith("xiaonei.com")
		// || hosturl.endsWith("5q.com")) {
		// } else {
		// redirectPage(response);
		// return false;
		// }
		// }
		// } else {
		// redirectPage(response);
		// return false;
		// }
		return true;
	}

	private void redirectPage(HttpServletResponse response) {
		try {
			response.sendRedirect(OpiConstants.urlWww.toString());
			return;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
