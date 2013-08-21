package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.logic.DigitalBeatLettersLogic;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.MD5;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

@LoginRequired
@Path( { "agioud" })
public class AjaxGuideInserOrUpdateDigitalController {
	
	@Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
		String checksum=(String)inv.getParameter("checksum");			
		String strD=(String)inv.getParameter("d");	
		String premd5="hhxw"+strD;		
		String aftermd5=MD5.digest(premd5);
		if(!aftermd5.equals(checksum)){
			return "@";
		}
		Integer digital=0;		
		User user=UserProvider.getInstance().get(cm);
		Integer userId=user.getId();
		try{
			digital=Integer.parseInt(strD);
			if(digital>150){
				digital=150;
			}
			DigitalBeatLettersLogic.getInstance().insertOrUpdateDigital(userId, digital);
			DigitalBeatLettersLogic.getInstance().updateDigitalSum(digital);
		}
		catch(Exception e){
			e.printStackTrace();
		}	
		String json=DigitalBeatLettersLogic.getInstance().getDigitalAndLetters();
		return "@"+json;
	}
	
	 
}
