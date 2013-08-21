package com.xiaonei.commons.interceptors.accesslog;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

/**
 * Access log
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-10-15 上午11:41:06
 */
public class AccessLogInterceptor extends ControllerInterceptorAdapter {
	
	private static final String DELIMITER = " ";
	
	private ThreadLocal<DateFormat> dateFormats = new ThreadLocal<DateFormat>();
	
	public AccessLogInterceptor() {
		setPriority(29900 + 10);
	}

	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		if (logger.isInfoEnabled()) {
			StringBuilder sb = new StringBuilder(100);
			sb.append("[");
			sb.append(getTimestamp());
			sb.append("]");
			sb.append(DELIMITER);
			
			HttpServletRequest request = inv.getRequest();
			
			//HttpServletResponse response = inv.getResponse();
			sb.append(request.getHeader("host"));
			sb.append(DELIMITER);
			
			sb.append(request.getMethod());
			sb.append(DELIMITER);
			
			sb.append(request.getRequestURI());
			String queryString = request.getQueryString();
			if(queryString != null && queryString.length() != 0) {
				sb.append('?');
				sb.append(queryString);
			}
			
			sb.append(DELIMITER);
			sb.append(request.getRemoteAddr());
			//sb.append(response.getContentType());
			logger.debug(sb);
		}
		return instruction;
	}
	
	private String getTimestamp() {
		return getDateFormat().format(new Date());
	}
	
	private DateFormat getDateFormat() {
		DateFormat df = dateFormats.get();
		if (df == null) {
			df = new SimpleDateFormat("yyyy/MM/dd-HH:mm:ss:SSS");
			dateFormats.set(df);
		}
		return df;
	}
	
	public static void main(String[] args) {
		AccessLogInterceptor app = new AccessLogInterceptor();
		DateFormat df = app.getDateFormat();
		System.out.println(df.format(new Date()));
	}
	
}
