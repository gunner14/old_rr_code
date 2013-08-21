package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.guide.model.GuideFriendDefine;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.icode.verify.CodeValidateInfo;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.home.RequestFriendLogic;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.component.friends.model.FriendApplyResult;
import com.xiaonei.platform.component.request.define.RequestConstants;
import com.xiaonei.platform.component.request.logger.RequestRemoteLogger;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path( { "ajax_guide_request_friend.do" })
@LoginRequired
public class AjaxGuideRequestFriendController {

    @Autowired
    private HostHolder hostHolder;

    @Get
    public String get(Invocation inv) {
        HttpServletResponse response = inv.getResponse();
        JSONObject jo = null;
        jo = new JSONObject();
        jo.put("result", -1);
        writeAjaxStream(response, jo.toString());
        return null;

    }

    @Post
    public String post(Invocation inv) {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        User host = null;
        try {
            host = this.hostHolder.getUser();
            if (host == null) {
                throw new Exception("get host error");
            }
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }

        try {
            boolean needCheckCode = RequestFriendLogic.getInstance().needCheckCode(host);
            if (needCheckCode) { // 每天发好友申请超过限制数 或者
                String valueIcode = request.getParameter("code");
                CodeValidateInfo cvi = CodeUtil.getInstance().validCodeFull(valueIcode,"requestfriend_", String.valueOf(host.getId()));
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
            // 打一下远程log
            try {
                RequestRemoteLogger.getInstance().log(host.getId(), RequestConstants.FRIEND_APPLY, RequestRemoteLogger.SEND);
            } catch (Exception e) {
                System.err.println(e.toString());
            }
            int id;
            try {
                id = Integer.parseInt(request.getParameter("id"));
                if (host.getId() == id) {
                    retErr(response, "自己加自己为好友!");
                    return null;
                }
            } catch (Exception e) {
                retErr(response, "该用户不存在。");
                return null;
            }
            String why = request.getParameter("why");
            String from = "";
            try {
                from = request.getParameter("from");
            } catch (Exception e) {
                from = "";
            }
            if (from == null || "".equals(from)) {
                GuideLogger.printLog(" host id is:" + host.getId() + "from is null");
                return "@";
            } else if ("_".equals(from)) {
                GuideLogger.printLog(" host id is:" + host.getId() + "from is _");
            }
            if ("sg_guide_online".equals(from)) {
                GuideLogger.printLog("host:" + host.getId() + "=>sg_guide_online", GuideLogger.WARN);
                return "@";
            }
            User to = null;
            try {
                to = SnsAdapterFactory.getUserAdapter().get(id);
            } catch (Exception e) {
                System.err.println(e.toString());
            }
            if ((to == null) || (host == null)) {
                retErr(response, "该用户不存在。");
                return null;
            }
            String domain = OpiConstants.getCurrentDomain();
            GuideLogger.printLog(" host:" + host.getId() + " stage:"+host.getStage()+" request_count: 1"  + " from_flag:" + from);
            GuideLogger.printLog("AjaxGuideRequestFriendController:host:" + host.getId());
            FriendApply friendApply = new FriendApply(host, to, domain);
            friendApply.setContent(why);
            friendApply.setFrom(from);
            FriendApplyResult friendApplyResult = FriendsHome.getInstance().requestFriendApply(friendApply);
            returnMessage(friendApplyResult, response);
        } catch (Exception e) {
            System.err.println(e.toString());
        }
        return null;
    }

    private void returnMessage(FriendApplyResult friendApplyResult, HttpServletResponse response) {
        JSONObject jo = new JSONObject();
        jo.put("code", friendApplyResult.getResult());
        jo.put("message", friendApplyResult.getMessage());
        try {
            jo.put("targetfriendcount", FriendsHome.getInstance().getFCount(friendApplyResult.getFriendApply().getRecipient().getId()));
        } catch (Exception e) {
            System.err.println(e.toString());
        }
        jo.put("ta", friendApplyResult.getFriendApply().getRecipient().getGender());
        jo.put("id", friendApplyResult.getFriendApply().getRecipient().getId());
        jo.put("name", friendApplyResult.getFriendApply().getRecipient().getName());
        jo.put("tiny_head_url", friendApplyResult.getFriendApply().getRecipient().getTinyFullUrl());
        jo.put("hostIsVip", friendApplyResult.getFriendApply().getApplicant().isVipMember());
        jo.put("guestIsVip", friendApplyResult.getFriendApply().getRecipient().isVipMember());
        writeAjaxStream(response, jo.toString());
    }

    private void retErr(HttpServletResponse response, String message) {
        try {
            JSONObject jo = new JSONObject();
            jo.put("code", 2);
            jo.put("message", message);
            PrintWriter writer = response.getWriter();
            writer.print(jo.toString());
            writer.flush();
            writer.close();
        } catch (IOException e) {

        }
    }

    private void writeAjaxStream(final HttpServletResponse response, final String str) {
        try {
            response.setContentType("text/html; charset=UTF-8");
            PrintWriter writer = response.getWriter();
            writer.print(str);
            writer.flush();
            writer.close();
        } catch (IOException e) {

        }
    }

    public String toString() {
        return "rose AjaxGuideRequestFriendController";
    }

}
