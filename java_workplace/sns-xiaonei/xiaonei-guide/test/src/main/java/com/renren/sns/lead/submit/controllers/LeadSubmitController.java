package com.renren.sns.lead.submit.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.dao.VerifyCodeDAO;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.RandCodeUtil;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

@LoginRequired
@Path({ "leads" })
@IgnoreUserStatusImportedValidation
public class LeadSubmitController {

	private static final String ACT_MergeUser_Commit_VerifyCode = "mu_commit_vcode";
	private static final String ACT_MergeUser_Resend_VerifyCode = "mu_resend_vcode";
	
	@Get
	public String ngIndex(Invocation inv, CacheManager cm) {
		User host = UserProvider.getInstance().get(cm);
		String action = inv.getParameter("act");
		GuideLogger.printLog(" host:" + host.getId() + " act:" + action+" getmethod!notallowed", GuideLogger.WARN);
		return "r:lead";
	}

	@Post
	public String doIndex(Invocation inv, CacheManager cm) {
		User host = UserProvider.getInstance().get(cm);
		String action = inv.getParameter("act");
		String ret_url = "";
		if (ACT_MergeUser_Commit_VerifyCode.equals(action)) {
			String vcode = inv.getParameter("vcode");
			ret_url = doMuCommitVcode(host,vcode,inv);
		} else if (ACT_MergeUser_Resend_VerifyCode.equals(action)) {
			ret_url = doMuResendVcode(host);
		} else {
			GuideLogger.printLog(" host:" + host.getId() + " act:" + action, GuideLogger.WARN);
		}
		GuideLogger.printLog(" host:" + host.getId() + " act:" + action + " to:" + ret_url);
		return ret_url;
	}

	private String doMuCommitVcode(User host,String vcode , Invocation inv) {
		GuideLogger.printLog(" host:" + host.getId() + " vcode:" + vcode+" commited");
		String to_url = "r:lead";
		if(MyStringUtils.isNullOrEmpty(vcode)){
			vcode = "";
			to_url = "r:lead?e=1"; //需要验证码
			GuideLogger.printLog(" host:" + host.getId() + " vcode1:" + vcode);
		}
		else{
			//验证成功 合并
			vcode = vcode.trim();
			String curVC = VerifyCodeDAO.getVerifyCodeByUserId(host.getId());
			if(curVC.equals(vcode)){
				//换码
				String newvc = RandCodeUtil.getRandChar(8, 9);
				VerifyCodeDAO.updateUserVerifyKey(newvc, host.getId());
				
				//封号
				//com.xiaonei.admin.common.logic.user.BanUserLogic.banUserId(host.getId(), 223158817, com.xiaonei.admin.biz.model.user.BanRecord.BanMark.MERGE_USER, "糯米用户合并舍弃账号");
				GuideLogger.printLog(" host:" + host.getId() + " ban:" + host.getId(),GuideLogger.WARN);
				
				//换Host
				User newuser = StepProcessLogic.getInstance().getWillMergeToUser(host);
				if(newuser != null){
					LoginLogic.getInstance().doLoginR(newuser, inv.getRequest(), inv.getResponse(), 0, false);
					GuideLogger.printLog(" host:" + host.getId() + " changetohost:" + newuser.getId());
				}
				else{
					to_url = "r:lead?e=2";
					GuideLogger.printLog(" host:" + host.getId() + " changetohost:" + newuser,GuideLogger.WARN);
				}
			}
			else{
				to_url = "r:lead?e=2";
				GuideLogger.printLog(" host:" + host.getId() + " vcode2:" + vcode);
			}
		}
		
		return to_url;
	}
	private String doMuResendVcode(User host) {
		RegEmailLogic.getInstance().sendMergeEmail(host);
		GuideLogger.printLog(" host:" + host.getId() +" resended");
		return "@OK";
	}
}
