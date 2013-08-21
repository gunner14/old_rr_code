package com.xiaonei.platform.scheduleTask;
import java.io.IOException;
import java.io.InputStream;

import org.apache.commons.httpclient.HttpMethod;

public class HttpClientStream {
	
	private HttpMethod httpMethod;

	private InputStream httpInputStream;

	/**
	 * 
	 * @
	 * */
	
	public HttpClientStream(HttpMethod _httpMethod, InputStream _inputStream) {
		this.httpInputStream = _inputStream;
		this.httpMethod = _httpMethod;
	}

	/**
	 * 
	 * @return httpMethod
	 */
	public HttpMethod httpMethod() {
		return this.httpMethod;
	}

	/**
	 * 
	 * @return httpInputStream
	 */
	public InputStream getHttpInputStream() {
		return httpInputStream;
	}

	/**
	 * 
	 * @Close httpInputStream httpMethod
	 */
	public void close() {
		try {
			this.httpInputStream.close();
			this.httpMethod.releaseConnection();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
