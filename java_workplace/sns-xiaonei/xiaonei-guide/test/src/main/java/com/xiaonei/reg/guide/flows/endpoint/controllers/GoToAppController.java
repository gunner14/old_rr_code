package com.xiaonei.reg.guide.flows.endpoint.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.action.a090729.A090729Util;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

/**
 * GoToAppController.java
 * @author inshion(xin.yin@opi-corp.com)
 *
 * 2010-8-25 添加LTAPP逻辑 
 * 
 */
@LoginRequired
@Path("goapp")
@IgnoreUserStatusImportedValidation
public class GoToAppController {

	private final static String INIT = "f:lead";
	public final String SUCC = "r:guide.do";
	private final int thisStep = StepStatusLogic.STEP_APP;
	
	@Post
	@Get
	public String post(Invocation inv){
		
		String ret = INIT;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
				
			try {
				StepStatusLogic.updateStepStatus(host.getId(), thisStep);
				GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep,GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			String toUrl = "http://app."+Globals.domain+"/apps/apps.do?selected=2&type=1&category=0&origin=2801";
			String fromSource = ""+RegFromSourceLogic.getFromSource(host);
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - fromSource is :"+fromSource);
			String valueUrl = A090729Util.getUrl(fromSource);
			String farmUrl = A090729Util.getUrl("page.farm");
			String cookUrl = A090729Util.getUrl("page.cook");
			String mzmgUrl = A090729Util.getUrl("page.mzmg");
			String rrfarmUrl = A090729Util.getUrl("page.rrfarm");
			if(StringUtils.isNotEmpty(valueUrl)){
				toUrl = valueUrl;
			}
			GuideLogger.printLog(" host:"+host.getId()+ " - fromSource is :"+fromSource);
			List<String> list = RegFromSourceLogic.getFromSoureList(fromSource);
			if(list.contains("S_FARMREG")){
				toUrl = farmUrl;
			}
			if(list.contains("S_COOKREG")){
				toUrl = cookUrl;
			}
			if(list.contains(RegFromSourceLogic.FS_MAZIMIANGUAN)){
				toUrl = mzmgUrl;
			}
			if(list.contains("RRFARM")){
				toUrl = rrfarmUrl;
			}
			//S_IDREG;:APPTL:PVZ:_ABA:NETBAR
			if(list.contains(RegFromSourceLogic.FS_FROMLEADTOAPP) || list.contains(RegFromSourceLogic.FS_FROMAPPTOLEAD)){
			    toUrl = getAppUrl(host,list);
			    
			    GuideLogger.printLog("tourl:"+toUrl);
			    
			    //加空页面
			    if(list.contains("_ACA")){
			        inv.addModel("uid", host.getId());
			        inv.addModel("returl", toUrl);
			        GuideLogger.printLog("enterstatistics:"+" host:"+host.getId()+" account:"+host.getAccount());
			        return "/views/guide/flowxfivedarkpvz/pvz_statistics.jsp";
			                                             
			    }
			}
			
			if(list.contains(RegFromSourceLogic.FS_PAGE_LOVER)){
			    toUrl = getPageUrl(host);
			}
			if(toUrl == null || toUrl.trim().equals("")){
				toUrl = "http://app."+Globals.domain+"/apps/apps.do?selected=2&type=1&category=0&origin=2801";
			}
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - to url is :"+toUrl);
			ret ="r:"+toUrl;
			
		}
		return ret;
	}

	private String getAppUrl(User host, List<String> fromsource_list) {
	    String returl = null;
        for (String fs : fromsource_list) {
            if(fs == null)continue;
            String curget = A090729Util.getAppUrl("app."+fs.toLowerCase());
            if(MyStringUtils.isNullOrEmpty(curget)){
                continue;
            }
            else{
                returl = curget;
                String param = GuideUtil.getExtra_XXXString(host, fs.toLowerCase());
                if(!MyStringUtils.isNullOrEmpty(param)){
                    returl = returl +""+param;
                }
                GuideLogger.printLog(" host:"+host.getId()+" found:"+fs+" url:"+curget);
                break;
            }
           
        }
        if(MyStringUtils.isNullOrEmpty(returl)){
            GuideLogger.printLog(" host:"+host.getId()+" found:"+"nothing");
        }
        return returl;
    }

    //http://page.renren.com/600039359?ref=search
	private String getPageUrl(User host) {
		String ret = "";
		String pageid = GuideUtil.getExtra_XXXString(host, IRegExtraMsgLogic.key_reg_page_lover);
		if(MyStringUtils.isNullOrEmpty(pageid) ){
		    pageid = GuideUtil.getExtra_XXXString(host, IRegExtraMsgLogic.lover);
		}
		if(!MyStringUtils.isNullOrEmpty(pageid)){
			ret = Globals.urlPage+"/"+pageid;
		}
		return ret;
	}
}
