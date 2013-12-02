package com.renren.disk.http;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.net.URLEncoder;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpRequestBase;
import org.apache.http.conn.params.ConnManagerParams;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.util.EntityUtils;
import org.apache.log4j.Logger;


public class DiskClient {

	private  final int CONNECT_TIMEOUT = 30 * 1000;
	private  final int T_TIMEOUT = 60 * 60 * 1000; 
	
	public  final String VERSION = "0.9";

	final  String PCS_REQUEST_URL = "https://pcs.baidu.com/rest/2.0/pcs";
	private final  Logger log = Logger.getLogger(DiskClient.class.getName());

	/**
	 * 获取单个文件/目录的元信息
	 * 
	 * @param token
	 *            用户的access_token
	 * @param path
	 *            路径名
	 * @return 
	 */
	public  String getMeta(String token, String path) {
		String[] params = {"method", "access_token", "path", "meta", token, path};
		return doGet("file", params);
	}

	/**
	 * 获取当前用户空间配额信息
	 * 
	 * @param token
	 *            用户的access_token
	 * @return Json串（包括： quota: 空间配额  used：已使用空间）
	 */
	public  String quotaInfo(String token) {
		String[] params = {"method", "access_token", "info", token};
		return doGet("quota", params);
	}
	
    /**
     * 获取分享文件的url
     * @param path
     * 分享文件的路径
     * @param token
     * 用户的access_token
     * @return
     */
	public  String share(String path,String token){
		String[] params = {"method", "access_token", "path", "create", token, path};	
		return doGet("share", params);
	} 
	
    /**
     * 获取列表信息
     * @param token
     * 用户access_token
     * @param path
     * 目录路径
     * @return 列表中文件信息
     */
	public  String getList(String token, String path) {
		String[]  params = {"method", "access_token", "path", "list", token, path};
		return doGet("file", params);
	}	
	
	public  String search(String token, String path, String wd){
		String[] params ={"method", "access_token", "path", "wd", "re", "search", token, path, wd, "1"};
		return doGet("file", params);
	}
	/**
	 * 删除文件
	 * @param token、
	 * 用户access_token
	 * @param path
	 * 文件路径
	 * @return  
	 */
	public  String delete(String token, String path) {
		return doPost("delete", token, new String[]{"path", path});
	}
	
	/**
	 * 创建目录
	 * 
	 * @param token
	 *            用户access_token
	 * @param path
	 *            目录路径
	 * @return 创建结果，以json串表示,包括（fs_id：标识id，path：该目录绝对路径,ctime：创建时间,mtime：修改时间）
	 */
	public  String mkdir(String token, String path)
	{
		return doPost("mkdir", token, new String[]{"path", path});
	}

	/**
	 * 复制单个文件
	 * 
	 * @param token
	 *            用户access_token
	 * @param from
	 *            源地址
	 * @param to
	 *            目标地址
	 * @return 复制结果
	 */
	public  String copy(String token,String from, String to) {
		return doPost("copy", token, new String[]{"from", "to", from, to});
	}
	
	/**
	 * 移动单个文件
	 * 
	 * @param token
	 *            用户access_token
	 * @param from
	 *            源地址
	 * @param to
	 *            目标地址
	 * @return 移动结果
	 */
	public  String move(String token, String from, String to) {
		return doPost("move", token, new String[]{"from", "to", from, to});
	}
	
	/**
	 * 批量获取文件元信息
	 * 
	 * @param token
	 *            用户token
	 * @param jsonString
	 *            文件对象以json的形式表示
	 * @return 操作结果，以json形式表示
	 */
	public  String getFilesMeta(String token, String jsonString)
	{
	    return doFiles(token, "meta",jsonString);
	}

	/**
	 * 批量删除文件
	 * 
	 * @param token
	 *            用户access_token
	 * @param jsonString
	 *            文件对象，以json的形式表示
	 * @return 操作结果，以json形式表示
	 */
	public  String deleteFiles(String token, String jsonString)
	{
	    return doFiles(token, "delete",jsonString);
	}
	
	/**
	 * 批量复制文件
	 * 
	 * @param token
	 *            用户access_token
	 * @param jsonString
	 *            文件对象，以json的形式表示
	 * @return 复制结果，以json形式表示，包括了文件的基本信息
	 */
	public  String copyFile(String token, String jsonString){
		return doFiles(token, "copy",jsonString);
	}
	
