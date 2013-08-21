package com.xiaonei.reg.guide.flows.endpoint.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.jump.data.logic.MlinkRegToGameMapLogic;
import com.xiaonei.reg.jump.data.model.MlinkRegToGameMapModel;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * JumpToGameController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-15 创建文件
 */
@LoginRequired
@Path("jumptogame")
@IgnoreUserStatusImportedValidation
public class JumpToGameController {
    private static final String SUCC = "/views/guide/endpoint/jumptogame";
    @Get
    @Post
    public String index(Invocation inv,CacheManager cm){
        String ret = OpiConstants.urlGuide.toString();
        try {
            User host = UserProvider.getInstance().get(cm);
            String ss_id = inv.getParameter("ss");
            String rt_id = inv.getParameter("rt");
            int ssid = MyStringUtils.stringToInt(ss_id);
            int rtid = MyStringUtils.stringToInt(rt_id);
            MlinkRegToGameMapModel model = MlinkRegToGameMapLogic.getInstance().selectBySsIdAndRtId(host, ssid, rtid);
            if(model == null){
                
            }
            else{
                GuideLogger.printLog(" host:"+host.getId()+" model:"+model.toString());
                String codeid = model.getCode_id();
                if(MyStringUtils.isNullOrEmpty(codeid)){
                    codeid = "";
                }
                else{
                    codeid = codeid.trim();
                }
                int statid = model.getGame_stat_id();
                if(statid == 0){
                    
                }
                String tourl = model.getTo_url();
                if(MyStringUtils.isNullOrEmpty(tourl)){
                    ret = getDefaltUrl(codeid,statid);
                }
                else{
                    if(!tourl.startsWith("")){
                        tourl = "http://"+tourl.trim();
                    }
                    ret = getUrl(tourl,codeid,statid);
                }
                inv.addModel("codeid", codeid);
                inv.addModel("statid", statid);
                inv.addModel("tourl", tourl);
            }
            GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        } catch (Exception e) {
            e.printStackTrace();
        }
        inv.addModel("returl", ret);
        
        return SUCC;
    }
    private String getDefaltUrl(String code,int promid){
        String url = ""+"http://gamestat."+OpiConstants.domain+"/game/"+promid+"";
        
        return url;
    }
    private String getUrl(String tourl,String code,int promid){
        String url = ""+ tourl + "?code="+code+"&promitotionid="+promid;
        
        return url;
    }
}
