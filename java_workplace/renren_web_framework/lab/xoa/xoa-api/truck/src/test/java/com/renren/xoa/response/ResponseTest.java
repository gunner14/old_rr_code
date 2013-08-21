package com.renren.xoa.response;

import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.junit.Test;
import static org.junit.Assert.*;

public class ResponseTest {

	@Test
	public void testEmptyString() {
		
		DefaultXoaResponse response = new DefaultXoaResponse();
		response.setContent(new byte[]{});
		assertEquals("", response.getContentAsString());
		assertEquals("", response.getContentAs(String.class));
	}
	
	@Test
	public void testGzipedEmptyString() {
		
		DefaultXoaResponse response = new DefaultXoaResponse();
		response.setContent(new byte[]{});
		response.setHeader(HttpHeaders.Names.CONTENT_ENCODING, "gzip");
		assertEquals("", response.getContentAsString());
		assertEquals("", response.getContentAs(String.class));
	}
	
}
