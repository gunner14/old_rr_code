package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.sql.SQLException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;

/**
 * @todo 初始化Z3流程引导兴趣数据
 * @author bin.huang1@renren-inc.com
 */
@LoginRequired
@Path({ "initz3data" })
@IgnoreUserStatusImportedValidation
public class InitZ3DataController {

	@Autowired
	private GuideRecommendJadeDAO guideRecommendJadeDAO;

	private static int sex_boy = 0;
	private static int sex_girl = 1;

	@Get
	@Post
	public String get(Invocation inv) {

		String curip = BizFilterUtil.getClientIP(inv.getRequest());
		if (curip != null && curip.trim().startsWith("10.")) {

			try {
				guideRecommendJadeDAO.deleteGuideZ3Content();

				// 男生初始化数据
				guideRecommendJadeDAO.insertGuideZ3Content("电影", 2, sex_boy, "dianying.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("音乐", 3, sex_boy, "yinyue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("数码", 9, sex_boy, "shuma.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("旅行", 10, sex_boy, "lvxing.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("创意", 1, sex_boy, "chuangyi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("游戏", 8, sex_boy, "youxi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("科技", 4, sex_boy, "keji.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("萌", 5, sex_boy, "meng.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("摄影", 6, sex_boy, "sheying.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("设计", 7, sex_boy, "sheji.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("建筑", 11, sex_boy, "jianzhu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("动漫", 12, sex_boy, "dongman.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("美食", 13, sex_boy, "meishi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("阅读", 14, sex_boy, "yuedu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("科学", 15, sex_boy, "kexue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("体育", 16, sex_boy, "tiyu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("文学", 17, sex_boy, "wenxue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("时尚", 18, sex_boy, "shishang.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("汽车", 19, sex_boy, "qiche.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("家居", 20, sex_boy, "jiaju.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("视觉", 21, sex_boy, "shijue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("军事", 22, sex_boy, "junshi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("生活", 23, sex_boy, "shenghuo.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("宠物", 24, sex_boy, "chongwu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("插画", 25, sex_boy, "chahua.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("广告", 26, sex_boy, "guanggao.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("艺术", 27, sex_boy, "yishu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("恋物", 28, sex_boy, "lianwu.jpg", 0);
				// 女生初始化数据
				guideRecommendJadeDAO.insertGuideZ3Content("电影", 2, sex_girl, "dianying.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("音乐", 3, sex_girl, "yinyue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("美食", 9, sex_girl, "meishi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("旅行", 10, sex_girl, "lvxing.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("家居", 1, sex_girl, "jiaju.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("阅读", 8, sex_girl, "yuedu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("复古", 4, sex_girl, "fugu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("搭配", 5, sex_girl, "dapei.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("摄影", 6, sex_girl, "sheying.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("插画", 7, sex_girl, "chahua.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("动漫", 11, sex_girl, "dongman.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("时尚", 12, sex_girl, "shishang.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("创意", 13, sex_girl, "chuangyi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("视觉", 14, sex_girl, "shijue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("生活", 15, sex_girl, "shenghuo.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("萌", 16, sex_girl, "meng.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("宠物", 17, sex_girl, "chongwu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("设计", 18, sex_girl, "sheji.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("文学", 19, sex_girl, "wenxue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("汽车", 20, sex_girl, "qiche.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("艺术", 21, sex_girl, "yishu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("科学", 22, sex_girl, "kexue.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("科技", 23, sex_girl, "keji.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("建筑", 24, sex_girl, "jianzhu.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("数码", 25, sex_girl, "shuma.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("广告", 26, sex_girl, "guanggao.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("游戏", 27, sex_girl, "youxi.jpg", 0);
				guideRecommendJadeDAO.insertGuideZ3Content("恋物", 28, sex_girl, "lianwu.jpg", 0);
			} catch (SQLException e) {
				GuideLogger.printLog(e.getMessage());
			}
			return "@success";
		}
		return "@Inner Required";
	}
}
