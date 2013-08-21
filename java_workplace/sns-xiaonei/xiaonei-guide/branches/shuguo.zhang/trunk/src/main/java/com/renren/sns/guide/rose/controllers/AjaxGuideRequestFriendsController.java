package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONObject;

import com.kaixin.logic.RunnableTaskExecutor;
import com.renren.sns.guide.model.GuideFriendDefine;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.icode.verify.CodeValidateInfo;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.home.FriendsLogic;
import com.xiaonei.platform.component.friends.home.RequestFriendLogic;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.component.friends.model.FriendApplyResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.wService.userGuide.adapter.UserGuideAdapter;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-7 下午06:22:34 类说明
 */
@IgnoreUserStatusImportedValidation
@LoginRequired
@Path( { "ajaxRequestFriends", "ajaxGuideRequestFriends" })
public class AjaxGuideRequestFriendsController {

	@Get
	public String get(Invocation inv) {
		final HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		String user_id = request.getParameter("friends");
		String from_flag = request.getParameter("from");
		GuideLogger.printLog(" host:" + host.getId() + " request:" + user_id + " from:" + from_flag, GuideLogger.WARN);
		return "@";
	}

	@Post
	public String post(Invocation inv) {
		final HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		JSONObject json = new JSONObject();
		if (request == null || response == null) {
			GuideLogger.printLog(" request or response null " + (request == null) + (response == null), GuideLogger.ERROR);
		} else if (host == null) {
			GuideLogger.printLog(" host null ", GuideLogger.ERROR);
		} else {
			// 用户id组
			String user_id = request.getParameter("friends");
			final String from_flag = request.getParameter("from");
			if ("sg_guide_online".equals(from_flag)) {
				GuideLogger.printLog("host:" + host.getId() + "=>sg_guide_online", GuideLogger.WARN);
				return "@";
			}
			if (MyStringUtils.equals("", host.getName().trim())) {
				GuideLogger.printLog("host:" + host.getId() + " NoName", GuideLogger.WARN);
				Map<String, String> map = new HashMap<String, String>();
				map.put("friends", user_id);
				map.put("from_flag", from_flag);
				UserGuideAdapter.getInstance().push(host.getId(), UserGuideAdapter.NEW_REG_REQUEST_FRIENDS_WITH_NO_NAME, map);
				json.put("code", "1");
				json.put("msg", "");
				return "@" + json.toString();
			}
			String[] user_ids = { "" };
			if (user_id != null && !"".equalsIgnoreCase(user_id)) {
				user_ids = user_id.split(",");
			}
			final String[] ids = user_ids;
			if (from_flag == null || "".equals(from_flag)) {
				if (ids != null) {
					GuideLogger.printLog(" host:" + host.getId() + ";from_flag is not null;ids.length:" + ids.length);
				} else {
					GuideLogger.printLog(" host:" + host.getId() + ";from_flag is null;ids is null");
				}
				return "@";
			} else if ("_".equals(from_flag)) {
				if (ids != null) {
					GuideLogger.printLog(" host:" + host.getId() + ";from_flag is _;ids.length:" + ids.length);
				} else {
					GuideLogger.printLog(" host:" + host.getId() + ";from_flag is _;ids is null");
				}
			}
			if (ids != null && ids.length > 0) {
				boolean needCheckCode = RequestFriendLogic.getInstance().needCheckCode(host);
				if (needCheckCode || (ids != null && ids.length > GuideFriendDefine.MAX_ADD_FRIENDS)) { // 每天发好友申请超过限制数 或者
					String valueIcode = request.getParameter("code");
					CodeValidateInfo cvi = CodeUtil.getInstance().validCodeFull(valueIcode, "requestfriend_", String.valueOf(host.getId()));
					GuideLogger.printLog(" cvi.isValidate():" + cvi.isValidate());
					GuideLogger.printLog(" cvi.getRank():" + cvi.getRank());
					GuideLogger.printLog(" GuideFriendDefine.CODE_VALIDATE_RANK:" + GuideFriendDefine.CODE_VALIDATE_RANK);
					if (!(cvi.isValidate() && cvi.getRank() >= GuideFriendDefine.CODE_VALIDATE_RANK)) {
						JSONObject jo = new JSONObject();
						jo.put("result", -1);
						jo.put("info", "check code input error");
						this.writeAjaxStream(response, jo.toString());
						return null;
					}
				}
			}
			try {
				final String currentDomain = OpiConstants.domain.toString();
				//异步请求开始
				Runnable taskSave_NET = new Runnable() {
					public void run() {
						boolean error = false;
						GuideTimeCost cost = GuideTimeCost.begin();
						cost.endStep();
						if (ids != null && ids.length > 0) {// 分发多个请求                           
							GuideLogger.printLog(" host:" + host.getId() + " request_count:" + ids.length + " from_flag:" + from_flag);
							for (int i = 0; i < ids.length; i++) {
								cost.endStep();
								int accepterId = 0;
								try {
									accepterId = Integer.parseInt(ids[i]);
								} catch (NumberFormatException e) {
									BaseActionUtil.addMsg(request, "您没有向任何人发出邀请");
									error = true;
									break;
								}
								cost.endStep();
								if (accepterId > 0) {
									User touser = SnsAdapterFactory.getUserAdapter().get(accepterId);
									if (touser != null) {
										FriendApply fa = new FriendApply(host, touser, currentDomain);
										fa.setFrom(from_flag);
										try {
											FriendApplyResult rst = FriendsHome.getInstance().requestFriendApply(fa);
											String msg = rst.getMessage();
											GuideLogger.printLog(" host:" + host.getId() + " to:" + touser.getId() + " " + msg + ":" + rst.getResult());
										} catch (Exception e) {
											e.printStackTrace();
										}
									}
								}
								cost.endStep();
							}
							cost.endStep();
							if (!error) {
								BaseActionUtil.addMsg(request, "已经成功向" + ids.length + "人发出了添加为好友的请求，请等待对方确认");
							}
							cost.endStep();
						} else {
						}
						cost.endFinally();
					}
				};
				RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
				//异步请求结束
				//先判断当天发好友申请数量
				int todayRequestCount = FriendsLogic.getInstance().getTodayRequestFriendCount(host.getId());
				if (todayRequestCount > RequestFriendLogic.TODAY_REQUEST_FRIEND_MAX) {
					json.put("code", "99");
					json.put("msg", FriendApplyResult.DEFAULT_TODAY_REQUEST_FULL_MESSAGE);
				} else {
					json.put("code", "1");
					json.put("msg", "");
				}
			} catch (Exception e1) {
				e1.printStackTrace();
			}
		}
		return "@" + json.toString();
	}

	private void writeAjaxStream(final HttpServletResponse response, final String str) {
		try {
			response.setContentType("text/html; charset=UTF-8");
			PrintWriter writer = response.getWriter();
			writer.print(str);
			writer.flush();
			writer.close();
		} catch (IOException e) {
			System.err.println("AjaxGuideRequestFriendsController:" + e.toString());
		}
	}
}
