package com.renren.sns.guide.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.servlet.http.HttpServletRequest;

import org.jfree.util.Log;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

/**
 * PreviewInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 2010-09-26 18:25:17 添加 doAAASearch
 */
@LoginRequired
@Path({"guiderecommendfriendcon"})
@IgnoreUserStatusImportedValidation
public class GuideRecommendFriendController {
    
    @Get
    @Post
    public String get(Invocation inv){
        
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
        User host = GuideXFiveRequestUtil.getCurrentHost(request);      
        String aafret=null;
        try {
            if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
                return "r:http://reg.renren.com/sb/fillemail";
            }
            aafret = doAAFSearch(inv, request,host);
           
        } catch (Exception e) {
            GuideLogger.printLog("exception  "+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        
        return aafret;
    }
    
    private List<Integer> getFriends(Invocation inv,User host){
        List<Integer> list = FriendsHome.getInstance().getFriendsIds(host.getId());
        if(list == null){
            list = new ArrayList<Integer>();
        }
        return list;
    }
    
    private List<Integer> getRequestFriends(Invocation inv,User host){ 
		List<Integer> list = new ArrayList<Integer>();
		BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
        DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, -1);
        for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
			list.add(el.getAccepter());
        }
        return list;
	}
    
    @SuppressWarnings("unused")
    private String getIntsString(List<Integer> list){
        StringBuilder sb = new StringBuilder();
        sb.append("[");
        
        if(list == null || list.size() == 0){
            
        }
        else{
            for (Integer integer : list) {
                sb.append(integer).append(",");
            }
        }
        sb.append("]");
        return sb.toString();
    }
    
    private String doAAFSearch(Invocation inv, HttpServletRequest request, final User host) {
        try {
            List<Integer> friendlist = getFriends(inv, host);
            List<Integer> requestlist = getRequestFriends(inv, host);
            List<DefaultOffer> offerList = GuideUtil.getOfferFriendListByStage(host);
            List<Integer> ids = GuideUtil.getOfferFriendIdList(offerList); 
            GuideLogger.printLog(" host:"+host.getId()+" mayfriends:"+ids.size());
            ids.removeAll(friendlist);
            ids.removeAll(requestlist);
            GuideLogger.printLog(" host:"+host.getId()+" fl:"+friendlist.size()+" after:"+ids.size());
            
            int size=ids.size();
            if(size>30){
            	ids=ids.subList(0, 30);
            }
            
            Map<Integer, WUserCache> cmap = GuideUtil.getUserCacheMap(ids); 
            request.setAttribute("guidefriendcmap", cmap.values());
            
            /*
             * 增加好友推荐类型type
             * 前端接受对象 Map<Integer, DefaultOffer> defaultOfferMap
             * Kobin  bin.huang1@renren.inc.com
             */
            Map<Integer, DefaultOffer>  offerMap =  GuideUtil.getOfferFriendMap(offerList);
            Map<Integer, DefaultOffer> defaultOfferMap = new HashMap<Integer, DefaultOffer>();
            Iterator<Integer> it = ids.iterator();
            while(it.hasNext()){
            	int userId = it.next();
            	defaultOfferMap.put(userId, offerMap.get(userId));
            }
            request.setAttribute("defaultOfferMap", defaultOfferMap);
            
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        return "/pages/guide/guiderecommendflow/recommendfriend.jsp";
    }
}
