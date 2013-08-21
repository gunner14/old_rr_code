package com.xiaonei.platform.component.gossip.biz;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import xce.recentcontact.ContactType;

import com.xiaonei.antispam.AntiSpamUtils;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.email.service.control.ImmediateEmail;
import com.xiaonei.email.service.model.EmailConstants;
import com.xiaonei.platform.component.gossip.dao.GossipCountDAO;
import com.xiaonei.platform.component.gossip.dao.GossipDAO;
import com.xiaonei.platform.component.gossip.dao.GossipExtensionDAO;
import com.xiaonei.platform.component.gossip.dao.GossipIDDAO;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.model.GossipExtension;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.component.task.TaskManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.xce.recentcontact.RecentContactAdapter;
import com.xiaonei.xce.scorecache.DefaultScoreCache;
import com.xiaonei.xce.scorecache.ScoreCacheAdapter;
import com.xiaonei.xce.scorecache.ScoreCache.ScoreSignType;

/**
 * 
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 * 
 */
@Service
public class GossipBiz {

    private Log logger = LogFactory.getLog(this.getClass());

    @Autowired
    GossipDAO gossipDao;

    @Autowired
    GossipCountDAO gossipCountDao;

    @Autowired
    GossipExtensionDAO gossipExtensionDao;

    @Autowired
    GossipIDDAO gossipIdDao;

    @Autowired
    GossipAuditBiz gossipAuditBiz;

    /**
     * 增加留言<br>
     * 1、向gossip_recent_x表和gossip_body_recent_x 表插入记录，<br>
     * 2、更新leaveword_count表的leaveword_count字段<br>
     * 
     * @param gp Gossip类型的对象
     */
    public void save(Gossip gp) {
        save(gp, true);
    }

