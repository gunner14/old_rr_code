package com.renren.renrengame.utils;

public class ReturnInfoGenerator {

	public String getTrue()
	{
		return "true";
	}
	
	public String getFalse(String reason)
	{
		return "false,message:"+reason;
	}
}
