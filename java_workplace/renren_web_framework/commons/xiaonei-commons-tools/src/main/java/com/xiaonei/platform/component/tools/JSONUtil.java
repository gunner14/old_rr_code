package com.xiaonei.platform.component.tools;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;

public class JSONUtil {

    public static Map<String, String> jsonToMap(String json) throws ParseException {
	if (json == null) {
	    return null;
	}
	JSONObject jo = new JSONObject(json);
	Map<String, String> map = new HashMap<String, String>();
	for (Iterator keys = jo.keys(); keys.hasNext();) {
	    String key = (String) keys.next();
	    map.put(key, jo.get(key).toString());
	}
	return map;
    }

    /**
     * @param json
     *            将json数组的字符串转化成以map为元素的list,根据map的equals()去重
     * @return
     */
    public static List jsonArrayToMapList(String json) {
	if (json == null) {
	    return null;
	}
	List list = null;
	try {
	    JSONArray ja = null;
	    if (json.startsWith("[") && json.endsWith("]")) {
		ja = new JSONArray(json);
	    } else {
		ja = new JSONArray("[" + json + "]");
	    }
	    list = new ArrayList();
	    for (int i = 0; i < ja.length(); i++) {
		JSONObject o = ja.getJSONObject(i);
		Map map = jsonToMap(o.toString());
		if (!list.contains(map)) {
		    list.add(map);
		}
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
	return list;
    }

    public static String quote(String string) {
	if (string == null || string.length() == 0)
	    return "\"\"";
	char c = '\0';
	int len = string.length();
	StringBuffer sb = new StringBuffer(len + 4);
	sb.append('"');
	for (int i = 0; i < len; i++) {
	    char b = c;
	    c = string.charAt(i);
	    switch (c) {
	    case 34: // '"'
	    case 92: // '\\'
		sb.append('\\');
		sb.append(c);
		break;

	    case 47: // '/'
		if (b == '<')
		    sb.append('\\');
		sb.append(c);
		break;

	    case 8: // '\b'
		sb.append("\\b");
		break;

	    case 9: // '\t'
		sb.append("\\t");
		break;

	    case 10: // '\n'
		sb.append("\\n");
		break;

	    case 12: // '\f'
		sb.append("\\f");
		break;

	    case 13: // '\r'
		sb.append("\\r");
		break;

	    default:
		if (c < ' ') {
		    String t = "000" + Integer.toHexString(c);
		    sb.append("\\u").append(t.substring(t.length() - 4));
		} else {
		    sb.append(c);
		}
		break;
	    }
	}

	sb.append('"');
	return sb.toString();
    }

    public static String MapListToJSON(List<Map> mapList) {
	StringBuffer sb = new StringBuffer();
	for (int i = 0; i < mapList.size(); i++) {
	    Map map = mapList.get(i);
	    sb.append(mapToJSON(map));
	}
	return sb.toString();
    }

    public static String mapToJSON(Map map) {
	return new JSONObject(map).toString();
    }
}
