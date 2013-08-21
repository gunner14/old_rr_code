package com.xiaonei.tools.monitor.command;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.Random;

import com.xiaonei.tools.monitor.db.Connect;
import com.xiaonei.tools.monitor.entity.HttpErrEntity;

public class HttpErrDBCommand {

    Random random = new Random();

    //    public static final SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss");
    private final int HTTPERRARRLEN = 6;//record split(" ")以后的长度。

    private final int URLNORMALLEN = 50;

    private final int URLMAXLEN = 100;

    private final String SQL_PREFIX = "insert into http_error (happen_time,err_type,UIP,err_server,pre_domain,pre_url,cur_domain,cur_url,pre_ful_url,cur_ful_url,key_word) values ";

    public void excute(HttpErrEntity httpErrEntity, LinkedList<HttpErrEntity> httpErrList)
            throws SQLException {

        //        HttpErrEntity httpErrEntity = getHttpErrEntity(items);
        httpErrList.add(httpErrEntity);
        if (httpErrList.size() < 100) {} else {
            saveToDB(httpErrList);
            httpErrList.clear();
        }
    }

    public void saveToDB(LinkedList<HttpErrEntity> httpErrList) throws SQLException {
        String sql = getSql(httpErrList);
        //        System.out.println(sql);
        Connect.insert(sql);
    }

    //    public String[] getRecordArr(String record) {
    //        return record.split(" ");
    //
    //    }

    public HttpErrEntity getHttpErrEntity(String[] items) {
        if (!validateLog(items)) {
            //          System.out.println("validatelog err" + items[4] + "-----" + items[5]);
            return null;
        }
        HttpErrEntity httpErrEntity = new HttpErrEntity();
        httpErrEntity.setHappernTime(getHappenTime(items));
        httpErrEntity.setErrorType(getErrType(items));
        httpErrEntity.setUIP(getUIP(items));
        httpErrEntity.setErrorServer(getErrServer(items));
        httpErrEntity.setPreDomain(getPreDomain(items));
        httpErrEntity.setPreUrl(getPreUrl(items));
        httpErrEntity.setPreFulUrl(getPreFulUrl(items));
        httpErrEntity.setCurDomain(getCurDomain(items));
        httpErrEntity.setCurUrl(getCurUrl(items));
        httpErrEntity.setCurFulUrl(getcurFulUrl(items));
        httpErrEntity.setKeyWord(getKeyWord(httpErrEntity.getCurDomain()));
        return httpErrEntity;
    }

    public String getHappenTime(String[] items) {
        return changeTimeStrPattern(items[0].trim());
    }

    public String getPreFulUrl(String[] items) {
        String preFulUrl = items[4];
        if ("-".equals(preFulUrl) || "null".equals(preFulUrl) || "".equals(preFulUrl)) {
            return "null";
        }
        if (preFulUrl.endsWith("\\")) return preFulUrl + "\\";
        preFulUrl=  cutEndInvalidChar(preFulUrl);
        return preFulUrl;
    }

    public String getcurFulUrl(String[] items) {
        String curFulUrl = items[5];
        if ("-".equals(curFulUrl) || "null".equals(curFulUrl) || "".equals(curFulUrl)) {
            return "null";
        }
        if (curFulUrl.endsWith("\\")) {
            return curFulUrl + "\\";
        }
        curFulUrl=  cutEndInvalidChar(curFulUrl);
        return curFulUrl;
    }

    public String changeTimeStrPattern(String timeStr) {
        String[] items = timeStr.split("-");
        String correctTimeStr = "";
        int itemsLen = items.length;
        for (int i = 0; i < itemsLen - 1; i++) {
            correctTimeStr += items[i] + "-";
        }
        correctTimeStr = correctTimeStr.substring(0, correctTimeStr.length() - 1);
        correctTimeStr += " " + items[itemsLen - 1];
        return correctTimeStr;
    }

    public String getErrType(String[] items) {
        return items[1].trim();
    }

    public String getUIP(String[] items) {
        return items[2].trim();
    }

    public String getErrServer(String[] items) {
        String serverName = items[3].trim();
        if ("-".equals(serverName)) {
            serverName = "null";
        }
        return serverName;
    }

