package com.xiaonei.platform.component.tools;

/**
 * @author Administrator
 *	对spam进行过滤后，返回一个bool值标志是否可以发送，返回一个msg提示用户,并把过滤后的结果放在subject和body中
 */
public class Result {
	
	public static final String BLOCK="请不要发布政治敏感内容、色情内容、商业广告或其他不恰当内容 :D";
	public static final String SAME_COUNT_OVER="你短时间内发表了太多的相同内容,请歇一会再试 :)";
	public static final String FAST_COUNT_OVER="你短时间内发表的内容太快了,请歇一会再试 :)";
	public static final String SPAM="请不要发布政治敏感内容、色情内容、商业广告或其他不恰当内容 :D";
	public static final String MAY_BE_BLOCK="你所发布可能含有违禁内容 :(";
	public static final String SAME="你短时间内发表了太多的相同内容";
	public static final String FAST="你短时间内发表的内容太快了";
	public static final String TITLE_NULL="标题不能为空";
	public static final String BODY_NULL="内容不能为空";
	
	private boolean validate;
	private String msg;
	private String subject;
	private String body;
	
	public Result(boolean r){
		this.validate=r;
	}
	
	public Result(boolean r,String msg){
		this.validate=r;
		this.msg=msg;
	}
	
	public Result(boolean r,String msg,String body){
		this.validate=r;
		this.msg=msg;
		this.body=body;
	}
	
	public Result(boolean r,String msg,String subject,String body){
		this.validate=r;
		this.msg=msg;
		this.subject=subject;
		this.body=body;
	}
	
	public String getMsg() {
		return msg;
	}
	public void setMsg(String msg) {
		this.msg = msg;
	}
	public boolean isValidate() {
		return validate;
	}
	public void setValidate(boolean result) {
		this.validate = result;
	}

	public String getBody() {
		return body;
	}

	public void setBody(String body) {
		this.body = body;
	}

	public String getSubject() {
		return subject;
	}

	public void setSubject(String subject) {
		this.subject = subject;
	}

}