    public void save(Gossip gp, boolean incGossipCount) {
        try {
            // 生成一个留言记录Id
            long gid = gossipIdDao.getNext();
            gp.setId(gid);
            long ret = gossipDao.save(gp, gp.getOwner());
            gp.getBody().setId(gid);
            gossipDao.saveBody(gp.getOwner(), gid, gp.getBody().getBody(), gp.getBody().getTime());
            if (incGossipCount) {
                //采用一致的时间
                if (ret > 0) {
                    // 更新count
                    int state = gossipCountDao.increaseRecentCount(gp.getOwner());
                    if (state == 0) {
                        //System.err.println("GossipError(gossip_count_not_exist):id:"+gp.getOwner());
                        gossipCountDao.save(gp.getOwner());
                        //重新执行失败的增加操作
                        gossipCountDao.increaseRecentCount(gp.getOwner());
                    }
                } else {
                    //状态值非0正常
                }
                if (isValidPicUrl(gp.getHeadUrl()) && isValidPicUrl(gp.getLarge_url())) {
                    gossipExtensionDao.save(gid, gp.getOwner(), gp.getHeadUrl(), gp.getLarge_url());
                    gossipAuditBiz.saveAuditPic(gid, gp.getOwner(), gp.getHeadUrl());
                }
                sendEmail(gp.getSender(), gp.getOwner());
            } else {
                gossipCountDao.increaseChatCount(gp.getOwner());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static String getFilterOriginalBody(String body) {
        if (body == null) {
            return null;
        }
        String t = body;
        t = t.replaceAll("<xiaonei_only_to_me/>(.+)", "");
        t = t.replaceAll("<xiaonei_wap/>", "");
        t = t.replaceAll("<color:(\\w+)?:color/>", "");
        t = t.replaceAll("<xiaonei_(wap_)?gift(.+)/>", "");
        return t;
    }

    /**
     * @param host 用户id
     * @param guest 对方id
     * @param gossipId 第几页
     * @param count 多少个
     * @return
     */
    public List<GossipView> getConversation(int host, int guest, long gossipId, int count,
            boolean boundary) {
        try {
            List<GossipView> list = gossipDao.getConversation(host, guest, gossipId, count,
                    boundary);
            setExtForGossipList(host, host, false, list, false);
            return list;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<GossipView>();
    }

    public void setExtForGossipList(int host, int owner, boolean filterWhisper,
            List<GossipView> list, boolean admin) {
        if (list != null && list.size() > 0) {
            long heightGId = list.get(0).getId();
            long lowGId = list.get(list.size() - 1).getId();
            if (heightGId < lowGId) {
                long tmp = lowGId;
                lowGId = heightGId;
                heightGId = tmp;
            }
            List<Integer> idList = new ArrayList<Integer>();
            idList.add(owner);
            if (heightGId >= lowGId) {
                Map map = gossipExtensionDao.getGossipExtension(owner, lowGId, heightGId);
                if (map != null) {
                    for (GossipView gossip : list) {
                        Object extension = map.get(String.valueOf(gossip.getId()));
                        idList.add(gossip.getGuestId());
                        if (gossip.getChatId() != 0) {
                            idList.add(gossip.getChatId());
                        }
                        if (extension != null) {
                            GossipExtension ext = (GossipExtension) extension;
                            gossip.setHeadUrl(ext.getHead_url());
                            gossip.setLargeUrl(ext.getLarge_url());
                        }
                    }
                }
            }
            Map<Integer, WUserCache> idMap = null;
            try {
                idMap = WUserCacheAdapter.getInstance().getUserCacheMap(idList);
            } catch (Exception e) {
                e.printStackTrace();
            }

            for (Iterator<GossipView> itr = list.iterator(); itr.hasNext();) {
                GossipView gossip = itr.next();
                if (idMap != null && (idMap.get(gossip.getGuestId()) != null)) {
                    gossip.setKeepUse(idMap.get(gossip.getGuestId()).isKeepUse());
                    gossip.setVip(idMap.get(gossip.getGuestId()).isVipMember());
                }
                if (gossip.getChatId() > 0) {
                    if (idMap != null && (idMap.get(gossip.getChatId()) != null)) {
                        if (gossip.getGuestId() != gossip.getChatId()) {
                            //自己回复别人的
                            //gossip.setChatName(idMap.get(gossip.getChatId()).getName());
                            if (idMap.get(gossip.getChatId()) != null) {
                                gossip.setBody("回复" + idMap.get(gossip.getChatId()).getName()
                                        + ": " + gossip.getBody());
                            }
                        } else {
                            //别人回复自己的
                            //gossip.setChatName(idMap.get(owner).getName());
                            if (idMap.get(owner) != null) {
                                gossip.setBody("回复" + idMap.get(owner).getName() + ": "
                                        + gossip.getBody());
                            }
                        }
                    }
                }

                gossip.setSuperEmotion(isVip(idList, gossip.getGuestId()));

                if (!filterWhisper) {
                    continue;
                }
                if (admin) {
                    continue;
                }
                if (gossip.getOwner() == host || gossip.getGuestId() == host) {
                    continue;
                }
                if (gossip.isWhisper()) {
                    if (gossip.getChatId() > 0) {
                        // 此留言为悄悄话,不是自己看自己,过滤掉
                        if (host != owner) {
                            itr.remove();
                        }
                        continue;
                    } else {
                        // 获取Toid,老版悄悄话
                        try {
                            int toid = Integer.parseInt(gossip.getBody().substring(
                                    gossip.getBody().indexOf("<Toid/>") + "<Toid/>".length()));
                            if (toid != host) {
                                //非访问用户悄悄话
                                itr.remove();
                            }
                        } catch (NumberFormatException e) {
                            //暂不处理出现<Toid/>标签未闭合的少数情况
                            itr.remove();
                        }
                    }
                }
            }
        }
    }

    public boolean isValidPicUrl(String src) {
        if (StringUtils.startsWithIgnoreCase(src, "fmn")
                && (StringUtils.endsWithIgnoreCase(src, ".jpg") || StringUtils.endsWithIgnoreCase(
                        src, ".gif"))) {
            return true;
        }
        return false;
    }

    public void addRecentContact(int host, int guest) {
        if (host == guest) {
            return;
        }
        try {
            RecentContactAdapter.getInstance().add(host, guest, ContactType.XNMESSAGE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 删除一条留言<br>
     * 删除近期留言，删除留言记录，同时count数减一<br>
     * 删除历史留言，删除留言记录，没有更新count数。<br>
     * 
     * @param id 留言id
     * @param userId 留言所有者的用户Id
     * @throws SQLException
     */
    public int delete(long id, int userId, int hostID) {
        try {
            GossipView gv = gossipDao.getGossipById(userId, id);
            if (gv != null) {
                int ret = gossipDao.delete(id, userId);
                gossipDao.deleteBody(id, userId);
                if (gv.getChatId() == 0) {
                    gossipCountDao.decreaseRecentCount(userId);
                } else {
                    gossipCountDao.decreaseChatCount(userId);
                }
                AntiSpamUtils.getInstance().logDeleteComment(userId, hostID, CheckType.GOSSIP_TYPE,
                        id, " ");
                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public int getLevelCount(int ownerId) {
        return gossipDao.getGossipLevelCountByUser(ownerId);
    }

    public int updateLevelCount(int owner, int count) {
        return gossipCountDao.updateLevelCount(owner, count);
    }

    public void sendEmail(final int hostId, final int accepterId) {
        if (hostId == accepterId) {
            return;
        }

        final String domain;
        if (OpiConstants.getCurrentDomain() == null) {
            domain = "xiaonei.com";
        } else {
            domain = OpiConstants.getCurrentDomain();
        }
        Runnable task = new Runnable() {

            @Override
            public void run() {
                try {
                    User host = WUserAdapter.getInstance().get(hostId);
                    User accepter = WUserAdapter.getInstance().get(accepterId);
                    String senderName = host.getName();
                    String reciverName = accepter.getName();
                    String siteName = "人人网";
                    if ("kaixin.com".equals(domain)) {
                        siteName = "开心网";
                    }
                    String title = senderName + "在" + siteName + "给您留言了";
                    String body = "<head>	"
                            + "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"
                            + "</head>" + "<div style=\"line-height:1.5;\">"
                            + reciverName
                            + "你好： <br />"
                            + senderName
                            + "在"
                            + siteName
                            + "给您留言了，请登录你的"
                            + siteName
                            + "帐号，并用此链接查看：<br /><a href=\"http://gossip."
                            + domain
                            + "/\">http://gossip."
                            + domain
                            + "/</a><br />感谢你对"
                            + siteName
                            + "的支持！ <br />"
                            + siteName
                            + " <a href=\"http://www."
                            + domain
                            + "\">http://www."
                            + domain
                            + "/</a><br />"
                            + "<br />---<br />若你不想收到此类邮件通知，可登录"
                            + siteName
                            + "进行设置：<a href=\"http://safe."
                            + domain
                            + "/setEmail.do\">http://safe."
                            + domain
                            + "/setEmail.do</a>"
                            + "</div>";

                    boolean succ = ImmediateEmail.send(host, accepter, body, title,
                            EmailConstants.WHITE_COLLAR_REPLY_LEAVEWORD, domain);
                    //System.out.println(" send email : "+true);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        TaskManager.getInstance().addTask(0, task);
    }

    public boolean isVip(List<Integer> userIdList, int userId) {
        Map<Integer, DefaultScoreCache> scoreCache = null;
        boolean isVip = false;
        try {
            scoreCache = ScoreCacheAdapter.getInstance().getScoreDataMap(userIdList);
            if (scoreCache != null && (scoreCache.get(userId) != null)) {
                isVip = scoreCache.get(userId).isSignTrue(ScoreSignType.TYPE_SUPER_EMOTION);
                return isVip;
            } else {
                return true;
            }
        } catch (Exception e) {
            logger.warn("获取积分缓存信息异常:" + e.getMessage());
            isVip = true;
            return isVip;
        }

    }
}