    public String getPreDomain(String[] items) { //.do.php ..etc
        String preDomain = items[4];
        if ("-".equals(preDomain) || "null".equals(preDomain) || "".equals(preDomain)) {
            return "null";
        }
        preDomain = getDomain(preDomain);
        preDomain=  cutEndInvalidChar(preDomain);
        return preDomain;
    }

    public String getCurDomain(String[] items) { //.do.php ..etc
        String curUrl = items[5];
        if ("-".equals(curUrl) || "null".equals(curUrl) || "".equals(curUrl)) {
            return "null";
        }
        curUrl = getDomain(curUrl);
        curUrl=  cutEndInvalidChar(curUrl);
        return curUrl;
    }

    public String getPreUrl(String[] items) {
        String preUrl = items[4];
        if ("-".equals(preUrl) || "null".equals(preUrl) || "".equals(preUrl)) {
            return "null";
        }
        preUrl = getUrl(preUrl);
//        if (preUrl.endsWith("/")) {
//            return preUrl.substring(0, preUrl.length() - 1);
//        }
        preUrl=  cutEndInvalidChar(preUrl);
        return preUrl;
    }

    public String getCurUrl(String[] items) {
        String curUrl = items[5];
        if ("-".equals(curUrl) || "null".equals(curUrl) || "".equals(curUrl)) {
            return "null";
        }
        curUrl = getUrl(curUrl);
//        if (curUrl.endsWith("/")) {
//            return curUrl.substring(0, curUrl.length() - 1);
//        }
        curUrl=  cutEndInvalidChar(curUrl);
        return curUrl;
    }
    
    public String cutEndInvalidChar(String url ){
    	if (url.endsWith("/")||url.endsWith("\\")) {
            return cutEndInvalidChar (url.substring(0, url.length() - 1));
        }
    	return url; 
    }

    /**
     * check if has evil code .
     * 
     * @param arr
     * @return
     */
    public boolean hasEvilCode(char[] arr) {
        for (char c : arr) {
            if (c > 40869) {
                return true;
            }
        }
        return false;
    }

    public String getDomain(String url) {
        int index = url.indexOf("/", 8);
        String domain = null;
        if (index < 1) {
            domain = url.substring(0, url.length());
        } else if (index > 8) {
            domain = url.substring(7, index);
        }
        domain =cutEndInvalidChar(domain);
        return domain;
    }

    public String getKeyWord(String url) {
        //        if (url==null||"".equals(url))
        //            return null;
        //        String keyWord="";
        if (url.contains("renren")) {
            return "renren";
        } else if (url.contains("kaixin")) {
            return "kaixin";
        } else if (url.contains("xiaonei")) {
            return "xiaonei";
        }
        return null;
    }

    public String getUrl(String url) {
        int actioIndex = url.indexOf(".do");
        int jspIndex = url.indexOf(".jsp");
        int phpIndex = url.indexOf(".php");
        int index = url.indexOf("?");
        int htmIndex = url.indexOf(".htm");
        int htmlIndex = url.indexOf(".html");
        LinkedList<Integer> values = new LinkedList<Integer>();
        if (actioIndex > 1) values.add(actioIndex);
        if (jspIndex > 1) values.add(jspIndex);
        if (phpIndex > 1) values.add(phpIndex);
        if (index > 1) values.add(index);
        if (htmIndex > 1) values.add(htmIndex);
        if (htmlIndex > 1) values.add(htmlIndex);
        String urlResult = "";
        if (values.size() > 0) {
            int smallIndex = values.get(0);
            for (int i : values) {
                if (i < smallIndex) {
                    smallIndex = i;
                }
            }
            if (smallIndex > URLMAXLEN) {
                return urlResult;
            }
            // boolean hasevilcode = url.contains("'");
            if (smallIndex == actioIndex) {
                // .do
                urlResult = url.substring(0, smallIndex + 3);
                return urlResult;
            }
            if (smallIndex == jspIndex) {
                // .jsp
                urlResult = url.substring(0, smallIndex + 4);
                return urlResult;
            }
            if (smallIndex == phpIndex) {
                // .php
                urlResult = url.substring(0, smallIndex + 4);
                return urlResult;
            }
            if (smallIndex == index) {
                urlResult = url.substring(0, smallIndex);
                return urlResult;
            }
            if (smallIndex == htmlIndex) {
                urlResult = url.substring(0, htmlIndex + 5);
                return urlResult;
            }
            if (smallIndex == htmIndex) {
                urlResult = url.substring(0, htmIndex + 4);
                return urlResult;
            }
        }
        if (url.trim().equals("null")) {
            return urlResult = url;
        }
        if (values.size() == 0) {
            if (url.length() < URLNORMALLEN) {
                if (url.endsWith(".com") || url.endsWith(".com/")) return url;
            }
        }
        return urlResult;
    }

