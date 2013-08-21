package com.renren.sns.guide.controllers;



import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.platform.core.model.User;

@LoginRequired
@Path({ "guiderecommend" })
@IgnoreUserStatusImportedValidation
public class GuideHomePageRecInsterestController {
    
	@Autowired
	private  GuideRecommendLogic guideRecommendLogic;


    @Get
    public String get(Invocation inv) {
        return index(inv);
    }
    //guide 首面获到推荐内容所调的方法
    @Post
    public String index(Invocation inv) {

        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
       
        User host = GuideXFiveRequestUtil.getCurrentHost(request);
        int userid = host.getId();

        int num = 0;//要获取推荐内容数量
        int contentType = 0; //推荐内容类型
        String strContentType = request.getParameter("contentType");
        String strNum = request.getParameter("num");
        String from = request.getParameter("from");

        //如果num没有传或都 为null默认是 4
        if (strNum == null || "".equals(strNum) || (strNum.matches("[0-9]+") == false)) {
            num = 4;
        } else {
            num = Integer.parseInt(strNum);
        }
        //如果contentType没有传或都 为null默认是 1  contentType是内容的类别，目前还没有用到，因为里面只有小站的内容。
        if (strContentType == null || "".equals(strContentType)
                || (strContentType.matches("[0-9]+") == false)) {
            contentType = 1;
        } else {
            contentType = Integer.parseInt(strContentType);
        }
        
        if(StringUtils.isBlank(from)){
        	from ="guide_home";
        }
        //推荐位更改推荐位测略，当展示和点击不再往guide_recommend_content_view_log里面插数据只在guide_recommend_content统计数据。
        //当用户关注一个小站后再推一个同一兴趣下的小站。只用判断num为1的情况
        if(num==1){
        	 String tag=request.getParameter("tag");
        	 GuideLogger.printLog("tag:"+tag);
        	 request.setAttribute("recommendcontentlist",  guideRecommendLogic.getSameRecommendContent(userid,num,contentType,from,tag));

             return "/views/guide/v6/body/guide_v6_recommend.jsp";
        }
        
        request.setAttribute("recommendcontentlist",  guideRecommendLogic.getRecommendContentList(userid,num,contentType,from));

        return "/views/guide/v6/body/guide_v6_recommend.jsp";

    }
}
