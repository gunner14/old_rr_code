package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.usertrace.dao.GuideRecommendDAO;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;

@LoginRequired
@Path("saveuseroperee")
@IgnoreUserStatusImportedValidation

/**
 * SaveUserOperController.java
 * @author gangfeng.dong@renren-inc.com
 * 
 */

public class SaveUserOperController {
    private static final String SUCC = "f:lead"; 
    private static final int thisStep = StepStatusLogic.STEP_PREVIEW;
    private static final int BGLIMIT = 100;
    
    @Get
    @Post
    public String get(Invocation inv){
        HttpServletRequest request = inv.getRequest();
        User host = BaseThreadUtil.currentHost(request);
        if(host==null){
            GuideLogger.printLog("hostisnull");
            return "@ user is null";
        }
        
        if(!"333127273".equals(host.getId()+"")){
			return "@Inner Required";
		}
        String targetid=request.getParameter("targetid");
        if (targetid == null || "".equals(targetid) || (targetid.matches("[0-9]+") == false)) {
        	return "@data is not correct id:"+targetid;
        }
        
        try {
			User targetHost = SnsAdapterFactory.getUserAdapter().get(new Integer(targetid));
			//LoginLogic.getInstance().doLoginR(host,request,response,0,false);
			 LoginLogic.getInstance().doLoginR(targetHost, inv.getRequest(), inv.getResponse(), 0, false);
		} catch (NumberFormatException e) {
			 return "@ "+e.toString();
		}
        
        return "@success";
    }
    
    
    
}
