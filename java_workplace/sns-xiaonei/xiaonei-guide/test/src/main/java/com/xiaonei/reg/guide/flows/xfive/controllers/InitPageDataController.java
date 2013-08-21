package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.io.File;
import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

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
@Path({"initpagedata"})
@IgnoreUserStatusImportedValidation
public class InitPageDataController {
    
    @Autowired
    private GuideRecommendJadeDAO guideRecommendJadeDAO;

   
    
    @Get
	@Post
	public String get(Invocation inv) {

		String curip = BizFilterUtil.getClientIP(inv.getRequest());
		if (curip != null && curip.trim().startsWith("10.")) {

			try {
				guideRecommendJadeDAO.deletePageContent();
				
				
					Workbook book = Workbook.getWorkbook(new File("0709.xls"));

					// 获得第一个工作表对象
					Sheet sheet = book.getSheet(0);
					int columns=sheet.getColumns();
					System.out.println("columns:"+columns);
					int rows=sheet.getRows();
					System.out.println("rows:"+rows);
					
					int pageId=0;
					String pageName="";
					int universityId=0;
					String universityName="";
					String recommendInfo="";
					int pageType=0;
					String remarks="";
					
					String pageTypeStr="";
					
					Cell cellKey=null;
					
					// 得到第一列第一.二行的单元格
					for(int i=1;i<rows;i++){
						
						cellKey = sheet.getCell(0, i);
						universityId = Integer.parseInt(cellKey.getContents());
						System.out.println(universityId);
						
						cellKey = sheet.getCell(1, i);
						universityName = cellKey.getContents();
						System.out.println(universityName);
						
						cellKey = sheet.getCell(2, i);
						pageName = cellKey.getContents();
						System.out.println(pageName);
						
						cellKey = sheet.getCell(3, i);
						pageId = Integer.parseInt(cellKey.getContents());
						System.out.println(pageId);
						
						cellKey = sheet.getCell(4, i);
						recommendInfo = cellKey.getContents();
						System.out.println(recommendInfo);
						
						cellKey = sheet.getCell(5, i);
						pageTypeStr=cellKey.getContents();
						if("默认关注".equals(pageTypeStr.trim())){
							pageType=0;
						}
						else if("推荐关注".equals(pageTypeStr.trim())){
							pageType=1;
						}
						System.out.println(pageType);
						
						cellKey = sheet.getCell(6, i);
						remarks = cellKey.getContents();
						System.out.println(remarks);
						
						guideRecommendJadeDAO.insertPageContent(pageId,
								pageName, universityId, universityName, recommendInfo, pageType, remarks);
						System.out.println("***********");
					}
					
					

					book.close();
					System.out.println("查询EXCEL文件结束!");
				
			} catch (SQLException e1) {
				e1.printStackTrace();
			} catch (BiffException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			

			
			return "@success";
		}

		return "@Inner Required";
	}
    
}
