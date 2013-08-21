package com.renren.xoa2.sms;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;


/**
 * 由lantao.liu提供的短信接口
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-8-25 下午06:13:52
 */
public class LanTaoSmsService implements SmsService{
   
    public static final String SERVER_URL = "http://10.3.18.198:3000/asms";
    
    public boolean sendSms(String mobilePhone, String hostport, String serviceName, String state,
            String messageContent) {
        StringBuilder url = new StringBuilder();
        url.append(SERVER_URL);
        
        url.append("?contact=");
        url.append(mobilePhone);
        
        url.append("&host_name=");
        url.append(urlEncode(hostport));
        
        url.append("&service_name=");
        url.append(urlEncode(serviceName));
        
        url.append("&state=");
        url.append(urlEncode(state));
        
        url.append("&message_content=");
        url.append(urlEncode(messageContent));
        
        System.out.println(url.toString());
        
        try {
            HttpURLConnection conn = (HttpURLConnection)new URL(url.toString()).openConnection();
            conn.setDoOutput(false);
            conn.connect();
            
            int code = conn.getResponseCode();
            System.out.println(code);
           
            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            while (reader.readLine() != null);
            conn.disconnect();
            
            
        } catch (MalformedURLException e) {
        	System.out.println("SmsService MalformedURLException");
            // e.printStackTrace();
        } catch (IOException e) {
        	System.out.println("SmsService IOException");
            // e.printStackTrace();
        }
        return false;
    }

    private static String urlEncode(String s) {
        try {
            return URLEncoder.encode(s, "UTF-8").replace(".", "%2E");
            
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException("This should never happen.");
        }
    }
    
    public static void main(String[] args) {        
        LanTaoSmsService svc = new LanTaoSmsService();
        svc.sendSms("18611951882", "10.3.21.180:8188", "entries.xoa.renren.com", "down", "10.3.18.162 port 8080 RR_blog is down 0d 0h 4m 12s Down@19:03");
    }    
}
