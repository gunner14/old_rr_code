package com.xiaonei.commons.interceptors.ojmapping;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.codehaus.jackson.map.ObjectMapper;
import org.perf4j.StopWatch;

import com.xiaonei.commons.interceptors.performance.StopWatchable;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.instruction.Instruction;

/**
 * 将制定对象转换为json并写入的response中去的指令
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-26 下午06:18:17
 */
public class JsonIntruction implements Instruction, StopWatchable {

    private static Log logger = LogFactory.getLog(JsonIntruction.class);
    
    private static Log switchLogger = LogFactory.getLog(StopWatch.DEFAULT_LOGGER_NAME);
	
    private Object object;
	
	private ObjectMapper objectMapper;
	
	private StopWatch stopWatch;
	
	public JsonIntruction(Object obj, ObjectMapper mapper) {
		this.object = obj;
		this.objectMapper = mapper;
	}
	
	@Override
	public void render(Invocation inv) throws IOException, ServletException,
			Exception {
		
		HttpServletResponse response = inv.getResponse();
		
		String accept = inv.getRequest().getHeader("Accept");
		if (accept != null && accept.contains("application/json")) {
			response.setContentType("application/json");
		} else {	//浏览器调试的时候返回application/json是不能正常显示的
			response.setContentType("text/plain");
		}
		
		if (logger.isDebugEnabled()) {
		    logger.debug("trying to render object: " + object);
		}
		
		response.setCharacterEncoding("UTF-8");
		objectMapper.writeValue(response.getWriter(), object);
		
		
		if (checkPerformance() && stopWatch != null) {
		    
		    if (object != null) {
		        stopWatch.stop("renderToJson(" + object.getClass().getSimpleName() + ")");
		    } else {
		        stopWatch.stop("renderToJson(null)");
		    }
		}
	}

    @Override
    public void setStopWatch(StopWatch stopWatch) {
        this.stopWatch = stopWatch;
    }
    
    /**
     * @return 是否打开性能监测
     */
    private boolean checkPerformance() {
        return switchLogger.isInfoEnabled();
    }
}
