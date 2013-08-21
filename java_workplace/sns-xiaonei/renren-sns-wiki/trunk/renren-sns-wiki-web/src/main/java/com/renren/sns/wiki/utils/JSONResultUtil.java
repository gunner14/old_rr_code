package com.renren.sns.wiki.utils;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

/**
 * 构造WEB端返回JSON数据的工具类
 * 
 * @author yi.li@renren-inc.com since 2012-4-11
 * 
 */
public class JSONResultUtil {

    private static final String DATA_TAG = "data";

    /**
     * 将数据转化为统一JSON输出格式格式为{code:xx, msg:"xxxx", data:json }
     * 当data为String时，直接将data放入data中，也就是说如果String也是Json格式，
     * 转化完之后并会当做JSON对象而是作为字符串存在
     * 
     * @param code
     * @param msg
     * @param data
     * @return
     */
    public static String buildJSONResult(int code, String msg, Object data) {
        JSONObject result = new JSONObject();
        result.put("code", code);
        result.put("msg", msg);
        if (null == data) {
            result.put(DATA_TAG, "");
        } else if (data instanceof String) {
            result.put(DATA_TAG, data);
        } else if (data instanceof org.json.JSONObject) { // 兼容org.json.JSONObject
            result.put(DATA_TAG, data.toString());
        } else if (data instanceof List<?>) {
            JSONArray jsonArray = JSONArray.fromObject(data);
            result.put(DATA_TAG, jsonArray);
        } else {
            JSONObject json = JSONObject.fromObject(data);
            result.put(DATA_TAG, json);
        }

        return result.toString();
    }

    public static void main(String args[]) {
        Integer a = 1;
        Map<String, Object> b = new HashMap<String, Object>();
        b.put("hh", "oo");
        b.put("h33", 123);
        System.out.println(buildJSONResult(1, "test", "{a:1,b:c}"));
        System.out.println(buildJSONResult(1, "test", a));
        System.out.println(buildJSONResult(1, "test", b));
    }
}
