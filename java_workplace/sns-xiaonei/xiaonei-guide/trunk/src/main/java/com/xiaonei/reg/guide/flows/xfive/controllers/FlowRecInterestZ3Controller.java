package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.pojo.GuideZ3Pojo;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * @author 黄斌(bin.huang1@renren-inc.com)
 */
@LoginRequired
@Path({ "flowrecommendz3interest" })
@IgnoreUserStatusImportedValidation
public class FlowRecInterestZ3Controller {

	@Autowired
	private GuideRecommendLogic guideRecommendLogic;

	private static final String SUCC_DEFAULT = "/pages/guide/guiderecommendflow/guidez3recommend.jsp";

	/**
	 * 此方法返回兴趣页面_Z3流程。
	 * 
	 * @param inv
	 * @return
	 */
	@Get
	@Post
	public String get(Invocation inv) {
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		if (host != null && host.getStatus() == 10 && host.getDomain().contains("imop")) {
			GuideLogger.printLog("returntoregister id:" + host.getId() + " domain:" + host.getDomain() + " status:" + host.getStatus());
			return "r:http://reg.renren.com/sb/fillemail";
		}

		int sexFlag = 0;// 0 男，1 女
		if (host != null) {
			String gender = host.getGender();
			if (gender != null && gender.equals("男生")) {
				sexFlag = 0;
			}
			if (gender != null && gender.equals("女生")) {
				sexFlag = 1;
			}
			inv.getRequest().setAttribute("userId", host.getId());
		}

		List<GuideZ3Pojo> guideZ3List = guideRecommendLogic.getGuideZ3List(sexFlag);
		inv.getRequest().setAttribute("sexFlag", sexFlag);
		inv.getRequest().setAttribute("guideZ3List", guideZ3List);
		return SUCC_DEFAULT;
	}
}
