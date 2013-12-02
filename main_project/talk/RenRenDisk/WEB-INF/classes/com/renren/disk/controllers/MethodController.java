package com.renren.disk.controllers;

import com.renren.disk.config.DConstant;
import com.renren.disk.dao.Share;
import com.renren.disk.dao.ShareDAO;
import com.renren.disk.dao.DUserDAO;
import com.renren.disk.http.DiskClient;
import com.renren.disk.http.Mail;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;


import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

@LoginIRequired
@Path("")
public class MethodController {

	@Autowired
	ShareDAO shareDao;
	
	@Autowired
	DUserDAO userDao;
	
	@Autowired
	DiskClient diskClient;
	
	private final Logger log = Logger.getLogger(MethodController.class.getName());
	
	private String getToken(Invocation inv){
		return (String)inv.getRequest().getAttribute("token");
	}
	
	public String quota(Invocation inv) {
		log.debug("getQuota------------------------------------------------>start");
		return "@" + diskClient.quotaInfo(getToken(inv));
	}
	
	public String meta(@Param("dir") String dir,Invocation inv){
		log.debug("getMeta------------------------------------------------->start");
		return "@" + diskClient.getMeta(getToken(inv), dir);
	}
	
	public String metas(@Param("param") String param,Invocation inv){
		log.debug("getMetas------------------------------------------------>start");
	    return "@[" + diskClient.getFilesMeta(getToken(inv), param) + "]";	
	}
	
	public String list(@Param("dir") String dir,Invocation inv){
		log.debug("getList------------------------------------------------->start");
		return "@[" + diskClient.getList(getToken(inv), dir) + "]";		
	}
	
	public String mkdir(@Param("dir") String dir, Invocation inv){
		log.debug("mkDir--------------------------------------------------->start");
		return "@"+diskClient.mkdir(getToken(inv), dir);
	}
	
	public String dels(@Param("param") String param, @Param("id") String id, Invocation inv){
		log.debug("Dels---------------------------------------------------->start");
		String token = getToken(inv);
		String[] ids = id.split("\r\n");
	    shareDao.delete(ids);
	    String resp = diskClient.deleteFiles(token, param);
	    try {
			JSONObject obj = new JSONObject(resp);
			if(obj.has("error_code")){
				return "@" + resp;
			}
			return  "@" + diskClient.quotaInfo(token);
		} catch (JSONException e) {
			return  "@" + diskClient.quotaInfo(token);
		} 
	}
	
	public String copy(@Param("from") String from,@Param("to") String to, Invocation inv){
		log.debug("Copy--------------------------------------------------->start");
		return "@"+diskClient.copy(getToken(inv), from, to);
	}
	
	public String copys(@Param("param") String param, Invocation inv){
		log.debug("Copys--------------------------------------------------->start");
		return "@"+diskClient.copyFile(getToken(inv), param);
	}
	
	public String rename(@Param("from") String from,@Param("to") String to, Invocation inv){
		log.debug("Rename-------------------------------------------------->start");
		String token = getToken(inv);
	    String resp = diskClient.move(token, from, to);
        try {
			JSONObject obj = new JSONObject(resp);
			if(obj.has("error_code")){
				return "@" + resp;
			}
		    return "@[" + diskClient.getMeta(token, to) + "]";	
        }catch (JSONException e) {
    	   return "@[" + diskClient.getMeta(token, to) + "]";
	    }
	}
	
	public String move(@Param("from") String from,@Param("to") String to, Invocation inv){
		log.debug("move---------------------------------------------------->start");
		return "@"+diskClient.move(getToken(inv), from, to);
	}
	
	public String moves(@Param("param") String param, Invocation inv){
		log.debug("moves--------------------------------------------------->start");
		return "@"+diskClient.moveFile(getToken(inv), param);
	}
	
