package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import Ice.TimeoutException;

import com.dodoyo.datum.model.RegionInfo;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.lead.dataproviders.util.StatLogger;
import com.renren.sns.minisite.xoa.model.SiteInfo;
import com.renren.sns.tag.service.site.SiteUGCService;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.formbeans.InfoForPreview;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.usertrace.dao.GuideRecommendDAO;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.friendfinderinfocache.FriendFinderInfoCacheAdapter;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

/**
 * PreviewInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 2010-09-26 18:25:17 添加 doAAASearch
 */
@LoginRequired
@Path({"initrecommenddata"})
@IgnoreUserStatusImportedValidation
public class InitRecommendDataController {
    
    @Autowired
    private GuideRecommendJadeDAO guideRecommendJadeDAO;

    
	/*@Get
	@Post
	public String get(Invocation inv){
	    
	    String curip = BizFilterUtil.getClientIP(inv.getRequest());
        if(curip!=null && curip.trim().startsWith("10.")){
            final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
            final com.renren.sns.minisite.xoa.client.SiteService service = fact
                    .getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
            ServiceFuture<Integer[]> future = (ServiceFuture<Integer[]>)service.wallSiteIds();
            Integer[] wallSiteids = ServiceFutureHelper.execute(future, 30000);
            try {
                guideRecommendJadeDAO.deleteGuideContentdContent();
            } catch (SQLException e1) {
                e1.printStackTrace();
            }
            StringBuffer sbf=new StringBuffer();
            SiteInfo siteInfo=null;
            int size=wallSiteids.length;
            if(size<=1000){
                for(int i=0;i<size;i++){
                    sbf.append(wallSiteids[i]);
                    sbf.append(" * ");
                    try {
                        siteInfo=getXiaoZhanPojoById(wallSiteids[i]);
                        if(siteInfo.getDescription()!=null && siteInfo.getDescription().trim().length()>0){
                            GuideRecommendDAO.getInstance().intiRecommendContent(new Long(wallSiteids[i]));
                        }
                        
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                    
                }
                if(siteInfo!=null){
                    sbf.append(" id:"+siteInfo.getId()+" url:"+siteInfo.getUrl()+" name:"+siteInfo.getName()
                            +" head:"+siteInfo.getHead()+" tinyHead"+siteInfo.getTinyHead()+
                            " description:"+siteInfo.getDescription());
                }
               
                
                
                
                return "@"+sbf.toString();
            }
            else{
                for(int i=0;i<1000;i++){
                    sbf.append(wallSiteids[i]);
                    sbf.append(" * ");
                    
                    try {
                        siteInfo=getXiaoZhanPojoById(wallSiteids[i]);
                        if(siteInfo.getDescription()!=null && siteInfo.getDescription().trim().length()>0){
                            GuideRecommendDAO.getInstance().intiRecommendContent(new Long(wallSiteids[i]));
                        }
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                    
                }
                if(siteInfo!=null){
                    sbf.append(" id:"+siteInfo.getId()+" url:"+siteInfo.getUrl()+" name:"+siteInfo.getName()
                            +" head:"+siteInfo.getHead()+" tinyHead"+siteInfo.getTinyHead()+
                            " description:"+siteInfo.getDescription());
                }
                
                
                return "@"+sbf.toString();
            }
            

            
        }
        return "@Inner Required";
	}*/
	
    
    @Get
	@Post
	public String get(Invocation inv) {

		String curip = BizFilterUtil.getClientIP(inv.getRequest());
		if (curip != null && curip.trim().startsWith("10.")) {

			try {
				guideRecommendJadeDAO.deleteGuideContentdContent();
				GuideRecommendDAO.getInstance().intiRecommendContent(671000207l,"dxsyylm","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000843l,"yinyue","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002289l,"westernmusic","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002558l,"mrmoney","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002608l,"billboard","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003329l,"princemusic","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003547l,"88sing","音乐");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003997l,"shengdian","音乐");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000128l,"filmmuseum","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000363l,"filmloveletter","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001811l,"lovemovies","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671004381l,"aimatong","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005420l,"2minutes","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671006447l,"moviee","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671040029l,"jiangjjyyo","电影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671060344l,"nameisnemo","电影");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000108l,"ideamarket","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000338l,"design07","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000794l,"vinwww","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002340l,"xiusheji","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007490l,"monster","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007892l,"crossover","创意");
				GuideRecommendDAO.getInstance().intiRecommendContent(671075602l,"studyad","创意");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000125l,"mycity","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001381l,"mafengwo","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003682l,"qiongyou","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003790l,"qulvxing","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671009574l,"cityphoto","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671023361l,"ieurope","旅行");
				GuideRecommendDAO.getInstance().intiRecommendContent(671078689l,"aishiteru","旅行");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000297l,"naturalstay","摄影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000966l,"original","摄影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001053l,"filmandraw","摄影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001649l,"colourfulmood","摄影");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007053l,"newphotos","摄影");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000120l,"baking","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000767l,"chihe2011","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002498l,"xiachufang","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002534l,"meishike","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671004553l,"cccbbb","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671042898l,"groves","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671060678l,"hongyishifang","美食");
				GuideRecommendDAO.getInstance().intiRecommendContent(671162568l,"personalessen","美食");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000108l,"ideamarket","设计");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002547l,"dddesign","设计");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003110l,"cctv010","设计");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005600l,"hxf1314","设计");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005968l,"japandesign","设计");
				GuideRecommendDAO.getInstance().intiRecommendContent(671129039l,"designerwan","设计");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000107l,"fashion","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000250l,"visualanimal","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671000626l,"nishang","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002690l,"banmeme","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671006773l,"zou1016","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007306l,"lovedapei","时尚");
				GuideRecommendDAO.getInstance().intiRecommendContent(671120906l,"ovogue","时尚");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000853l,"homedeco","家居");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002171l,"mooogu","家居");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003056l,"liankong","家居");
				GuideRecommendDAO.getInstance().intiRecommendContent(671006252l,"voomii","家居");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007875l,"autosave","家居");
				GuideRecommendDAO.getInstance().intiRecommendContent(671105207l,"simplearchi","家居");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671002233l,"17kandonghua","动漫");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003131l,"520dms","动漫");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003191l,"newcartoon","动漫");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005887l,"yubaobaby","动漫");
				GuideRecommendDAO.getInstance().intiRecommendContent(671038714l,"chundongman","动漫");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671001270l,"walges","建筑");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001783l,"architect","建筑");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003056l,"liankong","建筑");
				GuideRecommendDAO.getInstance().intiRecommendContent(671004742l,"xiushe","建筑");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005600l,"hxf1314","建筑");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000219l,"supercar","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671006153l,"97qiyou","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671009114l,"suv4x4","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671009228l,"cheshang","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671042937l,"laoyeche","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671044024l,"iceteaanf","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671091460l,"classiccar","汽车");
				GuideRecommendDAO.getInstance().intiRecommendContent(671102965l,"newglobalcars","汽车");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000191l,"nannanguodong","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671004456l,"7758521","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671009631l,"lovelycats","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671020439l,"aimaoshu","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671050629l,"thumiaomiao","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671054545l,"guineapig","宠物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671102618l,"comeon24","宠物");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671095914l,"xiaoshagua","艺术品");
				GuideRecommendDAO.getInstance().intiRecommendContent(671120169l,"designalliance","艺术品");
				GuideRecommendDAO.getInstance().intiRecommendContent(671188642l,"francniwajiangjiang","艺术品");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671057435l,"xingzuotai","星座");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671006381l,"ilovescience","科学");
				GuideRecommendDAO.getInstance().intiRecommendContent(671013379l,"scientist","科学");
				GuideRecommendDAO.getInstance().intiRecommendContent(671018142l,"jiemiziranmima","科学");
				GuideRecommendDAO.getInstance().intiRecommendContent(671062753l,"mathart","科学");
				GuideRecommendDAO.getInstance().intiRecommendContent(671112637l,"naturetech","科学");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671069626l,"militaryfans","军事");
				GuideRecommendDAO.getInstance().intiRecommendContent(671125418l,"militaryak47","军事");
				GuideRecommendDAO.getInstance().intiRecommendContent(671142362l,"defence","军事");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671002516l,"gamertribe","游戏");
				GuideRecommendDAO.getInstance().intiRecommendContent(671004019l,"youxikong","游戏");
				GuideRecommendDAO.getInstance().intiRecommendContent(671005119l,"67xiaopang","游戏");
				GuideRecommendDAO.getInstance().intiRecommendContent(671035035l,"independentgame","游戏");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671036038l,"xubin122","篮球");
				GuideRecommendDAO.getInstance().intiRecommendContent(671068654l,"reveriegeneration1","篮球");
				GuideRecommendDAO.getInstance().intiRecommendContent(671130100l,"basketballneverstop","篮球");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671004386l,"caomuguangyin","自然");
				GuideRecommendDAO.getInstance().intiRecommendContent(671006512l,"tianwenxue","自然");
				GuideRecommendDAO.getInstance().intiRecommendContent(671145531l,"nationalgeographic","自然");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671002528l,"zhipin","购物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007109l,"taobaodaren","购物");
				GuideRecommendDAO.getInstance().intiRecommendContent(671084948l,"moto520","购物");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671186390l,"wuroubuhuanzhenzong","厨艺");
				GuideRecommendDAO.getInstance().intiRecommendContent(671208473l,"sifangcaisong","厨艺");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671000367l,"chipsmao","文艺");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001350l,"lianwu","文艺");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001393l,"xinlingzhiyu","文艺");
				GuideRecommendDAO.getInstance().intiRecommendContent(671001921l,"hellotime","文艺");
				GuideRecommendDAO.getInstance().intiRecommendContent(671002214l,"daocaorenshouji","文艺");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671003471l,"duanpiankong","短片");
				GuideRecommendDAO.getInstance().intiRecommendContent(671007317l,"lee123","短片");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671001941l,"hanqian2011","武器");
				GuideRecommendDAO.getInstance().intiRecommendContent(671022874l,"punish","武器");
				GuideRecommendDAO.getInstance().intiRecommendContent(671037060l,"blackwarrior","武器");
				GuideRecommendDAO.getInstance().intiRecommendContent(671038855l,"fighter","武器");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671002690l,"banmeme","服饰");
				GuideRecommendDAO.getInstance().intiRecommendContent(671039137l,"itsfan","服饰");
				GuideRecommendDAO.getInstance().intiRecommendContent(671186966l,"fushidapeikong","服饰");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671002948l,"engadget","数码");
				GuideRecommendDAO.getInstance().intiRecommendContent(671003630l,"shishangshuma","数码");
				GuideRecommendDAO.getInstance().intiRecommendContent(671010233l,"peterz","数码");
				
				GuideRecommendDAO.getInstance().intiRecommendContent(671003792l,"phoenixism","互联网");
				GuideRecommendDAO.getInstance().intiRecommendContent(671020126l,"socialfind","互联网");
				GuideRecommendDAO.getInstance().intiRecommendContent(671027208l,"solidot","互联网");
				
				
				
				
			} catch (SQLException e1) {
				e1.printStackTrace();
			}
			

			
			return "@success";
		}

		return "@Inner Required";
	}
    
    
    
    
    
	 public SiteInfo getXiaoZhanPojoById(long siteId){
	        final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
	        final com.renren.sns.minisite.xoa.client.SiteService service = fact
	                .getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
	        //参数小站id
	        ServiceFuture<SiteInfo> future = service.getSite(new Integer(siteId+"")); 
	        SiteInfo ret = ServiceFutureHelper.execute(future, 10000);
	        return ret;

	    }
}
