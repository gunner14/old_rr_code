package com.renren.sns.wiki.controllers.adminwiki;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;
import org.apache.commons.httpclient.protocol.Protocol;
import org.apache.commons.httpclient.protocol.SSLProtocolSocketFactory;

/**
 * 用于验票
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-3
 * 
 */
public class PassportUtils {

    @SuppressWarnings("deprecation")
    public static String getPassport(String ticket) {
        Protocol myhttps = new Protocol("https", new SSLProtocolSocketFactory(), 443);
        String url = "/verify.php?t=" + ticket;
        String result = "";
        HttpClient httpClient = new HttpClient();
        GetMethod getMethod = new GetMethod(url);
        getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
                new DefaultHttpMethodRetryHandler());
        int statusCode = 1000;// 默认代码Connection refused
        try {
            httpClient.getHostConfiguration().setHost("passport.no.opi-corp.com", 443, myhttps);
            getMethod.addRequestHeader("Referer",
                    "http://page.renren.com/adminpage/superAdminLogin/get"); // 跳转的URL
            statusCode = httpClient.executeMethod(getMethod);
            if (statusCode == HttpStatus.SC_OK) {
                result = getMethod.getResponseBodyAsString();
            }
            return result;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            getMethod.releaseConnection(); // 释放连接
        }
        return null;
    }
}
