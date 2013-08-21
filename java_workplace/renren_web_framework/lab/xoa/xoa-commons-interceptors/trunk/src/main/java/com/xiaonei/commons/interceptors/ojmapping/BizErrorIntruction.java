package com.xiaonei.commons.interceptors.ojmapping;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletResponse;

import org.codehaus.jackson.map.ObjectMapper;

import com.renren.xoa.commons.bean.XoaBizErrorBean;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.instruction.Instruction;

/**
 * 
 * 用来渲染{@link XoaBizErrorBean}
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-8-2 下午03:50:42
 */
public class BizErrorIntruction implements Instruction {
	
	private XoaBizErrorBean xoaBizErrorBean;
	
	public BizErrorIntruction(XoaBizErrorBean xoaBizErrorBean) {
		this.xoaBizErrorBean = xoaBizErrorBean;
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
		response.setCharacterEncoding("UTF-8");
		response.setStatus(XoaBizErrorBean.STATUS_CODE);
		ObjectMapper mapper = ServerSideObjectMapperFactory.getInstance()
				.getObjectMapper();
		mapper.writeValue(response.getWriter(), xoaBizErrorBean);
	}

}
