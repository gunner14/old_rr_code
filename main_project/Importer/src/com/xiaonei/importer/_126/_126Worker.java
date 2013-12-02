package com.xiaonei.importer._126;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.UUID;

import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpConnectionManager;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.MultiThreadedHttpConnectionManager;
import org.apache.commons.httpclient.cookie.CookiePolicy;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.StringRequestEntity;
import org.apache.commons.httpclient.params.DefaultHttpParams;
import org.apache.commons.httpclient.params.HttpConnectionManagerParams;
import org.apache.commons.httpclient.params.HttpMethodParams;

import com.xiaonei.importer.AbstractWorker;
import com.xiaonei.importer.WorkerCallback;
import com.xiaonei.importer.google.InfomationCache;
import com.xiaonei.importer.netease.NeteaseProtocal;
import com.xiaonei.svc.importer.BuddyInfo;
import com.xiaonei.svc.importer.BuddyList;
import com.xiaonei.svc.importer.BuddyListStatus;

public class _126Worker extends AbstractWorker {
	private static org.apache.commons.logging.Log _logger = org.apache.commons.logging.LogFactory
			.getLog(_126Worker.class);

	private final String _126Id;
	private final String _126Password;

	private StringBuilder cookie = new StringBuilder();
	private String sid = null;
	private String wmsvr_domain = null;

	private BuddyInfo[] _buddylist;

	protected _126Worker(String __126Id, String __126Password, UUID requestId,
			WorkerCallback callback) {
		super(requestId, callback);
		this._126Id = __126Id;
		this._126Password = __126Password;
	}

	public void run() {
		boolean auth = getAuth();

		BuddyListStatus status = null;
		if (auth) {
			status = new BuddyListStatus(302, "Token OK, needs contacts");
		} else {
			status = new BuddyListStatus(403, "Auth failed.");
		}
		BuddyList list = InfomationCache.getInstance().getContacts(
				this._requestId);
		list.status = status;

		if (auth) {
			exportContacts();
		}
	}

	private static HttpClient httpclient;
	static {
		DefaultHttpParams.getDefaultParams().setBooleanParameter(
				HttpMethodParams.SINGLE_COOKIE_HEADER, true);
		DefaultHttpParams.getDefaultParams().setParameter(
				HttpMethodParams.COOKIE_POLICY, CookiePolicy.IGNORE_COOKIES);

		HttpConnectionManager manager = new MultiThreadedHttpConnectionManager();
		HttpConnectionManagerParams parm = new HttpConnectionManagerParams();
		parm.setDefaultMaxConnectionsPerHost(64);
		parm.setMaxTotalConnections(128);
		parm.setStaleCheckingEnabled(true);
		manager.setParams(parm);
		httpclient = new HttpClient(manager);
	}