	/**
	 * 批量移动文件
	 * 
	 * @param token
	 *            用户access_token
	 * @param jsonString
	 *            文件对象，以json的形式表示
	 * @return 操作结果，以json形式表示
	 */
	public  String moveFile(String token, String jsonString){
		return doFiles(token, "move",jsonString);
	}
	
	/**
	 * 单文件post请求处理
	 * 
	 * @param method
	 *            方法名
	 * @param token
	 *            用户access_token
	 * @param param
	 *            处理参数
	 * @return 处理结果，以json形式表示
	 */
	private  String doGet(String object_name, String[] params){
		String url = PCS_REQUEST_URL + "/" + object_name + "?" + getUrl(params);
		log.info("url = " + url);
		HttpGet httpget = new HttpGet(url);
		String response = sendHttpRequest(httpget);		
		return response;
	}
	
	/**
	 * 单文件post请求处理
	 * 
	 * @param method
	 *            方法名
	 * @param token
	 *            用户access_token
	 * @param param
	 *            处理参数
	 * @return 处理结果，以json形式表示
	 */
	private  String doPost(String method, String token, String[] param){
		String[] params = {"method", "access_token", method, token};
		String url = PCS_REQUEST_URL + "/file?" + getUrl(params);
		HttpPost post = new HttpPost(url);
		try {
			post.setEntity(new UrlEncodedFormEntity(buildBoryParams(param), "utf-8"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		String response = sendHttpRequest(post);
		return response;
	}
	
	/**
	 * 批量操作数据
	 * 
	 * @param token
	 *            用户access_token
	 * @param method
	 *            方法名
	 * @param jsonString
	 *            操作对象
	 * @return 操作结果，以json的形式展示
	 */
	public  String doFiles(String token, String method, String jsonString)
	{	    
        String[] params = {"method", "access_token", method, token};
	    String url = PCS_REQUEST_URL + "/file?" + getUrl(params);
	    HttpPost post = new HttpPost(url);
	    if(jsonString!=null && jsonString.length()>0)
	    {
	        try {
	            post.setEntity(new UrlEncodedFormEntity(buildBoryParams(new String[]{"param", jsonString}), "utf-8"));
	        } catch (UnsupportedEncodingException e) {
	            e.printStackTrace();
	        }
	    }
	    String response = sendHttpRequest(post);
	    return response;
	}

	/**
	 * 将参数转化为url参数形式
	 * 
	 * @param args
	 *            参数
	 * @return url参数
	 */
	private  String getUrl(String[] args){
    	int len = args.length;
    	int num = len/2;
    	String url="";
    	for(int i = 0; i < num; i++){
    		if(i != 0){
    			url += "&";
    		}
    		try {
				url += args[i] + "=" + URLEncoder.encode(args[i+num],"utf-8");
			} catch (UnsupportedEncodingException e) {
				url += args[i] + "=" + args[i+num];
			}
    	}
    	return url;
    }

	private  List<NameValuePair> buildBoryParams(String[] params){
	    List<NameValuePair> bodyParams = new ArrayList<NameValuePair>();
	    int len = params.length;
	    for(int i = 0; i< len/2; i++)
	    bodyParams.add(new BasicNameValuePair(params[i], params[i + len/2]));
	    return bodyParams;
	}	

	public  String sendHttpRequest(HttpRequestBase request) {
		String resultJsonString = "";
		if (null != request) {
			HttpClient client = newHttpClient();
			try {
				HttpResponse response = client.execute(request);
				if (null != response) {
					HttpEntity resEntity = response.getEntity();
					resultJsonString = EntityUtils.toString(resEntity);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return resultJsonString;
	}

	private  HttpClient newHttpClient() {
		HttpClient httpClient;
		httpClient = TrustAllSSLSocketFactory.getNewHttpClient();
		final HttpParams params = httpClient.getParams();
		HttpConnectionParams.setConnectionTimeout(params, CONNECT_TIMEOUT);
		ConnManagerParams.setTimeout(params, T_TIMEOUT);
		return httpClient;
	}
    
}
