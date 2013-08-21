package com.dodoyo.invite.httpservice.test;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class PostHttpRequest {
    private String urlStr;
    private URL url;
    private HttpURLConnection url_con;
    private String response_content;
    private String param;
    private int http_status = -1;

    /**
     * use this struct, need call method setUrlStr and setParam
     */
    PostHttpRequest() {
    }

    public PostHttpRequest(String req_url, String post_data) {
        this.urlStr = req_url;
        this.param = post_data;
    }

    public void setUrlStr(String urlStr) {
        this.urlStr = urlStr;
    }

    public String getResponse_content() {
        return response_content;
    }

    public int getHttp_status() {
        return this.http_status;
    }

    private void setHttp_status(int status) {
        this.http_status = status;
    }

    private void setResponse_content(String response_content) {
        this.response_content = response_content;
    }

    public void setParam(String param) {
        this.param = param;
    }

    public void send_url() {
        System.out.println("a post http request execute");
        try {
            url = new URL(urlStr);
            url_con = (HttpURLConnection) url.openConnection();
            url_con.setRequestMethod("POST");
            //new add
            url_con.setRequestProperty("Header-Param", "test3");
            url_con.setRequestProperty("User-Agent", "IE6");
            url_con.setDoOutput(true);
            url_con.getOutputStream().write(param.getBytes());
            url_con.getOutputStream().flush();
            url_con.getOutputStream().close();


            InputStream in = url_con.getInputStream();
            BufferedReader rd = new BufferedReader(new InputStreamReader(in, "UTF-8"));
            StringBuilder tempStr = new StringBuilder();
            String begin_line = rd.readLine();
            if (begin_line != null)
                tempStr.append(begin_line);
            while (begin_line != null) {
                begin_line = rd.readLine();
                if (begin_line != null)
                    tempStr.append(begin_line);
            }
            setResponse_content(new String(tempStr));
            setHttp_status(url_con.getResponseCode());
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        finally {
            if (url_con != null)
                url_con.disconnect();
        }
    }

    public static void main(String[] args) {
        PostHttpRequest pr = new PostHttpRequest();
//    	pr.setUrlStr("http://192.168.0.63/workteny/TestAction.do");
        pr.setUrlStr("http://login.kaixin.com/Login.do");
        String recv_str_data = "";
        recv_str_data += "<user_search_req>";
        recv_str_data += "<keyword>gao</keyword>";
        recv_str_data += "<page_no>1</page_no>";
        recv_str_data += "<num_per_page>30</num_per_page>";
        recv_str_data += "</user_search_req>";
        recv_str_data = "email=hongliang.gao@opi-corp.com&password=passghl";
        pr.setParam(recv_str_data);
        pr.send_url();
        pr.send_url();
        System.out.println(pr.getResponse_content());
        System.out.println(pr.getHttp_status());
    }
}