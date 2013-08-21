package com.renren.sns.guide.controllers;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.newbie.module.NewBieHelperStep;
import com.renren.newbie.service.NewbieHelperService;
import com.renren.newbie.utils.NewbieHelperConstants;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

@LoginRequired
@Path("ajaxtUpdateNiewbieHelperStep")
public class AjaxtUpdateNiewbieHelperStepController {

	@Autowired
	HostHolder hostHolder;
	
	@Post("setStep")
	@Get("setStep")
	public String setStep(Invocation inv,@Param("step") String step )
			throws InterruptedException {
		
		User user = hostHolder.getUser();
		long newStep =0;
		NewBieHelperStep nhs = NewbieHelperService.getInstance().getStep(user.getId());
		if(nhs == null){
			return "@ERR";
		}
		long oldStep = nhs.getStep();
		if(NewbieHelperConstants.FILLI_NFO_KEY.equals(step)){
			
			newStep = oldStep|NewbieHelperConstants.FILLI_NFO;
			
		}else if(NewbieHelperConstants.SEARCH_FRIEND_KEY.equals(step)){
			
			newStep = oldStep|NewbieHelperConstants.SEARCH_FRIEND;
			
		}else if(NewbieHelperConstants.UPLOAD_HEAD_KEY.equals(step)){
			
			newStep = oldStep|NewbieHelperConstants.UPLOAD_HEAD;
			
		}else if(NewbieHelperConstants.UPLOAD_PHOTO_KEY.equals(step)){
			
			newStep = oldStep|NewbieHelperConstants.UPLOAD_PHOTO;
			
		}
		
		boolean result =NewbieHelperService.getInstance().setStep(user.getId(), newStep);
		
		if(!result){
			return "@ERR";
		}
		JSONObject json = NewbieHelperService.getInstance().getStepJson(user.getId());
		
		return "@"+json.getString("nextStep");
	
	}
	
	@Post("setDoingStep")
	@Get("setDoingStep")
	public String setDoingStep(Invocation inv,@Param("step") String step )
			throws InterruptedException {
		
		User user = hostHolder.getUser();
		long newStep =0;
//		NewBieHelperStep nhs = NewbieHelperService.getInstance().getStep(user.getId());
//		if(nhs == null){
//			return "@ERR";
//		}
//		long oldStep = nhs.getDoingStep();
		if(NewbieHelperConstants.FILLI_NFO_KEY.equals(step)){
			
			newStep = NewbieHelperConstants.FILLI_NFO;
			
		}else if(NewbieHelperConstants.SEARCH_FRIEND_KEY.equals(step)){
			
			newStep = NewbieHelperConstants.SEARCH_FRIEND;
			
		}else if(NewbieHelperConstants.UPLOAD_HEAD_KEY.equals(step)){
			
			newStep = NewbieHelperConstants.UPLOAD_HEAD;
			
		}else if(NewbieHelperConstants.UPLOAD_PHOTO_KEY.equals(step)){
			
			newStep = NewbieHelperConstants.UPLOAD_PHOTO;
			
		}
		
		boolean result = NewbieHelperService.getInstance().setDoingStep(user.getId(), newStep);
		
		if(!result){
			return "@ERR";
		}
//		JSONObject json = NewbieHelperService.getInstance().getStepJson(user.getId());
		
		return "@OK";
	}
}
