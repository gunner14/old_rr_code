package com.renren.disk.controllers;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;

import javax.servlet.http.Cookie;

import org.apache.http.client.methods.HttpGet;
import org.apache.log4j.Logger;
import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.disk.config.DConstant;
import com.renren.disk.dao.DUserDAO;
import com.renren.disk.dao.Share;
import com.renren.disk.dao.ShareDAO;
import com.renren.disk.http.DiskClient;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

@Path("")
public class UserController {

	@Autowired
	private DUserDAO userDao;	
	@Autowired
	private ShareDAO shareDao;	
	@Autowired
	private HostHolder hostHolder;
	@Autowired
	private DiskClient diskClient;
	
	private final static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
	
	private final static Logger log = Logger.getLogger(UserController.class.getName());
	
	public String setToken(@Param("code") String code, Invocation inv) throws ParseException{
		String url = new String("https://openapi.baidu.com/oauth/2.0/token?"
							     +"grant_type=authorization_code&"
							     +"code="+code+"&"
							     +"client_id=6UmoiUeSBcwMd7UMGRmyikD9&"
							     +"client_secret=Z61P71cNZkOd3tPlZEMCUjhsIt9TF5GR&"
							     +"redirect_uri=" + DConstant.renren_base_path+ "register.html");
			String response = diskClient.sendHttpRequest(new HttpGet(url));
			JSONObject data = null;
			try {
				data = new JSONObject(response);
			} catch (Exception e) {
				return "@" + DConstant.renren_base_path;
			}
			try {
				if(!data.getString("scope").equals("basic netdisk")){
					return "@" + DConstant.renren_base_path;
				}else{
					String token = data.getString("access_token");
					if(token != null && token.length() != 0){
						String refresh = data.getString("refresh_token");
						int uid = getUserId();
						if(uid == -1){
							return "@http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path;
						}
						userDao.insert(uid, token, refresh);
					}				
					Cookie cookie = new Cookie("a_t", token);
					cookie.setMaxAge(-1);
					cookie.setPath("/");			
					inv.getResponse().addCookie(cookie);	
					inv.getRequest().setAttribute("token",token);				
					return "@" + DConstant.renren_base_path;
				}
			} catch (Exception e) {
				return "@" + DConstant.renren_base_path;
			}
	}
	
	@Get("share/{id}")
	public String share(@Param("id") String id, Invocation inv){
		int uid = getUserId();
		if(uid == -1) {
			return "r:http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path + "share/" + id;
		}
		return "r:c:" + "download/" + id;
	}
	
	@Post("get/{id}")
	public String getInfo(@Param("id") String id){
		int uid = getUserId();
		if(uid == -1){
			return "@" + "http://www.renren.com/SysHome.do?" + DConstant.renren_base_path + "download.html?id=" + id;
		}
		Share sh = shareDao.find(id);
		if(sh != null){
			JSONObject obj = new JSONObject();
			try {
				obj.put("path", sh.getUrl());
				obj.put("size", sh.getSize());
				obj.put("downloads", sh.getDownloads());
				obj.put("time", sh.getTime());
				return "@" + obj;
			} catch (JSONException e) {
				return "@获取分享文件失败";
			}
		}
		return "@分享文件已取消";
	}
	
	@Post("diskshare/{owner}/{id}")
	@Get("diskshare/{owner}/{id}")
	public String getShareInfo(@Param("id") String id, @Param("owner") String owner){
		log.info("id------------------------------------------------->" + id);
		log.info("owner------------------------------------------------->" + owner);
		JSONObject obj;
		String Id = null;
		String name = "disk.png";
		try {
			obj = new JSONObject(id);
			Id = obj.getString("id").substring(1);
			name = obj.getString("name");
		} catch (JSONException e1) {
			e1.printStackTrace();
		}
		
	    obj = new JSONObject();
		try {
			obj.put("link", "http://disk.renren.com/share/" + Id);
			obj.put("title","网盘分享");
			obj.put("thumUrl", "http://disk.renren.com/images/" + DConstant.getFileTypeCss(name));
			obj.put("summary", "test");
			obj.put("fromno", owner);
			obj.put("type", 6);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return "@" + obj;
	}
	
	@Post("shareSuccess")
	public String webShare(@Param("json") String json){
		try {
			JSONObject obj = new JSONObject(json);
			String id = obj.getString("id");
			if(id.charAt(0) != 't') {
				return "@success";
			}
			id = id.substring(1);
			String size = obj.getString("size");
			String name = obj.getString("name");
			int uid = obj.getInt("uid");
			String time = sdf.format(new Date());
			shareDao.insert(id, uid, name, size, time);
			String tempId=UUID.randomUUID().toString();
		    obj = new JSONObject();
			obj.put("id", id);
			obj.put("time", time);
			obj.put("path", name);
			obj.put("size", size);
			JSONObject resp = new JSONObject();
			resp.put("share", obj);
			resp.put("tempId", tempId);
			return "@" + resp;
		} catch (JSONException e) {
			return "@fail";
		}
	}
	
	private int getUserId(){
		User user = hostHolder.getUser();
		if (user != null) {
			return user.getId();
		}
		return -1;
	}
	
	
}
