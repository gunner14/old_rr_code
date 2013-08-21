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
import com.xiaonei.platform.component.friends.home.RequestFriendLogic;
import com.xiaonei.platform.component.friends.util.FriendsUtil;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-10 上午09:58:15 类说明
 */
@Path( { "GuideBeforeRequestFriend.do" })
@LoginRequired
public class AjaxGuideBeforeRequestFriendController {

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
        JSONObject jo = new JSONObject();;
        User host = null;
        try {
            host = this.hostHolder.getUser();
            if (host == null) {
                throw new Exception("get host error");
            }
        } catch (Exception e) {
            e.printStackTrace();            
            System.err.println("get host error");
            jo.put("result", -1);
            writeAjaxStream(response, jo.toString());
            return null;
        }
        /** 输入验证码就验证 */
        if ("checkCode".equals(request.getParameter("method"))) {
            String inputCode = request.getParameter("code");
            boolean flag = checkCode(inputCode, host.getId());
            if (flag) {
                jo = new JSONObject();
                jo.put("result", 0);
                writeAjaxStream(response, jo.toString());
                return null;
            } else {
                jo = new JSONObject();
                jo.put("result", 1);
                writeAjaxStream(response, jo.toString());
                return null;
            }
        } else {
            try {
                int guestId = Integer.parseInt(request.getParameter("id"));
                User guest = SnsAdapterFactory.getUserAdapter().get(guestId);
                jo = new JSONObject();
                jo.put("isStar", host.isStarUser());
                if (guest.getGender().contains("男")) {
                    jo.put("gender", 1);
                } else if (guest.getGender().contains("女")) {
                    jo.put("gender", 0);
                } else {
                    jo.put("gender", 2); // 页面中显示应该是TA
                }
                boolean needCheckCode = RequestFriendLogic.getInstance().needCheckCode(host);
                boolean isConfirmed = UserConfirmLogic.getInstance().isConfirmed(host);
                // 用户id组
                String user_id = request.getParameter("uids");
                String[] user_ids = { "" };
                if (user_id != null && !"".equalsIgnoreCase(user_id)) {
                    user_ids = user_id.split(",");
                }
                final String[] ids = user_ids;
                if (needCheckCode
                        || (ids != null && ids.length > GuideFriendDefine.MAX_ADD_FRIENDS)) {
                    jo.put("result", 1);
                } else {
                    jo.put("result", 0);
                }
                jo.put("confirm", isConfirmed);
                jo.put("isGuide", FriendsUtil.getInstance().isGuideUser(host));
                jo.put("hostId", host.getId());
                jo.put("guestId", guestId);
                writeAjaxStream(response, jo.toString());
                return null;
            } catch (Exception e) {
                e.printStackTrace();                
                jo = new JSONObject();
                jo.put("result", -1);
                writeAjaxStream(response, jo.toString());
                return null;
            }
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

    private boolean checkCode(String inputCode, int id) {
        try {
            MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_icode);
            String cacheCode = (String) mcm
                    .get(MemCacheKeys.key_icode_user + "requestfriend_" + id);
            if (inputCode == null || cacheCode == null || inputCode.length() == 0
                    || cacheCode.length() == 0 || !cacheCode.equals(inputCode.toUpperCase())) {
                return false;
            } else {
                return true;
            }
        } catch (Exception e) {            
        }
        return false;
    }

    public String toString() {
        return "rose AjaxBeforeRequestFriendController";
    }
}
