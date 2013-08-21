package com.renren.xoa.demo.controllers;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.BlogFactory;

public class TextController {

	//@Autowired
	//private HostHolder hostHolder;
	
	private byte[] bytes;
	
	private String[] ss = new String[128];
	
	public TextController() {
		
		Blog[] blogs = BlogFactory.getInstance().getDataBySize(200);
		ObjectMapper mapper = new ObjectMapper();
		try {
			
			ByteArrayOutputStream bo = new ByteArrayOutputStream(25600);
			mapper.writeValue(bo, blogs);
			bytes = bo.toByteArray();
			bytes[0] = '@';
		} catch (JsonGenerationException e) {
			e.printStackTrace();
		} catch (JsonMappingException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		for (int i = 0; i < ss.length; i++) {
			try {
				ss[i] = new String(bytes, 0, i * 1000 + 1, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}
	}
	
	@Get("/{k}/{delay}")
	@Post("/{k}/{delay}")
	public Object index(Invocation inv, @Param("k")Integer k, @Param("delay")Integer delay) {
		
		if (delay > 0) {
			try {
				Thread.sleep(delay);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		return ss[k];
	}
	
}
