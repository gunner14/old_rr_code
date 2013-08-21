package com.renren.sns.guide.controllers;



import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFetherImpl;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.var.Model;
import net.sf.json.JSONObject;

import com.renren.newbie.service.NewbieHelperService;
import com.renren.newbie.utils.NewbieHelperConstants;
import com.renren.sns.guide.logic.GuideLogic;
import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.annotation.NewbieHelper;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.sns.platform.core.rose.interceptors.CookieCleaner;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;

@CookieCleaner
@LoginRequired
@Path( { "Guide.do", "guide.do", "Guide", "guide", "Guide1.do", "guide1.do", "Guide1", "guide1"})
@Performance
@IgnoreUserStatusImportedValidation
public class GuideController {

    private final RemoteAddrFether remoteAddrFether= new RemoteAddrFetherImpl();
    @NewbieHelper
    @Get
    public String index(final Invocation inv, final Portal portal, final CacheManager cm) throws InterruptedException {
        portal.setTimeout(300);// 超时300ms就切除
        final User host = UserProvider.getInstance().get(cm);
        
        if((host!=null) && (host.getStatus()==10) && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
        
        String newFlowSource=RegFromSourceLogic.getFromSource(host);
        if(newFlowSource==null){
        	newFlowSource="";
        }
        final int userid=host.getId();
         
        final String view = inv.getRequest().getParameter("__view");
        // 走完引导第一次到达 guide页
        final String gt = inv.getRequest().getParameter("t");

        boolean newStageTen = false;
        if ((host.getStage() == Stage.STAGE_HEIGHSCHOOL) && ((host.getId() % 10) <= 5)) {
            newStageTen = true;
        }

        //所有的用户都进入v6
        if(!host.getUserState().isV6()){

        	GuideLogger.printLog("*********change user to v6  userid:"+host.getId());
            SnsAdapterFactory.getUserStateAdapter().setV6On(host.getId());
            //int[] appIds = new int[]{166607,166613,166615,166616,166617};
            //FavoriteMenuLogicAdapter.getInstance().reorder(Integer.valueOf(host.getId()), appIds);
            Model model = inv.getModel();
            model.add("userMenu", UserMenuManager.getInstance().getNewMenuList(host.getId(), remoteAddrFether.getRemoteAddr(inv.getRequest())));
        }

        inv.addModel("newStageTen", newStageTen);
        final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
        if ("first".equals(gt)) {
            GuideLogger.printLog("first Get guide");
            
            if (mem != null) {
                mem.set(GuideUtils.FIRST_TIME_GUIDE + host.getId(), "first", (int) MemCacheKeys.day * 1);
            }
            //添加用户的训导数据
            if(newFlowSource.endsWith("_AAF") && (userid%10==0 || userid%10==9)){
            	 JSONObject step = NewbieHelperService.getInstance().getStepJson(userid);
                 if(step == null){
                 	 NewbieHelperService.getInstance().initStep(userid, 0, NewbieHelperConstants.VER_1,0);
                 	 step = NewbieHelperService.getInstance().getStepJson(userid);
                 }
                 inv.addModel("newbieHelperStep", step.toString());
            }
           
            
        }
        GuideLogic.getInstance().addCommonWindows(inv, portal, host);
        if ("async-html".equals(view)) {

        	//GuideLogger.printLog("async-html enter");
            GuideLogic.getInstance().addAllStageWindows(inv, portal, host);
            return "guide-async-v6";

        } else if ("async-html-reload".equals(view)) {


        	//GuideLogger.printLog("async-html-reload enter");
            GuideLogic.getInstance().addAllStageWindows(inv, portal, host);


            return "guide-async-v6";
            
        } else {
        	//GuideLogger.printLog("guide enter");
            // 判断一下这个用户的所完成的步骤，如果没有完成回到该去完成的页面
            final String toUrl = StepStatusLogic.goWhere(host);
            if (!toUrl.equals("") && (toUrl.length() > 0)) {
                return "r:" + toUrl;
            }
            // 防止直接访问将要分发的action.do
            inv.getRequest().setAttribute("G" + String.valueOf(host.getId()),String.valueOf(host.getId()));
            try {
                final String vid = (String) inv.getRequest().getAttribute("G" + String.valueOf(host.getId()));
                if ((vid == null) || !String.valueOf(host.getId()).equals(vid)) {
                    return null;
                }
            } catch (final Exception e) {
                e.printStackTrace();
            }


            GuideLogic.getInstance().addFavList(portal);
            return "guide-v6";
        }
    }
    

}
