package com.xiaonei.commons.interceptors.performance;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.perf4j.StopWatch;
import org.perf4j.commonslog.CommonsLogStopWatch;

/**
 * 用于性能统计。
 * 使用的第三方组件perf4j
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-10-15 上午11:35:19
 */
public class PerformanceInterceptor extends ControllerInterceptorAdapter {
	
    private static final int QUERY_STRING_MAX_LENGTH = 20;
    
    private static final long SLOW_THRESHOLD = 100; //慢请求的阈值
    
	private Log switchLogger = LogFactory.getLog(StopWatch.DEFAULT_LOGGER_NAME);
	
	public PerformanceInterceptor() {
		setPriority(30000);
	}
	
	@Override
	public Object before(Invocation inv) throws Exception {
		
		if (checkPerformance()) {
			StopWatch stopWatch = new CommonsLogStopWatch("xoa");
			inv.addModel("stopWatch", stopWatch);
		}
		return true;
	}

	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		if (checkPerformance()) {
			StopWatch stopWatch = (StopWatch)inv.getModel("stopWatch");
			if (stopWatch != null) {
				stopWatch.lap(generateOperatonIdendifier(inv, stopWatch.getElapsedTime()));
			}
			
			if (instruction instanceof StopWatchable) {
	            StopWatchable sw = (StopWatchable)instruction;
	            sw.setStopWatch(stopWatch);
	        }
		}
		return instruction;
	}
	
	private String generateOperatonIdendifier(Invocation inv, long exeTime) {
		HttpServletRequest request = inv.getRequest();
		StringBuilder sb = new StringBuilder(64);
		
		//方法
		String method;
		if (request.getParameter("_method") != null) {    //有可能是通过参数来指定的方法
		    method = request.getParameter("_method");
		} else {
		    method = request.getMethod();
		}
		sb.append(method);
		
		sb.append('|');
		
		//URI
		if (switchLogger.isTraceEnabled()) {	//如果是trace级别，统计到具体的URI
			sb.append(request.getRequestURI());
			
			String queryString = request.getQueryString();
			if (queryString != null && queryString.length() > 0) {
			    sb.append("?");
			    
			    if (queryString.length() > QUERY_STRING_MAX_LENGTH) {
			        
			        //queryString太长的话要截取
			        sb.append(queryString.substring(0, QUERY_STRING_MAX_LENGTH));
			        sb.append("...");
			    } else {
			        sb.append(queryString);
			    }
			}
		} else {	//按URI pattern匹配，方便汇总
			sb.append(inv.getResourceId());
		}
		
		//客户端IP
		if (switchLogger.isDebugEnabled()) {
			sb.append('|');
			String clientIp = inv.getRequest().getRemoteAddr();
			sb.append(clientIp);
			sb.append('|');
			sb.append(request.getHeader("User-Agent"));
		} else {  //debug以上级别的时候统计slow request
		    if (exeTime >= SLOW_THRESHOLD) {
		        sb.append("|SLOW");
		    }
		}
		return sb.toString();
	}
	
	/**
	 * @return 是否打开性能监测
	 */
	private boolean checkPerformance() {
		return switchLogger.isInfoEnabled();
	}
}
