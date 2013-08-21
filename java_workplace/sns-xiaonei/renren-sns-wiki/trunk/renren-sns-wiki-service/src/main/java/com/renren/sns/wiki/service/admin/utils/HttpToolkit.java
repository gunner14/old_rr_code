package com.renren.sns.wiki.service.admin.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.URL;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Properties;
import java.util.zip.GZIPInputStream;

import com.renren.sns.wiki.service.admin.utils.bean.NameValuePair;

/**
 * HttpToolkit的实现类 设置全局代理 HttpToolkit.setProxy("192.168.1.1", 8080);
 */
public class HttpToolkit {
    
	private boolean debug = false;

	private static Proxy proxy;

	private static int timeout = 30000;
	private Map<String, String> cookieMap;
	private String cookie = "";

	private String charset = "utf-8";
	public int statusCode;
	private String location = null;
	private boolean gzip = false; // 是否进行了gzip压缩
	private int contentLength; // 上次请求返回的Content-Length

	// 默认的http请求头,可以调用set方法设置
	private static Map<String, String> defaultHeaderMap = new LinkedHashMap<String, String>();
	// 自定义的headerMap,当这个属性不为null的时候,defaultHeaderMap将失效
	private Map<String, String> headerMap;

	// 是否自动跳转
	private boolean redirect = true;// 是否自动跳转,默认为true

	static {
		// ie 9
		defaultHeaderMap.put("Accept", "text/html, application/xhtml+xml, */*");
		defaultHeaderMap.put("Accept-Language", "zh-CN");
		defaultHeaderMap
				.put("User-Agent",
						"Mozilla/5.0 (compatible; MSIE 9.0; QDesk 2.3.1185.202; qdesk 2.3.1186.202; Windows NT 6.1; WOW64; Trident/5.0)");
		defaultHeaderMap.put("Accept-Encoding", "gzip, deflate");
	}

	public HttpToolkit() {
	}

