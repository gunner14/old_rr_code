package com.dodoyo.invite.core.logic;

import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.dao.InviteDAO;
import com.dodoyo.invite.model.InviteInfo;
import com.dodoyo.invite.model.Owners;
import com.xiaonei.album.model.Album;
import com.xiaonei.platform.component.application.home.AppOutSiteInvitationHome;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

import javax.servlet.http.HttpServletRequest;
import java.io.UnsupportedEncodingException;
import java.sql.SQLException;
import java.util.*;

public class InviteParamsUtils {
    private InviteParamsUtils() {
    }

    private static InviteParamsUtils ipu = new InviteParamsUtils();

    public static InviteParamsUtils getInstance() {
        return ipu;
    }

    public InviteParams getInviteParams(HttpServletRequest request) {
        InviteParams ip = new InviteParams();
        User host = BaseThreadUtil.currentHost(request);
        String app_html = request.getParameter("app_html");
        String action = request.getParameter("action");
        String url = request.getParameter("url");
        String invite = request.getParameter("action");
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
        LogUtils.logInfo("appId = " + ip.getAppId());
        if (ip.getAppId() != null && !"".equals(ip.getAppId()) && !"0".equals(ip.getAppId())) {
            try {
            	LogUtils.logInfo("Call AppOutSiteInvitationHome.insert by appId = " + ip.getAppId());
                String appToken = AppOutSiteInvitationHome.getInstance().insert(ip.getInviterId(), Integer.parseInt(ip.getAppId()));
                ip.setAppToken(appToken);
            }
            catch (Exception e) {
                ip.setAppToken("tokenerr");
                e.printStackTrace();
            }
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
        List<InviteInfo> senders = InviteDAO.getInstance().getInviteUsers(email);
        List<Owners> counters = new ArrayList<Owners>();
        map.put("senders", senders);
        for (int i = 0; i < senders.size(); i++) {
            InviteInfo ii = senders.get(i);
            Owners o = new Owners();
            o.setOwner_name(ii.getOwner_name());
            int friendsCount = FriendsHome.getInstance().getFriendCount(host, 0);
            o.setFriendsCount(friendsCount);
            int notesCount = /*GossipLogic.getCount(ii.getOwner_id(),"recent")*/0;
            o.setNotesCount(notesCount);
            int photosCount = 0;//AlbumDAO.getInstance().getPhotoCount(ii.getOwner_id());
            o.setPhotosCount(photosCount);
            counters.add(o);
        }
        map.put("counters", counters);
        map.put("hostname", host.getName());
        map.put("hostid", host.getId());
        map.put("headurl", /*host.getHead_Url()*/"");
        map.put("headurl_2", /*host.getHeadUrl()*/"");
        map.put("friendscount", /*host.getFriendCount()*/0);
        map.put("someTips", someTips);
        List<WUserCache> friendList = FriendsHome.getInstance().getFriends(host, 0, 4);
        List<WUserCache> friendList_2 = FriendsHome.getInstance().getFriends(host, 0, 4);
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
        Map paramMap = new HashMap();
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