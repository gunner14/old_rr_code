package com.xiaonei.reg.guide.logic;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.xiaonei.notify.logic.NotifyLogic;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.webpager.event.WebpagerEventNotify;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.model.UserPersonalInfoModel;
import com.xiaonei.reg.guide.view.RegNotifyView;
import com.xiaonei.reg.guide.view.UserTinyView;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.dao.UserPersonalInfoDAO;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.logic.additional.RegMsgLogic;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 注册通知相关
 * 
 * @author wang-tai
 */
public class RegNotifyLogic {

    public static final String NOTIFY_ADD_FRIENDS = "notify_add_friends";

    public static final String NOTIFY_BY_ADMIN = "notify_by_admin";

    public static final String NOTIFY_BY_ADMIN_WITH_GOSSIP = "notify_by_admin_with_gossip";

    public static final String NOTIFY_BY_ADMIN_WITH_GOSSIP_HS = "notify_by_admin_with_gossip_hs";

    public static final String NOTIFY_MSN_BUDDY = "notify_msn_buddy";

    public static final String NOTIFY_SAME_IP_PEOPLE = "notify_same_ip_people";

    private static RegNotifyLogic _instance = new RegNotifyLogic();

    private Map<String, Object> notifyLocalMemCache = null;

    public static RegNotifyLogic getInstance() {
        return _instance;
    }

    private RegNotifyLogic() {
        super();
        if (notifyLocalMemCache == null) {
            notifyLocalMemCache = new ConcurrentHashMap<String, Object>();
        }
    }

