package com.xiaonei.commons.interceptors.ojmapping;

import java.io.Serializable;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.instruction.Instruction;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.codehaus.jackson.map.ObjectMapper;

import com.renren.xoa.commons.bean.XoaBizErrorBean;

/**
 * 将返回对象进行编码以传输的Interceptor
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-5-7 上午11:25:04
 */
public class TransferEncodeInterceptor extends ControllerInterceptorAdapter {

    private Log logger = LogFactory.getLog(this.getClass());
    
	public TransferEncodeInterceptor() {
		setPriority(19900);
	}
	
	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {

	    if (logger.isDebugEnabled()) {
	        logger.debug("TransferEncodeInterceptor.after -> ENTER");
	    }
	    
	    if (instruction == null) {
	        if (logger.isDebugEnabled()) {
	            logger.debug("instruction null");
	        }
			return null;
		}
		
		// Instruction类型不处理
		if (instruction instanceof Instruction) {
		    if (logger.isDebugEnabled()) {
                logger.debug("Instruction type " + instruction.getClass() + ": " + instruction);
            }
			return instruction;
		}

		// String类型不处理
		if (instruction instanceof String) {
		    if (logger.isDebugEnabled()) {
		        logger.debug("String type, value:" + instruction);
		    }
			return instruction;
		}
		
		
		if (instruction instanceof XoaBizErrorBean) {
			return new BizErrorIntruction((XoaBizErrorBean)instruction);
		}
		
		String accept = inv.getRequest().getHeader("Accept");
		
		ObjectMapper objectMapper = ServerSideObjectMapperFactory.getInstance()
				.getObjectMapper();
		if (accept != null) {
			
			//优先使用json格式
			if (accept.contains("application/json")) {
				return new JsonIntruction(instruction, objectMapper);
			}
			
			//如果支持序列化就走序列化机制
			if (instruction instanceof Serializable	
					&& accept.contains("application/serializable")) {
				return new SerializationIntruction(instruction);
			}
			
			//如果支持protoc-buff就走protoc-buff
			if (instruction instanceof com.google.protobuf.GeneratedMessage
				&& accept.contains("application/protoc-buff")) {
				return new ProtocolBuffersIntruction((com.google.protobuf.GeneratedMessage)instruction);
			}
		}
		
		//默认情况走Json
		return new JsonIntruction(instruction, objectMapper);
	}
}
