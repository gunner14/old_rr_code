package com.xiaonei.platform.scheduleTask;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.NameValuePair;
import org.apache.commons.httpclient.cookie.CookiePolicy;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpClientParams;
import org.apache.commons.httpclient.params.HttpMethodParams;
import org.apache.commons.io.IOUtils;

public class HttpclientUtil {

	private HttpClient client;

	private HttpClientParams hparas;

	public HttpclientUtil() {
		this.client = new HttpClient();
		this.hparas = new HttpClientParams();
		this.hparas.setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);
		this.hparas.setBooleanParameter("http.protocol.single-cookie-header",
				true);
		client.setParams(hparas);
	}

	public HttpclientUtil(int _timeout) {
		this.client = new HttpClient();
		this.hparas = new HttpClientParams();
		this.hparas.setSoTimeout(_timeout);
		this.hparas.setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);
		this.hparas.setBooleanParameter("http.protocol.single-cookie-header",
				true);
		client.setParams(hparas);
	}

	public HttpClient login(String target, Map parameters, String methodtype) {
		String redirectLocation = "";
		HttpMethod method = getHttpMethod(target, methodtype, parameters);
		try {
			this.client.executeMethod(method);
			redirectLocation = getRedirectLoacation(method);
			if (httpIsRedirect(redirectLocation)) {
				method.releaseConnection();
				return login(redirectLocation, parameters, methodtype);
			}
			return this.client;
		} catch (HttpException e) {
			return null;
		} catch (IOException e) {
			return null;
		} finally {
			method.releaseConnection();
		}

	}

	private boolean httpIsRedirect(String redirectLocation) {
		boolean isredirect = false;
		if (redirectLocation != null && !redirectLocation.trim().equals("")
				&& redirectLocation.toLowerCase().startsWith("http")) {
			isredirect = true;

		}
		return isredirect;
	}

	private String getRedirectLoacation(HttpMethod method) {
		String redirectLocation = "";
		Header locationHeader = null;
		locationHeader = method.getResponseHeader("location");

		if (locationHeader != null) {
			redirectLocation = locationHeader.getValue();
		}
		return redirectLocation;

	}

	private void addPostParameter(HttpMethod method, Map parameters) {
		if (parameters != null && !parameters.isEmpty()) {
			Set keySet = parameters.keySet();
			Iterator keys = keySet.iterator();
			int i = 0;
			NameValuePair parameter;
			for (; keys.hasNext(); ((PostMethod) method)
					.addParameter(parameter)) {
				Object key = keys.next();
				Object val = parameters.get(key);
				parameter = new NameValuePair(key.toString(), val.toString());
			}
		}
	}

	private String transTargetByGet(String target, Map parameters) {
		if (parameters != null && !parameters.isEmpty()) {
			Set keySet = parameters.keySet();
			Iterator keys = keySet.iterator();
			while (keys.hasNext()) {
				Object key = keys.next();
				Object val = parameters.get(key);
				String p1 = "?" + key + "=" + val;
				String p2 = "&" + key + "=" + val;
				if (target.indexOf(p1) == -1 && target.indexOf(p2) == -1) {
					if (target.indexOf("?") == -1) {
						target += "?" + key + "=" + val;
					} else {
						target += "&" + key + "=" + val;
					}
				}
			}
		}
		return target;
	}

	private InputStream getHttpInputStream(HttpClient client,
			HttpMethod method, Map parameters) {
		InputStream input = null;
		String redirectLocation = "";
		method.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
				new DefaultHttpMethodRetryHandler());
		try {
			int statusCode = client.executeMethod(method);			
			
			if (statusCode == HttpStatus.SC_OK) {  //page 200 OK 
				redirectLocation = getRedirectLoacation(method);
				if (httpIsRedirect(redirectLocation)) {
					method.releaseConnection();
					return getHttpInputStream(client, getHttpMethod(
							redirectLocation, method.getName(), parameters),
							parameters);
				} 
			}else if(statusCode==HttpStatus.SC_NOT_FOUND ){//page 404 Not Found 
				    return input;
			}

			input = method.getResponseBodyAsStream();
		} catch (HttpException e) {
			System.out.println("Please check your provided http address!");
		} catch (IOException e) {
			e.printStackTrace();
		}
		return input;

	}

	private String getBodyToString(HttpClient client, HttpMethod method,
			Map parameters, String encoding) {
		String body = "";
		InputStream input = getHttpInputStream(client, method, parameters);
		try {
			if(input==null){
			   return body;	
			}else
			   body = IOUtils.toString(input, encoding);			
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			method.releaseConnection();
		}
		return body;
	}

	private Object[] getBody(HttpClient client, HttpMethod method,
			Map parameters, String encoding) {
		try {
			String body = "";
			body = getBodyToString(client, method, parameters, encoding);
			Timestamp lastModified = new Timestamp(System.currentTimeMillis());
			Header headers[] = method.getResponseHeaders();
			for (int i = 0; i < headers.length; i++) {
				if (!headers[i].getName().equals("Last-Modified"))
					continue;
				try {
					SimpleDateFormat df = new SimpleDateFormat(
							"d MMM yyyy HH:mm:ss 'GMT'", Locale.US);
					lastModified = new Timestamp(df.parse(
							headers[i].getValue().substring(5)).getTime());
				} catch (Exception exception) {
				}
				break;
			}
			return (new Object[] { body, lastModified });
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	private String getBodyWithProxy(String url, String encoding,
			Map parameters, String ip, String port, String methodtype) {
		String body = "";
		System.setProperty("proxySet", "true");
		System.setProperty("proxyHost", ip);
		System.setProperty("proxyPort", port);

		System.setProperty("socksProxySet", "true");
		System.setProperty("socksProxyHost", ip);
		System.setProperty("socksProxyPort", port);
		body = getBodyToString(this.client, getHttpMethod(url, methodtype,
				parameters), parameters, encoding);

		System.setProperty("proxySet", "false");
		System.setProperty("socksProxySet", "false");
		return body;
	}

	private HttpMethod getHttpMethod(String url, String methodtype,
			Map parameters) {
		HttpMethod method = null;
		parameters = putSSORequest(parameters);
		if (methodtype.toLowerCase().equals("get")) {
			url = transTargetByGet(url, parameters);
			method = new GetMethod(url);
		} else if (methodtype.toLowerCase().equals("post")) {
			method = new PostMethod(url);
			addPostParameter(method, parameters);
		}
		return method;
	}

	public String getBodyToString(HttpClientType clienttype) {
		return getBodyToString(this.client, getHttpMethod(clienttype.getUrl(),
				clienttype.getMethodtype(), clienttype.getParameters()),
				clienttype.getParameters(), clienttype.getEncoding());
	}

	public String getBodyToString(String url, String encoding, Map parameters,
			String methodtype) {		
		return getBodyToString(this.client, getHttpMethod(url, methodtype,
				parameters), parameters, encoding);

	}

	public Object[] getBody(HttpClientType clienttype) {
		return getBody(this.client, getHttpMethod(clienttype.getUrl(),
				clienttype.getMethodtype(), clienttype.getParameters()),
				clienttype.getParameters(), clienttype.getEncoding());
	}

	/**
	 * 
	 * @param url
	 * @param encoding
	 * @param methodtype
	 *            <value>get</value> Or <value>post</value>
	 * @return
	 */
	public HttpClientStream getHttpInputStream(String url, Map parameters,
			String methodtype) {
		HttpMethod method = getHttpMethod(url, methodtype, parameters);
		return new HttpClientStream(method, getHttpInputStream(this.client,
				method, parameters));

	}

	/**
	 * 
	 * @param url
	 * @param encoding
	 * @param methodtype
	 *            <value>get</value> Or <value>post</value>
	 * @return
	 */
	public Object[] getBody(String url, String encoding, Map parameters,
			String methodtype) {
		return getBody(this.client, getHttpMethod(url, methodtype, parameters),
				parameters, encoding);
	}

	public String getBodyWithProxy(HttpClientType clienttype) {
		return getBodyWithProxy(clienttype.getUrl(), clienttype.getEncoding(),
				clienttype.getParameters(), clienttype.getIp(), clienttype
						.getPort(), clienttype.getMethodtype());
	}

	private Map putSSORequest(Map map) {
		if (map == null) {
			map = new HashMap();
		}
		map.put("request", "roar-sso-login");
		return map;
	}

	public HttpClient getClient() {
		return client;
	}

	public static void main(String[] args) {
		String[] aa = new String[5];
		aa[0] = "http://fm031.img.xiaonei.com/report.html?rand=43";
		aa[1] = "http://fm421.img.xiaonei.com/report.html?rand=81";
		aa[2] = "http://cl001.file.xiaonei.com/report.html?rand=49";
		aa[3] = "http://fm391.img.xiaonei.com/report.html?rand=0";
		aa[4] = "http://fm441.img.xiaonei.com/report.html?rand=64";
		HttpclientUtil one;
		
		
		for (String bb : aa) {
			one = new HttpclientUtil();
			System.out.println("============"
					+ one.getBodyToString(bb, "gbk", new HashMap(), "get"));

		}

	}
}