    public void sendNotify(User host, boolean isSendAdminNotify) {
        RegInviteMap inviter = RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());
        /* 给邀请人，和邀请人的好友发notify */
        RegNotifyView notify = new RegNotifyView();
        notify.setId(host.getId());
        notify.setName(host.getName());
        if (inviter != null) {
            notify.setInviterId(inviter.getInviterId());
            if (inviter.getInviterId() > 0) {
                /* 发送notify */
                RegNotifyLogic.getInstance().sendNotify(notify, RegNotifyLogic.NOTIFY_ADD_FRIENDS);
            }
        }
        /* 管理员给我发邀请 */
        if (UserAdapter.get(host).getStage() == Stage.STAGE_HEIGHSCHOOL) {
            RegNotifyLogic.getInstance().sendNotify(notify,RegNotifyLogic.NOTIFY_BY_ADMIN_WITH_GOSSIP_HS);
        } else if (UserAdapter.get(host).getStage() == Stage.STAGE_UNIV) {
            RegNotifyLogic.getInstance().sendNotify(notify,RegNotifyLogic.NOTIFY_BY_ADMIN_WITH_GOSSIP);
        } else {
            if (isSendAdminNotify) {
                RegNotifyLogic.getInstance().sendNotify(notify, RegNotifyLogic.NOTIFY_BY_ADMIN);
            }
        }
        /* 给msn联系人列表里有我的人发送notify */
        RegNotifyLogic.getInstance().sendNotify(notify, RegNotifyLogic.NOTIFY_MSN_BUDDY);
    }

    /**
     * 发送notify
     * 
     * @param typeNodity
     * @param host
     * @author wang-tai
     */
    public void sendNotify(final RegNotifyView notify, final String typeNotify) {
        if (null == notify || null == typeNotify) return;
        User host = UserAdapter.getUser(notify.getId());
        if (null == host) return;
        /* 给邀请人发送notify */
        if (NOTIFY_ADD_FRIENDS.equals(typeNotify)) {
            if (0 == notify.getInviterId()) return;
            User inviter = UserAdapter.getUser(notify.getInviterId());
            if (inviter == null) {
                return;
            }
            // 给邀请人的活跃好友发通知
            // String[] friendsIds = getFriendsId(inviter);
            // sendAddFri(friendsIds, getContent(inviter, host));
            // 给邀请人发送通知
            send2InviterNotify(host, inviter.getId());
            // 给被邀请人发送通知
            send2InviteeNotify(inviter, host, false);
        }
        /* 以管理员身份发送notify */
        else if (NOTIFY_BY_ADMIN.equals(typeNotify)) {
            // 站内信
            try {
                //RegMsgLogic.getInstance().sendWelcomeMessage(host);
                RegMsgLogic.getInstance().sendWelcomeMessageNew(host);
            } catch (Exception e) {
                System.err.println("Message sent failure");
                e.printStackTrace();
            }
        }
        /* 以管理员身份给高中发送notify */
        else if (NOTIFY_BY_ADMIN_WITH_GOSSIP_HS.equals(typeNotify)) {
            // 站内信
            try {
                RegMsgLogic.getInstance().sendWelcomeMessageHs(host);
            } catch (SQLException e) {
                System.err.println("Message sent failure");
                e.printStackTrace(System.err);
            }
            // 留言
            // gossipToHs(host);
        }
        /* 以管理员身份给大学发送notify */
        else if (NOTIFY_BY_ADMIN_WITH_GOSSIP.equals(typeNotify)) {
            // 站内信
            try {
                //RegMsgLogic.getInstance().sendWelcomeMessage(host);
                RegMsgLogic.getInstance().sendWelcomeMessageNew(host);
            } catch (Exception e) {
                System.err.println("Message sent failure");
                e.printStackTrace(System.err);
            }
            // 应 产品－许欣 的要求在20081007去掉了它
            // // 留言
            // this.gossipTo(host);
        }
        /* 给到如果msn列表的那些人发送相关的notify */
        else if (NOTIFY_MSN_BUDDY.equals(typeNotify)) {
            try {
                send2MSNBuddyInvitee(host);
                send2MSNBuddyInviters(host);
            } catch (Exception e) {

            }
        } else if (NOTIFY_SAME_IP_PEOPLE.equals(typeNotify)) {
            try {
                send2SameIP(notify);
            } catch (Exception e) {

            }
        }
    }

    /**
     * 给使用相同ip的人发notify [xx 可能是你认识的人，是否要加为好友？或者 去看看那Ta的个人主页>>]
     * 
     * @param host
     */
    private void send2SameIP(final RegNotifyView notify) {

        List<UserPersonalInfoModel> ls = null;
        try {
            ls = UserPersonalInfoDAO.getInstance().getUserList(notify.getIp_long());
        } catch (SQLException e) {
            e.printStackTrace();
            return;
        }
        User host = UserAdapter.getUser(notify.getId());
        if (null == host) return;

        String url = Globals.urlMain + "/profile.do?id=" + host.getId();
        String aid = "addFriend" + host.getId();
        String js = "showRequestFriendDialog('" + aid + "','" + host.getId() + "','"
                + host.getName() + "','" + UserAdapter.get(host).getHead_Url() + "')";
        String content = "";
        content += "<a href=\"" + url + "\" target=\"_blank\">" + host.getName() + "</a> 可能是你认识的人，";
        content += "<span class=\"notify-link\"> ";
        content += "<a  href=\"javascript:void(0)\"class=\"head-pic\" style=\"background-image:url("
                + UserAdapter.get(host).getHead_Url() + ");\"></a> ";
        content += "是否要<a id=\"" + aid + "\" href=\"javascript:void(0)\" onclick=\"javascript:"
                + js + "\">加为好友</a>？";
        content += "<span class=\"pipe\">|</span> <a href=\"" + url
                + "\" target=\"_blank\">去看看Ta的个人主页</a>&gt;&gt;";
        content += "</span>";
        int begin = RegUtil.randomNum(ls.size());
        int end = ls.size() - begin > 20 ? 20 : ls.size() - begin;
        for (int i = begin; i < end; i++) {
            saveOnlyOneNotify(host.getId(), ls.get(begin + i).getId(), 0, content);
        }
    }

    /**
     * 给含有我的msn 好友列表主人发送notify。 [xx 刚刚注册人人网了，Ta可能是你认识的人，去看看吧>> | 加为好友]
     * 
     * @param invitee
     */
    private void send2MSNBuddyInviters(final User invitee) {
        String url = Globals.urlMain + "/profile.do?id=" + invitee.getId();
        String aid = "addFriend" + invitee.getId();
        String js = "showRequestFriendDialog('" + aid + "','" + invitee.getId() + "','"
                + invitee.getName() + "','" + UserAdapter.get(invitee).getHead_Url() + "')";
        String content = "";
        content += "<a href=\"" + url + "\" target=\"_blank\">" + invitee.getName()
                + "</a> 刚刚注册人人网了，Ta可能是你认识的人，";
        content += "<span class=\"notify-link\"> ";
        content += "<a  href=\"" + url + "\" class=\"head-pic\" style=\"background-image:url("
                + UserAdapter.get(invitee).getHead_Url() + ");\"></a> ";
        content += "<a href=\"" + url
                + "\" target=\"_blank\">去看看吧</a>&gt;&gt; <span class=\"pipe\">|</span> ";
        content += "<a id=\"" + aid + "\" href=\"javascript:void(0)\" onclick=\"javascript:" + js
                + "\">加为好友</a>";
        content += "</span>";

        List<InviteInfo> ls = getWhoseTheMSNBuddyListIHava(invitee);
        if (null == ls) return;

        for (InviteInfo user : ls) {
            saveOnlyOneNotify(invitee.getId(), user.getOwner_id(), 0, content);
        }
    }

    /**
     * 给我发送含有我的msn 好友列表主人信息的notify。 [xx 也注册了人人网，你可能认识Ta吧，去看看>> 或者 加为好友]
     * 
     * @param invitee
     */
    private void send2MSNBuddyInvitee(final User invitee) {
        List<InviteInfo> ls = getWhoseTheMSNBuddyListIHava(invitee);
        if (null == ls) return;
        for (InviteInfo invite : ls) {
            User user = UserAdapter.getUser(invite.getOwner_id());
            if (user == null) return;
            String content = "";
            String url = Globals.urlMain + "/profile.do?id=" + user.getId();
            String aid = "addFriend" + user.getId();
            String js = "showRequestFriendDialog('" + aid + "','" + user.getId() + "','"
                    + user.getName() + "','" + UserAdapter.get(invitee).getHead_Url() + "')";
            content += "<a href=\"" + url + "\" target=\"_blank\">" + user.getName()
                    + "</a> 也注册了人人网，你可能认识Ta吧，";
            content += "<span class=\"notify-link\"> ";
            content += "<a  href=\"javascript:void(0)\"class=\"head-pic\" style=\"background-image:url("
                    + UserAdapter.get(invitee).getHead_Url() + ");\"></a> ";
            content += "<a href=\"" + url
                    + "\" target=\"_blank\">去看看</a>&gt;&gt; <span class=\"pipe\">|</span> ";
            content += "<a id=" + aid + " href=\"javascript:void(0)\" onclick=\"javascript:" + js
                    + "\">加为好友</a>";
            content += "</span>";

            saveOnlyOneNotify(user.getId(), invitee.getId(), 0, content);
        }
    }

    /**
     * 得到那些好友列表有我
     * 
     * @param host
     * @return
     */
    private List<InviteInfo> getWhoseTheMSNBuddyListIHava(final User host) {
        List<InviteInfo> iils = null;
        try {
            iils = InviteDAO.getInstance().invitedGetInvitoer(host.getAccount());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }

        return iils;
    }

    /**
     * 注册成功后，给被邀请人发送notify。
     * 
     * 邀请人A：XXXB接受你的邀请注册了人人网，并与你成为好友。（链接到B的个人主页）
     * 邀请人A：XXXB接受你的邀请注册了人人网，并与你成为好友。（链接到B的个人主页）
     * 
     * @param inviter 发送人
     * @param invitee 接收人
     * @param isRegUserSelf 是不是接受人自己
     */
    public void send2InviteeNotify(final User inviter, final User invitee, boolean isRegUserSelf) {
        String content = "";
        if (isRegUserSelf) {
            content += "你已经和<a href=\"" + Globals.urlMain + "/profile.do?id=" + inviter.getId()
                    + "\" target=\"_blank\">" + inviter.getName() + "</a>成为了好友，<a href=\""
                    + Globals.urlMain + "/profile.do?id=" + inviter.getId()
                    + "\" target=\"_blank\">查看Ta的个人主页</a>。";
        } else {
            content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + inviter.getId()
                    + "\" target=\"_blank\">" + inviter.getName() + "</a>接受你的邀请注册了人人网，并与你成为好友。";
        }
        saveOnlyOneNotify(inviter.getId(), invitee.getId(), 0, content);
    }

    /**
     * 给邀请过我的人发送notify。 <br/>
     * 你曾邀请过的XXXB刚刚注册了人人网，查看Ta的个人主页。（链接到B的个人主页）
     * 
     * @param invitee
     */
    public void sendToInviterNotify(final User invitee, final User inviter) {
        String url = Globals.urlMain + "/profile.do?id=" + invitee.getId();
        String content = "";
        content += "你曾邀请过的<a href=\"" + url + "\" target=\"_blank\">" + invitee.getName()
                + "</a>刚刚注册了人人网，<a href=\"" + url + "\" target=\"_blank\">查看Ta的个人主页</a>。";
        saveOnlyOneNotify(invitee.getId(), inviter.getId(), 0, content);
    }

    /**
     * 给邀请人发通知
     * 
     * @param host
     * @param inviterId
     */
    private void send2InviterNotify(final User host, final int inviterId) {
        String content = "";
        content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId()
                + "\" target='_blank'>" + host.getName() + "</a>";
        content += " 接受你的邀请，成功注册并已成为你的好友。";
        content += "<span class=\"notify-link\"> ";
        content += "<a  href=\"javascript:void(0)\"class=\"head-pic\" style=\"background-image:url("
                + UserAdapter.get(host).getHead_Url() + ");\"></a> ";
        content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId()
                + "\" target='_blank'>去看看吧~</a>";
        content += "</span>";
        saveOnlyOneNotify(host.getId(), inviterId, 0, content);
    }

    /**
     * id註冊發送notify
     * 
     * @param host
     */
    public void sendXidRegNofify(final User host) {
        //20100331 功能下线
        if (true) return;
        /*String content = "";
        content = "您已经使用" + host.getAccount()
                + "在人人网注册成功，绑定手机、绑定邮箱或者设置密保问题增加帐号安全性，在丢失密码的情况下可以轻松 找回帐号。" + "<a href=\""
                + Globals.urlSafeCenter + "/guide.do?type=clear\">点击立即进入绑定流程</a>";
        NotifyLogic.saveNotify(host.getId(), 0, content);*/
    }

    /**
     * 给邀请人发web pager通知
     * 
     * @param host
     * @param inviterId
     */
    @SuppressWarnings("unused")
    private void send2InviterWebpagerEventNotify(final User host, final int inviterId) {
        String title = "你邀请的好友加入了人人网";
        String content = "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId()
                + "\" target='_blank'\">" + host.getName() + "</a>接受你的邀请加入人人网了";
        WebpagerEventNotify.notify(inviterId, WebpagerEventNotify.TYPE_FRIEND, title, content);
    }

    /**
     * 群发送添加好友notify
     * 
     * @param friendsIds
     * @param content
     */
    @SuppressWarnings("unused")
    private void sendAddFri(final String[] friendsIds, final String content) {
        if (null == friendsIds) {
            System.err.println("friendsIds is null");
            return;
        }
        for (String element : friendsIds) {
            try {
                saveOnlyOneNotify(-1, (new Integer(element)).intValue(), 0, content);
            } catch (Exception e) {
                continue;
            }
        }
    }

    /**
     * 获得通知文字
     * 
     * @param host
     * @return
     * @template <a
     *           href=\"javascript:void(0)\">许欣</a> 刚刚接受 <a href="">xx</a>
     *           的邀请加入人人网，你可能认识 TA 吧？ <span class=\"notify-link\"> <a
     *           href=\"javascript:void(0)\" class=\"head-pic\"
     *           style=\"background-image: url(http://hd27./photos/hd27
     *           /20080613/01/47/tiny_7798o150.jpg);\"></a> <a
     *           href=\"javascript:void(0)\">去看看</a> <span
     *           class=\"pipe\">|</span> <a
     *           href=\"javascript:void(0)\">加为好友</a> </span> <span
     *           class=\"date\">2:17pm</span>
     */
    @SuppressWarnings("unused")
    private String getContent(final User inviter, final User host) {
        String content = "";
        String tinyurl = SnsAdapterFactory.getUserBasicAdapter().get(host.getId()).getTiny_Url();
        content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId() + "\">"
                + host.getName() + "</a> 刚刚接受 " + "<a href=\"" + Globals.urlMain
                + "/profile.do?id=" + inviter.getId() + "\">" + inviter.getName()
                + "</a> 的邀请加入人人网，你可能认识 TA 吧？";
        content += "<span class=\"notify-link\">";
        content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId()
                + "\" class=\"head-pic\" style=\"background-image: url(" + tinyurl + ");\"></a>";
        content += "<a href=\"" + Globals.urlMain + "/profile.do?id=" + host.getId() + "\">去看看</a>";
        content += "<span class=\"pipe\">|</span>";
        content += "<a id=\"addFriend" + host.getId()
                + "\" onclick=\"showRequestFriendDialog('addFriend" + host.getId() + "','"
                + host.getId() + "','" + host.getName() + "','"
                + UserAdapter.get(host).getHead_Url() + "')\" href=\"javascript:void(0)\">加为好友</a>";
        content += "</span>";
        return content;
    }

    /**
     * 得到活跃好友
     * 
     * @param host
     * @return
     */
    @SuppressWarnings("unused")
    private String[] getFriendsId(final User inviter) {
        if (inviter != null) {
            List<UserTinyView> friendsList = getOnlineFriendFromMem(inviter.getId(), FriendsHome
                    .getInstance().getFriendCount(inviter));
            if (null == friendsList || friendsList.size() == 0) return null;
            String[] friendsIds = new String[friendsList.size()];
            for (int i = 0; i < friendsIds.length; i++) {
                int id = friendsList.get(i).getId();
                friendsIds[i] = String.valueOf(id);
            }
            return friendsIds;
        }
        return null;

    }

    /**
     * 得到最近登录好友 自定数目
     * 
     * @param guestId
     * @param friendCounts
     * @param int numOfGetfriend
     * @return
     */
    private static List<UserTinyView> getOnlineFriendFromMem(final int guestId,
            final int friendCounts) {
        // 2009-12-11 15:56:54 Inshion 影响UserBasic拆分 注掉
        /*
        if (friendCounts <= 2000) {
        	try {
        		return new UserTinyViewDAO()
        				.getOnLineFriendList(guestId, 0, 24);
        	} catch (SQLException e) {
        		e.printStackTrace();
        	}
        }
        */
        return null;
    }

    /**
     * 用管理员身份给某人留言
     * 
     * @param host
     */
    @SuppressWarnings("unused")
    private void gossipTo(final User host) {
        if (null == host) return;
        String body = "";
        body += "Hello，欢迎加入人人网。\n";
        body += "在人人网你可以找到老同学，老朋友。\n";
        body += "也可以创建个性空间，发表日志，上传照片，分享好友新鲜事。\n";
        body += "还可以和朋友一起玩各种好玩的互动游戏。\n";
        body += "如需帮助，请到页面底部，点击客服，给我们留言。\n";
        //		User admin = UserAdapter.getUser(223158817);
        int cc = 0;
        //GossipHome.getInstance().postGossip(body, admin, host, cc);
    }

    /**
     * 用管理员身份给某人留言-高中
     * 
     * @param host
     */
    @SuppressWarnings("unused")
    private void gossipToHs(final User host) {
        if (null == host) return;
        String body = "";
        body += "Hello，欢迎加入人人网。\n";
        body += "在人人网你可以找到老同学，老朋友。\n";
        body += "也可以创建个性空间，发表日志，上传照片，分享好友新鲜事。\n";
        body += "还可以和朋友一起玩各种好玩的互动游戏。\n";
        body += "如需帮助，请到页面底部，点击客服，给我们留言。\n";
        //		User admin = UserAdapter.getUser(230229770);
        int cc = 0;
        //GossipHome.getInstance().postGossip(body, admin, host, cc);
    }

    /**
     * 一个人只给另一个人发一次notify 如果 sender = -1 不限制次数
     * 
     * @param sender
     * @param reciver
     * @param appId
     * @param content
     */

    @SuppressWarnings("unchecked")
    private void saveOnlyOneNotify(final int sender, final int reciver, final int appId,
            final String content) {
        if (-1 == sender) {
            NotifyLogic.saveNotify(reciver, appId, content);
        } else {
            String hskey = sender + "+" + reciver;
            // 先从mem cache中取
            // MemCacheManager mem =
            // MemCacheFactory.getInstance().getManagerByPoolName
            // (MemCacheKeys.pool_user_space);
            String key = MEN_CACHE_ONLY_ONE_NOTIFY;
            Map<String, String> hm = (Map<String, String>) notifyLocalMemCache.get(key);
            if (null == hm) {
                hm = new HashMap<String, String>();
                hm.put(hskey, hskey);
                notifyLocalMemCache.put(key, hm);
                // mem.set(key, hm, new Date(MemCacheKeys.minute * 60 * 24));

                NotifyLogic.saveNotify(reciver, appId, content);
            } else {
                Object obj = hm.get(hskey);
                if (null == obj) {
                    hm.put(hskey, hskey);
                    notifyLocalMemCache.put(key, hm);
                    // mem.set(key, hm, new Date(MemCacheKeys.minute * 60 *
                    // 24));

                    NotifyLogic.saveNotify(reciver, appId, content);
                }
            }
        }
    }

    private static final String MEN_CACHE_ONLY_ONE_NOTIFY = "men_cache_only_one_notify";

}
