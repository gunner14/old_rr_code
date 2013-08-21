package com.xiaonei.reg.guide.flows.xfive.controllers;

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
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * @author 黄斌(bin.huang1@renren-inc.com)
 */
@LoginRequired
@Path({ "flowrecmodifyz3step" })
@IgnoreUserStatusImportedValidation
public class FlowRecModStepZ3Controller {

	@Autowired
	private GuideRecommendLogic guideRecommendLogic;

	@Get
	@Post
	public String get(Invocation inv) {
		HttpServletRequest request = inv.getRequest();
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		int userId = host.getId();
		String gender = host.getGender();
		// 改标志
		try {
			StepStatusLogic.updateStepStatus(userId, StepStatusLogic.STEP_FlOW_RECOMMEND);
			GuideLogger.printLog("| host:" + userId + " update step:" + StepStatusLogic.STEP_FlOW_RECOMMEND);
		} catch (Exception e) {
			GuideLogger.printLog(e.getMessage());
			e.printStackTrace();
		}

		// 跳过
		String skip = request.getParameter("skip");
		if (skip != null && "1".equals(skip)) {
			return "f:lead";
		}

		// 获取关注话题
		String interestNmaes = request.getParameter("interestnames");
		if (interestNmaes == null || interestNmaes.length() <= 0) {
			GuideLogger.printLog("interestNmaes is not correct tagValues:" + interestNmaes);
			return "f:lead";
		}
		GuideLogger.printLog("interestNmaes:" + interestNmaes);
		String[] tagValues = interestNmaes.split(",");

		// 批量关注
		try {
			guideRecommendLogic.batchFoucsTag(tagValues, userId);
			int sexFlag = 0;
			if (gender != null && gender.equals("男生")) {
				sexFlag = 0;
			}
			if (gender != null && gender.equals("女生")) {
				sexFlag = 1;
			}
			// 统计关注
			guideRecommendLogic.addFoucsCount(tagValues, sexFlag,host.getId());
			// 给用户发送最新feed
			guideRecommendLogic.sendTagFeedToUser(userId, tagValues, 5);
		} catch (Exception e) {
			e.printStackTrace();
		}
		

		return "f:lead";
	}
}