	private boolean getAuth() {
		String url = "http://reg.163.com/login.jsp?type=1&url=http://entry.mail.163.com/coremail/fcg/ntesdoor2?lightweight%3D1%26verifycookie%3D1%26language%3D-1%26style%3D-1";
		PostMethod post = new PostMethod(url);
		String NTES_SESS = null, S_INFO = null, P_INFO = null, URSJESSIONID = null;
		try {
			post.addParameter("verifycookie", "1");
			post.addParameter("username", this._126Id);
			post.addParameter("password", this._126Password);
			post.addParameter("selType", "-1");
			post.addParameter("remUser", "");
			post.addParameter("%B5%C7%C2%BC%D3%CA%CF%E4",
					"%B5%C7%C2%BC%D3%CA%CF%E4");
			post.addParameter("style", "-1");
			post.addParameter("type", "1");
			post.addParameter("product", "mail163");
			post.addParameter("savelogin", "");
			post.addParameter("outfoxer", "");
			try {
				httpclient.executeMethod(post);
			} catch (Exception e) {
				_logger.error("ex ", e);
				return false;
			}

//			System.out.println(post.getResponseBodyAsString());
			for (Header head : post.getResponseHeaders("Set-Cookie")) {
				String str = head.getValue();
				String cuk1 = str.split(";")[0];
				int i = cuk1.indexOf('=');
				String key = cuk1.substring(0, i);
				String value = cuk1.substring(i + 1);
				if ("NTES_SESS".equals(key)) {
					NTES_SESS = value;
				} else if ("S_INFO".equals(key)) {
					S_INFO = value;
				} else if ("P_INFO".equals(key)) {
					P_INFO = value;
				} else if ("URSJESSIONID".equals(key)) {
					URSJESSIONID = value;
				}
			}
		} catch (Exception e) {
			_logger.error(e);
		} finally {
			post.releaseConnection();
		}

		if (NTES_SESS == null || S_INFO == null || P_INFO == null || URSJESSIONID == null) {
			return false;
		}
		cookie.append("NTES_SESS=" + NTES_SESS + "; S_INFO=" + S_INFO + "; P_INFO=" + P_INFO
		+ "; URSJESSIONID=" + URSJESSIONID);

		url = "http://entry.mail.126.com/cgi/ntesdoor?hid=10010102&lightweight=1&verifycookie=1&language=0&style=-1&username="
		+ this._126Id;
		GetMethod get = new GetMethod(url);
		get.setFollowRedirects(false);
		get.setRequestHeader("Cookie", cookie.toString());
		try {
			try {
				httpclient.executeMethod(get);
			} catch (Exception e) {
				_logger.error("get ex: ", e);
			}

			String coremail = null;
			try {
				String coremailstr = get.getResponseHeader("Set-Cookie")
						.getValue().split(";")[0];
				coremail = coremailstr.substring(coremailstr.indexOf('=') + 1);
				sid = coremail.split("%")[1];
			} catch (Exception e) {
				_logger.warn("parse 126 login error in step 2", e);
				return false;
			}
			cookie.append("; Coremail=" + coremail);

			String location = get.getResponseHeader("Location").getValue();
			wmsvr_domain = location.split("/")[2];
			cookie.append("; wmsvr_domain=" + wmsvr_domain);
			

			return true;
		} catch (Exception e) {
			_logger.debug(e);
			return false;
		} finally {
			get.releaseConnection();
		}
	}

	private void exportContacts() {
		PostMethod post = new PostMethod("http://" + wmsvr_domain + "/a/s?sid="
				+ sid + "&func=global:sequential");
		post.setRequestHeader("Accept-Encoding", "identity");
		post.setRequestHeader("Referer", "http://" + wmsvr_domain
				+ "/a/f/js3/0801290924/index_v5.htm");
		post.setRequestHeader("Cookie", cookie.toString());
		String postBody = "<?xml version=\"1.0\"?><object><array name=\"items\"><object><string name=\"func\">pab:searchContacts</string><object name=\"var\"><array name=\"order\"><object><string name=\"field\">FN</string><boolean name=\"ignoreCase\">true</boolean></object></array></object></object><object><string name=\"func\">user:getSignatures</string></object><object><string name=\"func\">pab:getAllGroups</string></object></array></object>";
		String response = null;
		try {
			post.setRequestEntity(new StringRequestEntity(postBody,
					"application/xml", "UTF-8"));
			try {
				httpclient.executeMethod(post);
			} catch (Exception e) {
				_logger.error("ex export contacts", e);
			}
			response = post.getResponseBodyAsString();
		} catch (UnsupportedEncodingException e1) {
			_logger.warn(e1);
		} catch (HttpException e) {
			_logger.warn(e);
		} catch (IOException e) {
			_logger.warn(e);
		} finally {
			post.releaseConnection();
		}
		if (response != null) {
			try {
				_buddylist = NeteaseProtocal.prase(response);
				BuddyListStatus status = new BuddyListStatus(200, "OK");
				_callback.finishContactList(_requestId, _buddylist, status);
			} catch (Exception e) {
				_logger.warn(e);
			}
		}
	}

}
