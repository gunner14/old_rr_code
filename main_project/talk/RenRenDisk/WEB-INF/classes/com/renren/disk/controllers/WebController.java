package com.renren.disk.controllers;

import java.util.UUID;

import javax.servlet.http.Cookie;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.http.client.methods.HttpGet;
import org.apache.log4j.Logger;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.disk.config.DConstant;
import com.renren.disk.dao.DUser;
import com.renren.disk.dao.DUserDAO;
import com.renren.disk.dao.Share;
import com.renren.disk.dao.ShareDAO;
import com.renren.disk.http.DiskClient;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

@Path("")
public class WebController {
	
	@Autowired
	private HostHolder hostHolder;
	
	@Autowired
	private DUserDAO userDao;
	
	@Autowired
	private ShareDAO shareDao;
	
	@Autowired
	private DiskClient diskClient;
	
	private final Logger log = Logger.getLogger(WebController.class.getName());
	
	@Get("main")
	public String Login(Invocation inv) {
		User user = hostHolder.getUser();
		if (user == null) {
			return "r:http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path + "main";
		}		
		int uid  = user.getId();
		log.info("uid----------------------------------------------------->" + uid);
		DUser Duser = userDao.findById(uid);
		if (Duser == null) {
			return "r:" + DConstant.baidu_base_path+"authorize?response_type=code&client_id=6UmoiUeSBcwMd7UMGRmyikD9&scope=netdisk&"
			           + "redirect_uri=" + DConstant.renren_base_path + "register";
		} else {
			inv.getRequest().setAttribute("token", Duser.getAccessToken());
			Cookie cookie = new Cookie("a_t", Duser.getAccessToken());
			cookie.setMaxAge(-1);
			cookie.setPath("/");
			inv.getResponse().addCookie(cookie);
		}
		inv.addModel("tempId", UUID.randomUUID().toString());
		inv.addModel("uid", uid);
		return "main.jsp";
	}
	
	@Get("download/{id}")
	public String download(@Param("id") String id, Invocation inv) {
		User user = hostHolder.getUser();
		if (user == null) {
			return "r:http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path + "download/" + id;
		}
		Share share = shareDao.find(id);
		inv.addModel("shareObj", share);
		return "download.jsp";
	}
	
	@Post("register")
	@Get("register")
	public String register(@Param("code") String code, Invocation inv) {
		User user = hostHolder.getUser();
		if (user == null) {
			return "r:http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path;
		}
		int uid = user.getId();
		String url = new String("https://openapi.baidu.com/oauth/2.0/token?"
							   +"grant_type=authorization_code&"
							   +"code="+code+"&"
							   +"client_id=6UmoiUeSBcwMd7UMGRmyikD9&"
							   +"client_secret=Z61P71cNZkOd3tPlZEMCUjhsIt9TF5GR&"
							   +"redirect_uri=" + DConstant.renren_base_path + "register");
		String response = diskClient.sendHttpRequest(new HttpGet(url));
		JSONObject data = null;
		try {
			data = new JSONObject(response);
		} catch (Exception e) {
			return "@:" + "注册失败";
		}
		
		try {
			if (data.getString("scope").equals("basic netdisk")) {
				String token = data.getString("access_token");
				String refresh = data.getString("refresh_token");				
			    userDao.insert(uid, token, refresh);
			    Cookie cookie = new Cookie("a_t", token);
				cookie.setMaxAge(-1);
				cookie.setPath("/");			
				inv.getResponse().addCookie(cookie);	
				inv.getRequest().setAttribute("token",token);	
			}		
			return "r:c:main";
		} catch (Exception e) {
			return "r:c:main";
		}
	}
}
