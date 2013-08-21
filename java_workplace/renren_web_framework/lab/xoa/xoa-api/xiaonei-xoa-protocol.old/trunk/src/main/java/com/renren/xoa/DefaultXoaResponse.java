package com.renren.xoa;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.GZIPInputStream;

import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

/**
 * XoaResponse的默认实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午05:37:55
 */
public class DefaultXoaResponse implements XoaResponse {

	private int statusCode;
	
	private Map<String, String> headers;
	
	private byte[] content;

	@Override
	public Object getContent(Class<?> klass) {
		return null;
	}

	public void setHeader(String headerName, String headerValue) {
		if (headers == null) {
			headers = new HashMap<String, String>();
		}
		headers.put(headerName, headerValue);
	}
	
	@Override
	public String getHeader(String headerName) {
		if (headers == null) {
			return null;
		}
		return headers.get(headerName);
	}

	public void setStatusCode(int statusCode) { 
		this.statusCode = statusCode;
	}
	
	@Override
	public int getStatusCode() {
		return statusCode;
	}
	
	@Override
	public JSONObject getContentAsJson() {
		try {
			JSONTokener jt = new JSONTokener(new BufferedReader(
					new InputStreamReader(getContentAsInputStream())));
			return new JSONObject(jt);
		} catch (JSONException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public String getContentEncoding() {
		return this.getHeader(HttpHeaders.Names.CONTENT_ENCODING);
	}

	/*public void setContentEncoding(String contentEncoding) {
		this.contentEncoding = contentEncoding;
	}*/

	public void setContent(byte[] content) {
		this.content = content;
	}

	@Override
	public <T> T getContent(JOMapper<T> mapper) {
		return mapper.map(getContentAsJson());
	}

	@Override
	public String getContentAsString() {

		if (isGzipCompressed()) {
			try {
				BufferedReader reader = new BufferedReader(new InputStreamReader(
						getContentAsGZIPInputStream()));
				StringWriter sw = new StringWriter(content.length * 2);
				char[] buff = new char[512];
				int len;
				while ((len = reader.read(buff)) != -1) {
					sw.write(buff, 0, len);
				}
				return sw.toString();
			} catch (IOException e) {
				e.printStackTrace();
			}
			return null;
		} else {
			return new String(content);
		}
	}
	
	private InputStream getContentAsInputStream() throws IOException {
		if (isGzipCompressed()) {
			return getContentAsGZIPInputStream();
		} else {
			return new ByteArrayInputStream(content);
		}
	}
	
	private GZIPInputStream getContentAsGZIPInputStream() throws IOException {
		return new GZIPInputStream(new ByteArrayInputStream(content));
	}
	
	private boolean isGzipCompressed() {
		return "gzip".equals(getContentEncoding());
	}
	
	public static void main(String[] args) {

		JOMapper<String> mapper = new JOMapper<String>() {

			@Override
			public String map(JSONObject json) {
				return "haha";
			}
		};

		DefaultXoaResponse res = new DefaultXoaResponse();

		res.setContent("{}".getBytes());

		String ret = res.getContent(mapper);
		System.out.println(ret);
	}

	

}
