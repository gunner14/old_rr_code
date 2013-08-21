package com.xiaonei.commons.interceptors.ojmapping;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.instruction.Instruction;

/**
 * 通过ProtocolBuffers来进行返回值编码的Intruction
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-5-7 上午11:30:05
 */
public class ProtocolBuffersIntruction implements Instruction {

	private Object object;

	public ProtocolBuffersIntruction(Object obj) {
		this.object = obj;
	}

	@Override
	public void render(Invocation inv) throws IOException, ServletException,
			Exception {

		if (object instanceof com.google.protobuf.GeneratedMessage) {
			HttpServletResponse response = inv.getResponse();
			response.setContentType("application/protoc-buff");
			((com.google.protobuf.GeneratedMessage) object).writeTo(response
					.getOutputStream());
		} else {
			throw new RuntimeException(object.getClass().getName()
					+ " not instance of com.google.protobuf.GeneratedMessage");
		}
	}
}
