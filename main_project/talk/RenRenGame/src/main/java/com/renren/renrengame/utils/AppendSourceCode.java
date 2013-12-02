package com.renren.renrengame.utils;

public class AppendSourceCode {
	public String append(String url, String code) {
		String new_url;
		int pos = url.indexOf("?");
		if (pos == -1)
			new_url = url+"?"+code;
		else
			new_url = url+"&"+code;
		return new_url;
	}
}
