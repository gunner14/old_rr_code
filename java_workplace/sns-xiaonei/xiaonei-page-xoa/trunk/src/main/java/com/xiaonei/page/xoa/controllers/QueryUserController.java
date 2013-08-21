package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.page.xoa.domain.UserView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("qu")
public class QueryUserController {
	@Get
	public UserView index(@Param("userId")int userId){
		
		User user=WUserAdapter.getInstance().get(userId);
		if(user!=null){
			UserView uv=new UserView();
			
			uv.setAccount(user.getAccount());
			uv.setAuth(user.getAuth());
			uv.setDomain(user.getDomain());
			uv.setHeadFullUrl(user.getHeadFullUrl());
			uv.setHeadUrl(user.getHeadUrl());
			uv.setId(user.getId());
			uv.setLargeFullUrl(user.getLargeFullUrl());
			uv.setLargeUrl(user.getLargeUrl());
			uv.setLevel(user.getLevel());
			uv.setLoginCount(user.getLoginCount());
			uv.setMainFullUrl(user.getMainFullUrl());
			uv.setMainUrl(user.getMainUrl());
			uv.setNickName(user.getNickname());
			uv.setRights(user.getRights());
			uv.setStar(user.getStar());
			uv.setStatus(user.getStatus());
			uv.setTinyFullUrl(user.getTinyFullUrl());
			uv.setTinyUrl(user.getTinyUrl());
			uv.setName(user.getName());
			
			return uv;
		}
		return null;
	}
}
