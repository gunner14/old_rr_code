package com.dodoyo.invite.core.logic;

import java.io.UnsupportedEncodingException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.dodoyo.invite.core.entity.InviteParams;
import com.xiaonei.album.model.Album;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

public class InviteParamsUtils {
    
    public InviteParamsUtils() {
    }

    public InviteParams getInviteParams(HttpServletRequest request) {
        InviteParams ip = new InviteParams();
        User host = BaseThreadUtil.currentHost(request);
        String app_html = request.getParameter("app_html");
        String url = request.getParameter("url");
        ip.setInviterId(host.getId());
        ip.setAppHtml(app_html);
        ip.setRetURL(url);
        return ip;
    }

    public boolean checkInviteParams(InviteParams ip) {
        return true;
    }

    /**
     * 初始化一些准备参数，供后面uuid使用
     *
     * @param ip
     * @return
     */
    public InviteParams updateInviteParamsForPrepare(InviteParams ip) {
        LogUtils.logDebug("appId = " + ip.getAppId());
        if (ip.getAppId() != null && !"".equals(ip.getAppId()) && !"0".equals(ip.getAppId())) {
            ip.setAppToken("");
        }
        ip.setBuddyGroup("bgroup");
        return ip;
    }

    public Map<String, Object> getDatasForVMReplace(InviteParams ip, User host, int sendType, String customMsg,
                                                    String email, int appId, String appToken, String someTips,
                                                    String classContents, String ss)
            throws SQLException, UnsupportedEncodingException {
        HashMap<String, Object> map = new HashMap<String, Object>();
        map.put("album_count", /*AlbumDAO.getInstance().getPhotoCount(host.getId())*/"0");
        map.put("gossip_count", /*GossipLogic.getCount(host.getId(),"recent")*/"0");
        map.put("customMessage", customMsg);
        map.put("hostname", host.getName());
        map.put("hostid", host.getId());
        map.put("headurl", /*host.getHead_Url()*/"");
        map.put("headurl_2", /*host.getHeadUrl()*/"");
        map.put("friendscount", /*host.getFriendCount()*/0);
        map.put("someTips", someTips);
        List<WUserCache> friendList = null;//FriendsHome.getInstance().getFriends(host, 0, 4);
        List<WUserCache> friendList_2 = null;//FriendsHome.getInstance().getFriends(host, 0, 4);
        map.put("friendList", friendList);
        map.put("friendList_2", friendList_2);
        List<Album> albumList = new ArrayList<Album>();//AlbumDAO.getInstance().getByUser(host.getId(), 99, 0, 3);
        map.put("albumList", albumList);
        map.put("mills", System.currentTimeMillis() / 1000);
        map.put("t", sendType);
        String monitorUrl = OpiConstants.urlReg + "/register.do?i="
                + host.getId() + "&e=" + email;
        map.put("monitorUrl", java.net.URLEncoder.encode(monitorUrl, "UTF-8"));
        String monitorUrlApp = OpiConstants.urlReg + "/register.do?i=" + host.getId() + "&e=" + email + "&appId=" + appId + "&appToken=" + appToken;
        map.put("monitorUrlApp", java.net.URLEncoder.encode(monitorUrlApp, "UTF-8"));
        map.put("monitorUrlMain", java.net.URLEncoder.encode(OpiConstants.urlWww.toString(), "UTF-8"));
        String monitorUrl_temp_1266 = OpiConstants.urlReg + "/register.do?i="
                + host.getId() + "&e=" + email + "&ss=21266";
        map.put("monitorUrl_temp_1266", java.net.URLEncoder.encode(monitorUrl_temp_1266, "UTF-8"));
        String monitorUrl_temp_1288 = OpiConstants.urlReg + "/register.do?i="
                + host.getId() + "&e=" + email + "&ss=21268";
        map.put("monitorUrl_temp_1288", java.net.URLEncoder.encode(monitorUrl_temp_1288, "UTF-8"));

        String monitorUrlApp_temp_kaixin = OpiConstants.urlReg + "/register.do?i=" + host.getId() + "&ss=31219" + "&e=" + email + "&appId=" + appId + "&appToken=" + appToken;
        map.put("monitorUrlApp_temp_kaixin", java.net.URLEncoder.encode(monitorUrlApp_temp_kaixin, "UTF-8"));
        String monitorUrlApp_temp_old = OpiConstants.urlReg + "/register.do?i=" + host.getId() + "&ss=31268" + "&e=" + email + "&appId=" + appId + "&appToken=" + appToken;
        map.put("monitorUrlApp_temp_old", java.net.URLEncoder.encode(monitorUrlApp_temp_old, "UTF-8"));
        map.put("email_to", email);
        String monitorUrl_31255 = OpiConstants.urlReg + "/register.do?i="
                + host.getId() + "&e=" + email + "&ss=" + ss;
        map.put("monitorUrl_31255", java.net.URLEncoder.encode(monitorUrl_31255, "UTF-8"));
        String monitorUrl_31257 = OpiConstants.urlReg + "/register.do?i="
                + host.getId() + "&e=" + email + "&ss=" + ss + "&appId=" + appId + "&appToken=" + appToken;
        map.put("monitorUrl_31257", java.net.URLEncoder.encode(monitorUrl_31257, "UTF-8"));
        Calendar c = Calendar.getInstance();
        map.put("year", c.get(Calendar.YEAR));
        map.put("month", c.get(Calendar.MONTH));
        map.put("day", c.get(Calendar.DATE));
        if (classContents != null && !"".equalsIgnoreCase(classContents)) {
            String contentArrs[] = classContents.split("\\|\\|");
            map.put("classid", contentArrs[0]);
            map.put("school", contentArrs[1]);
            map.put("department", contentArrs[2]);
            map.put("classname", contentArrs[3]);
        }

        map.put("link", ip.getTemplateContent().getLink());
        map.put("uuid", ip.getUuid());
        map.put("rt", ip.getTemplateContent().getRt());

        return map;
    }

    public Map<String, Object> getDatasForVMReplace(InviteParams ip) {
        Map<String, Object> paramMap = new HashMap<String, Object>();
        User host = WUserAdapter.getInstance().get(ip.getInviterId());
        try {
            return this.getDatasForVMReplace(ip, host, 0, ip.getTemplateContent().getBody(), ip.getInviteeMail(), 0, ip.getAppToken(), "", "", ip.getTemplateContent().getSourceId());
        } catch (UnsupportedEncodingException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return paramMap;
    }

    /**
     * 是否真正传入了appId
     *
     * @param appId
     * @return
     */
    public boolean isRealAppId(String appId) {
        return (appId != null && !"".equals(appId) && !"0".equals(appId));
	}
}