    public String down(@Param("path") String path, Invocation inv){
    	log.debug("down---------------------------------------------------->start");
    	int userId = (Integer) inv.getRequest().getAttribute("uid");
    	try{
    	    int num = shareDao.update(userId, path);
    	    if(num == 0){
    	    	return "@分享已不存在";
    	    }
    	    return "@" + diskClient.share(path,getToken(inv));
    	}catch(Exception e){
    		return "@文件获取失败";
    	}
    }
    
	public String download(@Param("path") String path, Invocation inv){
		log.debug("download------------------------------------------------>start");
		return "@["+diskClient.share(path,getToken(inv))+"]";
	}
	
	public String createLink(){
		log.debug("createLink---------------------------------------------->start");
		String uid=UUID.randomUUID().toString();
		return "@" + uid;
	}
	
	public String shareLink(@Param("path") String path, @Param("size") String size,
			@Param("id") String id, Invocation inv){
		log.debug("shareLink---------------------------------------------->start");
		int userId = (Integer) inv.getRequest().getAttribute("uid");
		try{
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
			String time = sdf.format(new Date());
		    shareDao.insert(id, userId, path, size, time);
		    return "@" + id + "&" + time;
		}catch(Exception e){
			e.printStackTrace();
			return "@fail";
		}
	}
	
	public String shareList(Invocation inv){
		log.debug("shareList---------------------------------------------->start");
		int userId = (Integer) inv.getRequest().getAttribute("uid");
		log.debug("userId---------------------------------------------->" + userId);
		try{
			List<Share> list= shareDao.find(userId);
			int len=list.size();
			if(len==0)  return "@fail";
			JSONArray arr = new JSONArray();
		    for(int i = 0; i < len; ++i){
				Share sh = list.get(i);
				JSONObject obj = new JSONObject();
				obj.put("id", sh.getId());
				obj.put("path", sh.getUrl());
				obj.put("size", sh.getSize());
				obj.put("time", sh.getTime().toString());
				arr.put(obj);
			}
			return "@" + arr;
		}catch(Exception e){
			log.debug(e);
			return "@fail";
		}
	}
	
	public  String cancelShare(@Param("path") String path , Invocation inv){
		log.debug("cancelShare---------------------------------------------->start");
		String[] arr = path.split("\r\n");
	    try{
	    	shareDao.delete(arr);
	    	return "@success";
	    }catch(Exception e){
	    	return "@fail";	
	    }
	}
	
	public String copyShareList(@Param("path") String path, Invocation inv) {
		log.debug("copyShareList-------------------------------------------->start");
		String[] arr = path.split("\r\n");
		int userId = (Integer) inv.getRequest().getAttribute("uid");
		try {
			String[] str = shareDao.findId(userId, arr);
			int len = str.length;
			String resp = "";
			for (int i = 0; i < len - 1; i++) {
				resp += DConstant.renren_base_path + "share/" + str[i]
						+ "\r\n";
			}
			resp += DConstant.renren_base_path + "share/"
					+ str[len - 1];
			return "@" + resp;
		} catch (Exception e) {
			return "@fail";
		}
	}
	
	public String sendMail(@Param("receivers") String receivers, @Param("content") String content, 
			               @Param("id") String id, @Param("filename") String filename, 
			               @Param("size") String size, Invocation inv){
		log.debug("sendMail-------------------------------------------------->start");
		String time = null;
		if (id != null && id.length() != 0) {
			String sId = UUID.randomUUID().toString();
			try{
				SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
				time = sdf.format(new Date());
			    shareDao.insert(sId, (Integer)inv.getRequest().getAttribute("uid"), filename, size, time);
			}catch(Exception e){
				return "@fail";
			}
		} 
		Mail mail = new Mail();
		String res = mail.SendMessage((String)inv.getRequest().getAttribute("username"), receivers, content);
		if(res != "fail"){
			if(time != null){
				return "@" + time;
			}
			return "@success";
		}
		return "@fail";
	}
	
	public String search(@Param("path") String path, @Param("wd") String wd, Invocation inv){
		log.debug("search----------------------------------------------->start");
		return "@[" + diskClient.search(getToken(inv), path, wd) + "]"; 		
	}
	
}
