package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.AuthService;

@Path("qur")
public class QueryUserRoleController {
	
	@Autowired
	private AuthService authService;
	
	@Get
	@Post
	public String index(@Param("userId") int userId,@Param("pageId") int pageId){
		
		if(authService.isSuperAdmin(userId)){//是否是超级管理员
			return "@"+"3";
		}else if(authService.isPageAdministrator(userId, pageId)){//该方法也包含超管判断，但如果走到这里说明只可能是page的管理员
			return "@"+"2";
		}else{
			return "@"+"1";
		}
	}
}
