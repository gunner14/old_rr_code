package com.xiaonei.click.business.handler.http;

import java.util.HashMap;
import java.util.Map;

import junit.framework.Assert;

import org.easymock.EasyMock;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.xiaonei.click.BaseTestClass;
import com.xiaonei.click.handler.http.HttpParser;
import com.xiaonei.click.handler.http.ParamParser;

public class TestParamParser extends BaseTestClass {

    HttpRequest mockRequest = null;

    @Before
    public void setUp() throws Exception {
        mockRequest = EasyMock.createMock(HttpRequest.class);

    }

    @Test
    public void testRightParamParse() throws Exception {
        Map<String, String> paramM = getRandomRightParam();
        String param = paramMapToString(paramM, true);
        EasyMock.expect(mockRequest.getUri()).andReturn("/test?" + param);
        EasyMock.expect(mockRequest.getContentLength()).andReturn(0l);
        EasyMock.replay(mockRequest);
        HttpParser p = new ParamParser();
        Map<String, String> ret = p.parse(mockRequest);
        Assert.assertEquals(paramM, ret);
    }

    @Test
    public void testRightBodyParse() throws Exception {
        Map<String, String> paramM = getRandomRightParam();
        String body = paramMapToString(paramM, false);
        EasyMock.expect(mockRequest.getUri()).andReturn("/test");
        EasyMock.expect(mockRequest.getContent()).andReturn(
                ChannelBuffers.copiedBuffer(body, "UTF-8"));
        EasyMock.expect(mockRequest.getContentLength()).andReturn(body.length() + 0l);
        EasyMock.replay(mockRequest);

        HttpParser p = new ParamParser();
        Map<String, String> ret = p.parse(mockRequest);
        Assert.assertEquals(paramM, ret);
    }

    @Test
    public void testParamAndBody() throws Exception {

        Map<String, String> paramM = getRandomRightParam();
        Map<String, String> paramB = getRandomRightParam();
        String param = paramMapToString(paramM, true);
        String body = paramMapToString(paramB, false);
        EasyMock.expect(mockRequest.getUri()).andReturn("/test?" + param);
        EasyMock.expect(mockRequest.getContent()).andReturn(
                ChannelBuffers.copiedBuffer(body, "UTF-8"));
        EasyMock.expect(mockRequest.getContentLength()).andReturn(body.length() + 0l);
        EasyMock.replay(mockRequest);

        HttpParser p = new ParamParser();
        Map<String, String> ret = p.parse(mockRequest);
        paramM.putAll(paramB);
        Assert.assertEquals(paramM, ret);

    }

    @Test
    public void testParamEncode() throws Exception {
        JSONObject jo = new JSONObject();
        for (int i = 3; i < 10; i++) {
            jo.put(getRandomString(), getRandomString());
        }
        jo.put(getRandomString(), getRandomString() + "?" + getRandomString() + "="
                + getRandomString() + "&" + getRandomString() + "=" + getRandomString());
        Map<String, String> paramM = new HashMap<String, String>();
        paramM.put("J", jo.toString());
        String param = paramMapToString(paramM, true);
        EasyMock.expect(mockRequest.getUri()).andReturn("/test?" + param);
        EasyMock.expect(mockRequest.getContentLength()).andReturn(0l);
        EasyMock.replay(mockRequest);
        HttpParser p = new ParamParser();
        Map<String, String> ret = p.parse(mockRequest);
        Assert.assertEquals(paramM, ret);
    }

    @Test
    public void testSpecialParamEncode() throws Exception {
        String param = "J={\\x22ID\\x22:\\x22230795889\\x22,\\x22R\\x22:\\x22http%3A%2F%2Fphoto.renren.com%2Fgetphoto.do%3Fid%3D2538240927%26owner%3D230795889%26curpage%3D3%26t%3D\\x22,\\x22X\\x22:753,\\x22Y\\x22:396,\\x22T\\x22:\\x22img\\x22,\\x22U\\x22:\\x22http://fmn.xnimg.cn/fmn038/20091208/2320/p_tiny_TdC5_4bbb00037cb22d0e.jpg\\x22";
        EasyMock.expect(mockRequest.getUri()).andReturn("/test?" + param);
        EasyMock.expect(mockRequest.getContentLength()).andReturn(0l);
        EasyMock.replay(mockRequest);
        HttpParser p = new ParamParser();
        Map<String, String> ret = p.parse(mockRequest);
        System.out.println(ret);
    }

    @After
    public void tearDown() throws Exception {
        mockRequest = null;
    }
}