	public static void initProxy() {
		// 是否需要设置代理
		File file = new File("proxy.txt");
		if (file.exists()) {
			Properties pro = new Properties();
			try {
				pro.load(new FileReader(file));
				if (pro.containsKey("proxy")
						&& pro.getProperty("proxy").trim()
								.equalsIgnoreCase("false")) {
					closeProxy();
				} else if (pro.containsKey("ip")) {
					String proxyIp = pro.getProperty("ip");
					int proxyPort = 80;
					if (pro.containsKey("port")) {
						proxyPort = Integer.parseInt(pro.getProperty("port"));
					}
					setProxy(proxyIp, proxyPort);
				} else {
					closeProxy();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			closeProxy();
		}
	}

	public static void setProxy(String ip, int port) {
		HttpToolkit.proxy = new Proxy(Proxy.Type.HTTP, new InetSocketAddress(
				ip, port));
	}

	public static void closeProxy() {
		HttpToolkit.proxy = null;
	}

	public static Proxy getProxy() {
		return HttpToolkit.proxy;
	}

	public String get(String url) {
		return get(url, null, null, null);
	}

	public String get(String url, String cookie) {
		return get(url, cookie, null, null);
	}

	public String get(String url, String cookie, String referer) {
		return get(url, cookie, referer, null);
	}

	public String get(String url, String cookie, String referer, String charset) {
		InputStream in = openInputStream(url, "GET", null, cookie, referer);
		if (charset != null) {
			this.charset = charset;
		}
		if (301 == statusCode || 302 == statusCode) {
			if (location != null && redirect) {
				return get(this.location, this.cookie, referer, charset);
			}
		}
		return stream2String(in, this.charset);
	}

	public String post(String url) {
		return post(url, null);
	}

	public String post(String url, NameValuePair[] params) {
		return post(url, params, null, null, null);
	}

	public String post(String url, NameValuePair[] params, String cookie) {
		return post(url, params, cookie, null, null);
	}

	public String post(String url, NameValuePair[] params, String cookie,
			String referer) {
		return post(url, params, cookie, referer, null);
	}

	public String post(String url, NameValuePair[] params, String cookie,
			String referer, String charset) {
		byte[] data = null;
		if (params != null && params.length > 0) {
			try {
				StringBuilder sb = new StringBuilder();
				for (NameValuePair nameValuePair : params) {
					sb.append("&").append(nameValuePair.getName()).append("=")
							.append(nameValuePair.getValue());
				}

				data = sb.substring(1).getBytes("utf-8");
				if (debug) {
					System.out.println("POST data is : "
							+ new String(data, "utf-8"));
				}
			} catch (UnsupportedEncodingException e) {
				throw new RuntimeException(e);
			}
		}
		return post(url, data, cookie, referer, charset);
	}

	public String post(String url, byte[] data, String cookie, String referer,
			String charset) {
		InputStream in = openInputStream(url, "POST", data, cookie, referer);
		if (charset != null) {
			this.charset = charset;
		}
		if (debug) {
			System.out.println("statusCode=" + statusCode + ", charset="
					+ this.charset + ", gzip=" + this.gzip);
		}
		if (301 == statusCode || 302 == statusCode) {
			if (debug) {
				System.out.println("statusCode=" + statusCode
						+ " and location=" + location);
			}
			if (location != null && redirect) {
				if (redirect) {
					return get(this.location, this.cookie, url, charset);
				}
			}
		}
		return stream2String(in, this.charset);
	}

	public String getCookie() {
		return this.cookie;
	}

	public InputStream openInputStream(String url, String cookie, String referer) {
		return openInputStream(url, "GET", null, cookie, referer);
	}

	public InputStream openInputStream(String url, String method, byte[] data,
			String cookie, String referer) {
		if (url == null) {
			throw new RuntimeException("url不能为空");
		}
		url = handleUrl(url);
		if (debug) {
			System.out.println("请求url: " + url);
			System.out.println("请求referer:" + referer);
			System.out.println("请求前cookie: " + cookie);
		}

		InputStream in = null;
		HttpURLConnection conn = null;
		try {
			URL urlNew = new URL(url);
			if (proxy == null) {
				conn = (HttpURLConnection) urlNew.openConnection();
			} else {
				conn = (HttpURLConnection) urlNew
						.openConnection(HttpToolkit.proxy);
			}
			// 设置为不自动跳转
			conn.setInstanceFollowRedirects(false);

			conn.setDoInput(true);
			if ("POST".equals(method)) {
				conn.setDoOutput(true);
			}

			conn.setConnectTimeout(10000);

			conn.setRequestMethod(method);
			conn.setReadTimeout(timeout);

			String host = getHost(url);
			if (!"".equals(host)) {
				conn.setRequestProperty("Host", host);
			}

			Map<String, String> map = null;
			if (headerMap == null) {
				map = defaultHeaderMap;
			} else {
				map = headerMap;
			}
			for (String key : map.keySet()) {
				conn.setRequestProperty(key, map.get(key));
			}

			// if (proxy) {
			// conn.setRequestProperty("Proxy-Connection", "Keep-Alive");
			// }
			if (referer != null) {
				conn.setRequestProperty("Referer", referer);
			}
			if (cookie != null && cookie.trim() != "") {
				this.cookie = cookie;
				conn.setRequestProperty("Cookie", this.cookie);
			}

			conn.connect();

			// POST向服务器写数据
			if ("POST" == method && data != null) {
				OutputStream out = conn.getOutputStream();
				out.write(data);
				out.close();
			}

			// statusCode
			String statusLine = null;
			if (conn.getContentLength() > 0) {
				statusLine = conn.getHeaderField(0);
			}
			if (statusLine != null) {
				String[] status = statusLine.split("\\s+");
				if (status.length >= 2) {
					statusCode = Integer.parseInt(status[1]);
				}
			}

			if (debug) {
				System.out.println("statusCode:" + statusCode);
			}

			if (404 == statusCode) {
				return null;
			}

			if (301 == statusCode || 302 == statusCode) {
				location = conn.getHeaderField("Location");
				if (location != null && !location.startsWith("http")) {
					location = "http://" + host + "/" + location;
				}
				if (debug) {
					System.out.println("statusCode=" + statusCode
							+ ", location=" + location);
				}
			}

			// 将获取的cookie重新组合
			cookieMap = cookie2Map(this.cookie);
			int i = 0;
			while (true) {
				i++;
				String headerKey = null;
				String headerValue = null;
				try {
					headerKey = conn.getHeaderFieldKey(i);
					headerValue = conn.getHeaderField(i);
				} catch (Exception e) {
					headerKey = null;
					e.printStackTrace();
				}
				if (headerKey == null) {
					break;
				}
				if ("Set-Cookie".equalsIgnoreCase(headerKey)) {
					if (debug) {
						System.out.println("Set-Cookie\t" + headerKey + "="
								+ headerValue);
					}
					// 好多返回的cookie不包括domain
					// if(!headerValue.contains("domain")){
					// continue;
					// }
					String[] ss = headerValue.split("\\s*;\\s*");
					boolean flag = false;
					for (String s : ss) {
						if (!s.startsWith("path=")) {
							continue;
						} else {
							if (!s.equals("path=/")) {
								flag = true;
							}
							break;
						}
					}

					if (flag) {
						continue;
					}

					if (ss != null && ss.length > 0) {
						String[] cookieNameValue = ss[0].split("=", 2);
						if (cookieNameValue.length == 2) {
							String cookieKey = cookieNameValue[0];
							String cookieValue = cookieNameValue[1];
							cookieMap.put(cookieKey, cookieValue);
						}
					}
				}
			}
			// 将cookieMap转化为cookie
			this.cookie = map2Cookie(cookieMap);
			if (debug) {
				System.out.println("请求后cookie: " + this.cookie);
			}

			// 文本编码
			String contentType = conn.getHeaderField("Content-Type");
			if (contentType != null && contentType.contains("charset=")) {
				int beginIndex = contentType.indexOf("charset=")
						+ "charset=".length();
				this.charset = contentType.substring(beginIndex);
			}

			if ("gzip"
					.equalsIgnoreCase(conn.getHeaderField("Content-Encoding"))) {
				this.gzip = true;
			} else {
				this.gzip = false;
			}

			// Content-Length
			String strContentLength = conn.getHeaderField("Content-Length");
			if (strContentLength != null) {
				contentLength = Integer.parseInt(strContentLength.trim());
			}

			in = conn.getInputStream();
			if (gzip && in != null) {
				return new GZIPInputStream(in);
			} else {
				return in;
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	/**
	 * 根据url地址获取host
	 * 
	 * @param url
	 * @return
	 */
	private String getHost(String url) {
		String host = "";
		if (url.contains("//")) {
			int beginIndex = url.indexOf("//") + 2;
			int endIndex = url.indexOf("/", beginIndex);
			if (endIndex != -1 && beginIndex < endIndex) {
				host = url.substring(beginIndex, endIndex);
			} else {
				host = url.substring(beginIndex);
			}
		}
		return host;
	}

	/**
	 * 将cookie转化为cookieMap
	 * 
	 * @param cookie
	 * @return
	 */
	public static Map<String, String> cookie2Map(String cookie) {
		Map<String, String> map = new LinkedHashMap<String, String>();
		if (cookie != null) {
			String[] cookies = cookie.split(";\\s*");
			for (String s : cookies) {
				String[] ss = s.split("=", 2);
				if (ss.length == 2) {
					map.put(ss[0], ss[1]);
				}
			}
		}
		return map;
	}

	/**
	 * 将map转化为cookie的子串格式
	 * 
	 * @param cookieMap
	 * @return
	 */
	public static String map2Cookie(Map<String, String> cookieMap) {
		StringBuilder sb = new StringBuilder();
		for (String key : cookieMap.keySet()) {
			String cookieValue = cookieMap.get(key);
			if (cookieValue != null && (!"null".equalsIgnoreCase(cookieValue))
					&& (cookieValue.length() != 0)) {
				sb.append("; ").append(key).append("=")
						.append(cookieMap.get(key));
			}
		}
		if (sb.length() > 2) {
			return sb.substring(2);
		} else {
			return "";
		}
	}

	/**
	 * 对url进行处理,将url域名后补充/ 因为不支持https,所以将https换成http
	 * 
	 * @param url
	 * @return
	 */
	private String handleUrl(String url) {

		if (url == null) {
			return null;
		}

		String result = url;

		int beginIndex = result.indexOf("//");
		if (beginIndex != -1) {
			int endIndex = result.indexOf("/", beginIndex + 2);
			if (endIndex == -1) {
				int questionIndex = result.indexOf("?");
				if (questionIndex == -1) {
					result += "/";
				} else {
					result = result.replaceFirst("/?[?]", "/?");
				}
			}
		}
		if (result.startsWith("https://")) {
			result = result.replaceFirst("https://", "http://");
		}
		return result;
	}

	/**
	 * 得到cookie某个值的值
	 * 
	 * @param cookie
	 * @param key
	 * @return
	 */
	public static String getCookieValue(String cookie, String key) {
		if (cookie == null || key == null) {
			return null;
		}
		Map<String, String> map = HttpToolkit.cookie2Map(cookie);
		return map.get(key);
	}

	/**
	 * 根据参数名称返回参数值
	 * 
	 * @param originUrl
	 * @param key
	 * @return
	 */
	public static String getUrlValue(String urlParams, String key) {

		if (urlParams == null) {
			return null;
		}
		if (urlParams.contains("?")) {
			int beginIndex = urlParams.indexOf("?");
			urlParams = urlParams.substring(beginIndex + 1);
		}
		urlParams = urlParams.replaceAll("&amp;", "&");

		String[] ss = urlParams.split("&");
		for (String keyValue : ss) {
			String[] entry = keyValue.split("=", 2);
			if (entry.length == 2) {
				String paramsKey = entry[0];
				String paramsValue = entry[1];
				if (paramsKey.equals(key)) {
					return paramsValue;
				}
			}
		}
		return null;
	}

	/**
	 * 将流转化为字符串
	 * 
	 * @param in
	 * @param charSet
	 * @return
	 */
	public static String stream2String(InputStream in, String charset) {
		if (in == null) {
			return null;
		}
		if ("ISO-8859-1".equalsIgnoreCase(charset)) {
			charset = "UTF-8";
		}
		BufferedReader inbuffer = null;
		StringBuilder sb = new StringBuilder();
		try {
			inbuffer = new BufferedReader(new InputStreamReader(in, charset));
			String s = null;
			while ((s = inbuffer.readLine()) != null) {
				sb.append(s).append("\n");
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		} finally {
			if (inbuffer != null) {
				try {
					inbuffer.close();
				} catch (IOException e) {
					throw new RuntimeException(e);
				}
			}
		}
		return sb.toString();
	}

	public boolean isGzip() {
		return gzip;
	}

	public String getCharset() {
		return charset;
	}

	public void setCookie(String cookie) {
		this.cookie = cookie;
	}

	public String getLocation() {
		return location;
	}

	public void setLocation(String location) {
		this.location = location;
	}

	public int getContentLength() {
		return contentLength;
	}

	public static void setDefaultHeaderMap(Map<String, String> defaultHeaderMap) {
		HttpToolkit.defaultHeaderMap = defaultHeaderMap;
	}

	public static Map<String, String> getDefaultHeaderMap() {
		return defaultHeaderMap;
	}

	public void setHeaderMap(Map<String, String> headerMap) {
		this.headerMap = headerMap;
	}

	public Map<String, String> getHeaderMap() {
		return headerMap;
	}

	public boolean isRedirect() {
		return redirect;
	}

	public void setRedirect(boolean redirect) {
		this.redirect = redirect;
	}

	public int getStatusCode() {
		return statusCode;
	}

}
