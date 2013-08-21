package com.renren.sns.guide.controllers;

import java.util.List;

import mop.hi.oce.domain.network.Network;
import mop.hi.oce.domain.network.UniversityInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.dodoyo.datum.cache.DatumInfoMgr;
import com.dodoyo.datum.network.DatumNetworkMgr;
import com.renren.sns.guide.logic.GuideLogic;
import com.renren.sns.guide.utils.GuideUtils;
import com.renren.sns.guide.utils.ThreadPool;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.highschool.domain.Page;
import com.xiaonei.highschool.service.FansService;
import com.xiaonei.highschool.service.PageService;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.sns.platform.core.rose.interceptors.CookieCleaner;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;

@CookieCleaner
@LoginRequired
@Path( { "Guide.do", "guide.do", "Guide", "guide", "Guide1.do", "guide1.do", "Guide1", "guide1" })
@Performance
@IgnoreUserStatusImportedValidation
public class GuideController {

	@Get
	public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
		portal.setTimeout(300);// 超时300ms就切除
		User host = UserProvider.getInstance().get(cm);
		
		if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
		
		String view = inv.getRequest().getParameter("__view");
		// 走完引导第一次到达 guide页
		String gt = inv.getRequest().getParameter("t");

		boolean newStageTen = false;
		if (host.getStage() == Stage.STAGE_HEIGHSCHOOL && host.getId() % 10 <= 5) {
			newStageTen = true;
		}

		inv.addModel("newStageTen", newStageTen);
		if ("first".equals(gt)) {
			GuideLogger.printLog("first Get guide");
			MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
			if (mem != null) {
				mem.set(GuideUtils.FIRST_TIME_GUIDE + host.getId(), "first", (int) MemCacheKeys.day * 1);
			}
		}
		GuideLogic.getInstance().addCommonWindows(inv, portal, host);
		if ("async-html".equals(view)) {
			GuideLogic.getInstance().addAllStageWindows(inv, portal, host);
			if (newStageTen) {
				GuideLogic.getInstance().addWindowsForStageTen(inv, portal, host);
				return "guide-ms-async";
			}
			GuideLogic.getInstance().addOtherWindows(inv, portal, host);
			return "guide-async";
		} else if ("async-html-reload".equals(view)) {
			GuideLogic.getInstance().addAllStageWindows(inv, portal, host);
			if (newStageTen) {
				GuideLogic.getInstance().addWindowsForStageTen(inv, portal,host);
				return "guide-ms-async-reload";
			}
			GuideLogic.getInstance().addOtherWindows(inv, portal, host);
			return "guide-async-reload";
		} else {
		    
		    
			// 判断一下这个用户的所完成的步骤，如果没有完成回到该去完成的页面
			String toUrl = StepStatusLogic.goWhere(host);
			if (!toUrl.equals("") && toUrl.length() > 0) {
				return "r:" + toUrl;
			}
			

			
			// 防止直接访问将要分发的action.do
			inv.getRequest().setAttribute("G" + String.valueOf(host.getId()),String.valueOf(host.getId()));
			try {
				String vid = (String) inv.getRequest().getAttribute("G" + String.valueOf(host.getId()));
				if (vid == null || !String.valueOf(host.getId()).equals(vid)) {
					return null;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			//校园频道需求 新注册用户，如果满足以下两个条件
			//1、大学入学年为2011  2、填写了大学资料
			//就自动关注资料中填写的大学所对应的校园频道。
			
			class univThread implements Runnable{
				
				User host;
				public univThread(User host){
					this.host = host;
				}
				
				@Override
				public void run() {
					List<UniversityInfo> uniList = null;
					try {
						 uniList = DatumInfoMgr.getInstance().getUniversityInfos(host.getId());
					} catch (Exception e) {
						e.printStackTrace();
					}
					if(uniList!=null && uniList.size() >0){
						for(UniversityInfo uni:uniList){
							if(uni.getEnrollYear() == 2011){
								Page schoolPage = PageService.getInstance().getPageBySchoolId(uni.getUniversityId());
								if(!FansService.getInstance().isFans(schoolPage,host)){
									FansService.getInstance().reqBeFans(schoolPage, host);
								}
								break;
							}
						}
					}
					
				}
			}
			ThreadPool.defaultInstance().execute(new univThread(host));
			
			return "guide";
		}
	}

}