    public String getSql(LinkedList<HttpErrEntity> httpErrList) {
        StringBuilder sb = new StringBuilder();
        sb.append(SQL_PREFIX);
        for (HttpErrEntity httpErrEntity : httpErrList) {
            sb.append("('");
            sb.append(httpErrEntity.getHappernTime());
            sb.append("','");
            sb.append(httpErrEntity.getErrorType());
            sb.append("','");
            sb.append(httpErrEntity.getUIP());
            sb.append("','");
            sb.append(httpErrEntity.getErrorServer());
            sb.append("','");
            sb.append(httpErrEntity.getPreDomain());
            sb.append("','");
            sb.append(httpErrEntity.getPreUrl());
            sb.append("','");
            sb.append(httpErrEntity.getCurDomain());
            sb.append("','");
            sb.append(httpErrEntity.getCurUrl());
            sb.append("','");
            sb.append(httpErrEntity.getPreFulUrl());
            sb.append("','");
            sb.append(httpErrEntity.getCurFulUrl());
            sb.append("','");
            sb.append(httpErrEntity.getKeyWord());
            sb.append("')");
            sb.append(",");
        }
        return sb.substring(0, sb.length() - 1);
    }

    public boolean validateLog(String[] items) {
        if (items.length != HTTPERRARRLEN) {
            //            System.out.println("Length is evil:" + logArr.length);
            return false;
        }
        String url = getUrl(items[5]);
        String referer = getUrl(items[4]);
        if (url.contains("'") || url.contains("%") || referer.contains("'")
                || referer.contains("%")) {
            //            System.out.println("contains Evil words:" + url + "--------" + referer);
            return false;
        }
        if ((!url.contains("renren.com")) && (!url.contains("xiaonei.com"))
                && (!url.contains("kaixin.com"))) {
            //            System.out.println("Do not contains xiaonei.com or kaixin.com:" + url);
            return false;
        }
        if (url.equals("") || url.equals("-")) {
            //            System.out.println("url and referer are empty string or - or null:" + url);
            return false;
        }
        char[] urlArr = url.toCharArray();
        char[] refererArr = referer.toCharArray();
        if (hasEvilCode(urlArr) || hasEvilCode(refererArr)) {
            //            System.out.println("has evil code:" + url);
            return false;
        }
        if (items[0].length() != 19) {
            //            System.out.println("time is not correct" + logArr[0]);
            return false;
        }
        String pre_Domain = getPreDomain(items);
        if (pre_Domain.length() > 198) {
            return false;
        }
        String errType = getErrType(items);
        if (!allNumbers(errType)) {
            return false;
        }

        String preFulUrl = getPreFulUrl(items);
        if (preFulUrl.length() > 999) return false;
        if (hasEvilCode(preFulUrl.toCharArray())) return false;
        if (preFulUrl.contains("'")) return false;

        String curFulUrl = getcurFulUrl(items);
        if (curFulUrl.length() > 999) return false;
        if (curFulUrl.contains("'")) return false;
        if (hasEvilCode(curFulUrl.toCharArray())) return false;
        return true;
    }

    public boolean allNumbers(String str) {
        char[] strArr = str.toCharArray();
        for (int i = 0; i < strArr.length; i++) {
            if (strArr[i] < '0' || strArr[i] > '9') {
                return false;
            }
        }
        return true;
    }

    public static String getDBLatestTime() throws SQLException {
        String sql = "select happen_time from http_error order by happen_time desc limit 1";
        ResultSet set = Connect.querySet(sql);
        while (set.next()) {
            return set.getString("happen_time");
        }
        return null;
    }

}
