package com.xiaonei.reg.guide.flows.oldactions.controllers;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForm;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

/*
 * <action path="/nof" type="com.xiaonei.reg.guide.action.appflashflow.NoFlashLoadedAction" scope="request"></action>
 * 
 */

/**
 * NoFlashLoadedController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-1 兼容 /nof.do
 */

@LoginRequired
@Path({"nof.do"})
@IgnoreUserStatusImportedValidation
public class NoFlashLoadedController{


    @Get
    @Post
    public String index(Invocation inv) {
        GuideLogger.printLog("NEWCONTROLLER");
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        ActionMapping mapping = new ActionMapping(map);
        return executePt(mapping, null, request, response).toString();
    }
    
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			{
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		
		String reason = "99";
		String type = "99";
		String newsource = RegFromSourceLogic.getFromSource(host);
		try {
			cost.endStep();
			reason = request.getParameter("reason");
			type =  request.getParameter("type");
			cost.endStep();
			
			if(newsource == null){
				//是什么应用的
				if("0".equals(type)){ //farm
					newsource = "S_FARM";
				}
				else if("1".equals(type)){ //cook
					newsource = "S_COOK";
				}
				else{
					newsource = RegFromSourceLogic.getFromSource(host);
				}
				cost.endStep();
				
				if("0".equals(reason)){
					newsource = newsource + "_TIMEOUT";
				}
				else if("1".equals(reason)){
					newsource = newsource + "_NOFLASH";
				}
				else{
					newsource = newsource + "_ERR";
				}
				cost.endStep();
			}
			else{
				newsource = newsource.replace("S_FARMREG", "S_FARM_TIMEOUT").replace("S_COOKREG", "S_COOK_TIMEOUT");
			}
			
		} catch (Exception e) {
			GuideLogger.printLog("para error? host:"+host.getId()+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		cost.endStep();
		if(newsource == null){
			GuideLogger.printLog("getFromSource failed! host:"+host.getId(),GuideLogger.ERROR);
		}
		cost.endStep();
		GuideLogger.printLog("save newsrc host:"+host.getId()+" - "+newsource);
		RegFromSourceLogic.update(host.getId(), newsource);
		cost.endStep();
		cost.endFinally();
		return new ActionForward("/guide.do",true);
	}

}
