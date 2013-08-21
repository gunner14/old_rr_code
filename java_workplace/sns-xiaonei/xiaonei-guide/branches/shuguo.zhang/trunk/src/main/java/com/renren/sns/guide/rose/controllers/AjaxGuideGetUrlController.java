package com.renren.sns.guide.rose.controllers;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-16 下午04:49:24
 */
public class AjaxGuideGetUrlController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        String result = "";
        String ip_url = inv.getRequest().getParameter("ip_url");
        String address = "";
        try {
            URL url = new URL("http://www.ip.cn/getip.php?action=queryip&ip_url=" + ip_url);
            BufferedReader in = new BufferedReader(new InputStreamReader(url.openStream(), "GBK"));
            //使用openStream得到一输入流并由此构造一个BufferedReader对象
            String inputLine;
            //从输入流不断的读数据，直到读完为止
            while ((inputLine = in.readLine()) != null) {
                if (inputLine.trim().length() != 0) {
                    result += inputLine + "\n";
                }
            }
            String[] datas = result.split("</span>");
            if (datas != null && datas.length > 1) {
                address = datas[1];
            }
            in.close();
            return "@" + address;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "@" + address;
    }

    public static void main(String[] args) {
        String result = "";
        String ip_url = "125.120.193.72";
        try {
            URL url = new URL("http://www.ip.cn/getip.php?action=queryip&ip_url=" + ip_url);
            BufferedReader in = new BufferedReader(new InputStreamReader(url.openStream(), "GBK"));
            //使用openStream得到一输入流并由此构造一个BufferedReader对象
            String inputLine;
            //从输入流不断的读数据，直到读完为止
            while ((inputLine = in.readLine()) != null) {
                if (inputLine.trim().length() != 0) {
                    result += inputLine + "\n";
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
