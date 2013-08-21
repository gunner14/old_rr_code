package com.renren.xoa.commons.test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;


public class TextFactory {

	private static TextFactory instance = new TextFactory();
	
	public static TextFactory getInstance() {
		return instance;
	}
	
	private TextFactory() {
		pepareDatas();
	}

	private byte[] bytes;
	
	private String[] ss = new String[128];
	
	private void pepareDatas() {
		Blog[] blogs = BlogFactory.getInstance().getDataBySize(200);
		ObjectMapper mapper = new ObjectMapper();
		try {
			
			ByteArrayOutputStream boa = new ByteArrayOutputStream();
			mapper.writeValue(boa, blogs);
			bytes = boa.toByteArray();
			bytes[0] = '@';
		} catch (JsonGenerationException e) {
			e.printStackTrace();
		} catch (JsonMappingException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		for (int i = 0; i < ss.length; i++) {
			ss[i] = new String(bytes, 0, i * 1000 + 1);
		}
	}
	
	public String getString(int k) {
		return ss[k];
	}

	public static void main(String[] args) {
		String s = TextFactory.getInstance().getString(1);
		System.out.println(s);
	}
	
}

