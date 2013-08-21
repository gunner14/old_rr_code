package com.xiaonei.commons.interceptors.ojmapping;

import java.io.IOException;
import java.io.ObjectOutputStream;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.instruction.Instruction;

/**
 * 使用Java序列化机制来对返回值编码的 Intruction
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-5-7 上午11:25:45
 */
public class SerializationIntruction implements Instruction{

	private Object object;
	
	public SerializationIntruction(Object obj) {
		this.object = obj;
	}
	
	@Override
	public void render(Invocation inv) throws IOException, ServletException,
			Exception {
		HttpServletResponse response = inv.getResponse();
		response.setContentType("application/serializable");
		ObjectOutputStream os = new ObjectOutputStream(response.getOutputStream());
		os.writeObject(object);
		os.flush();
		os.close();
	}
}
