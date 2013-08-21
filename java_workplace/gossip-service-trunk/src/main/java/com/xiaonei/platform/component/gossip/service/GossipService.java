package com.xiaonei.platform.component.gossip.service;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import xce.recentcontact.ContactInfo;
import xce.recentcontact.ContactType;

import com.xiaonei.platform.component.gossip.biz.GossipAuditBiz;
import com.xiaonei.platform.component.gossip.biz.GossipBiz;
import com.xiaonei.platform.component.gossip.biz.GossipCheckBiz;
import com.xiaonei.platform.component.gossip.biz.GossipNotifyBiz;
import com.xiaonei.platform.component.gossip.biz.GossipRightBiz;
import com.xiaonei.platform.component.gossip.biz.GossipScoreBiz;
import com.xiaonei.platform.component.gossip.biz.GossipUserWhiteListBiz;
import com.xiaonei.platform.component.gossip.dao.GossipCountDAO;
import com.xiaonei.platform.component.gossip.dao.GossipDAO;
import com.xiaonei.platform.component.gossip.dao.GossipExtensionDAO;
import com.xiaonei.platform.component.gossip.exception.NoPermissionException;
import com.xiaonei.platform.component.gossip.exception.PostGossipException;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.model.GossipCheckResult;
import com.xiaonei.platform.component.gossip.model.GossipCount;
import com.xiaonei.platform.component.gossip.model.GossipRight;
import com.xiaonei.platform.component.gossip.util.GossipUtils;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.component.gossip.view.RecentContactView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.platform.framework.exception.AntispamException;
import com.xiaonei.platform.framework.exception.FormValidationException;
import com.xiaonei.xce.notify.NotifyAdapter;
import com.xiaonei.xce.recentcontact.RecentContactAdapter;

/**
 * 
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 * 
 */
@Service
public class GossipService {

    @Autowired
    GossipCountDAO gossipCountDao;

    @Autowired
    GossipExtensionDAO gossipExtensionDao;

    @Autowired
    GossipDAO gossipDao;

    @Autowired
    GossipBiz gossipBiz;

    @Autowired
    GossipRightBiz gossipRightBiz;

    @Autowired
    GossipCheckBiz gossipCheckBiz;

    @Autowired
    GossipScoreBiz gossipScoreBiz;

    @Autowired
    GossipNotifyBiz gossipNotifyBiz;

    @Autowired
    GossipAuditBiz gossipAuditBiz;

    @Autowired
    GossipUserWhiteListBiz gossipUserWhiteListBiz;

    private Log logger = LogFactory.getLog(this.getClass());
    
    /**
     * 得到留言总数(除去悄悄话)<br>
     * 可以得到近期留言数或历史留言数（除去悄悄话）
     * 
     * @param ownerId 留言所有者的用户Id
     */
     public int getPublicGossipCount(int ownerId) {
    	 try {
    		 int publicGossipCount = gossipDao.getPublicCount(ownerId);
    		 return publicGossipCount;
    	 } catch (Exception e) {
    		 e.printStackTrace();
    	 }
    	 return 0;
     }
    /**
     * 得到留言总数<br>
     * 可以得到近期留言数或历史留言数
     * 
     * @param ownerId 留言所有者的用户Id
     */
    public int getGossipCount(int ownerId) {
        try {
            GossipCount gossipCount = gossipCountDao.getGossipCount(ownerId);
            if (gossipCount == null) {
                return 0;
            }
            return gossipCount.getLeavewordCount();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }
    
    public int getGossipCountNew(int ownerId) {
    	 try {
             int gossipCount = gossipDao.getCount(ownerId);
             return gossipCount;
         } catch (Exception e) {
             e.printStackTrace();
         }
    	 return 0;
    }

    public int getGossipCount(int ownerId, int host) {
        return getGossipCount(ownerId);
        // int total = 0;
        // try {
        // if(host==ownerId || User.isPageId(ownerId)){
        // Integer i = gossipCountDao.getGossipCount(ownerId);
        // total = (i==null)?0:i;
        // }
        // else{
        // total = gossipDao.getCount(ownerId);
        // }
        // } catch (Exception e) {
        // e.printStackTrace();
        // }
        // return total;
    }

    /**
     * 获取ownerid的留言数 当查看自己留言板时,返回包括聊天消息的全部留言数,查看他人留言板, 返回不包括聊天消息的留言数
     * 
     * @param ownerId 被访问的用户
     * @param host 当前登录用户
     * @return
     */
    public int getAllGossipCount(int ownerId, int host) {
        try {
            GossipCount gossipCount = gossipCountDao.getGossipCount(ownerId);
            if (gossipCount == null) {
                return 0;
            }
            if (host == ownerId) {
                return gossipCount.getChatCount() + gossipCount.getLeavewordCount();
            } else {
                return gossipCount.getLeavewordCount();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public int initGossipCount(int host) {
        try {
            return gossipCountDao.save(host);
        } catch (Exception e) {
            return 0;
        }
    }

    public GossipView getGossip(int host, long gossipId) {
        try {
            return gossipDao.getGossipById(host, gossipId);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public GossipView getGossipByTime(int owner, int sender, Date time) {
        try {
            return gossipDao.getGossipByTime(owner, sender, time);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    
    /**
     * 按时间区间获取我给别人的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    public List<GossipView> getOwnerGossipByTimeInterval(int owner,Date starttime,Date endtime,int limit,int offset) {  
        try {
        	return gossipDao.getOwnerGossipByTimeInterval(owner, starttime, endtime,limit,offset);
        } catch (Exception e) {
        	e.printStackTrace();
        	return null;
        }
    }
    
    /**
     * 按时间区间获取别人给我的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    public List<GossipView> getOtherGossipByTimeInterval(int owner,Date starttime,Date endtime,int limit,int offset) {  
        try {
        	return gossipDao.getOtherGossipByTimeInterval(owner, starttime, endtime,limit,offset);
        } catch (Exception e) {
        	e.printStackTrace();
        	return null;
        }
    }
    
    
    
    /**
     * 删除一条留言<br>
     * 删除近期留言，删除留言记录，同时count数减一<br>
     * 
     * @param id 留言id
     * @param userId 留言所有者的用户Id
     * @param hostID 删除者
     * @throws SQLException
     */
    public int delete(long id, int userId, int hostID) {
        return gossipBiz.delete(id, userId, hostID);
    }

    public int getConversationCount(int host, int guest) {
        try {
            return gossipDao.getConversationCount(host, guest);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    /**
     * 取留言,不包括聊天消息
     * 
     * @param host
     * @param guest
     * @param page
     * @param count
     * @return
     * @throws NoPermissionException
     */
    public List<GossipView> getGossipList(User host, User guest, int page, int count)
            throws NoPermissionException {
        boolean seegossip = gossipRightBiz.hasRight(host.getId(), 162, 32);
        if (!seegossip && host.getId() != guest.getId()) {
            if (!gossipRightBiz.hasGossipRight(host, guest, GossipRight.Get)) {
                throw new NoPermissionException("由于对方隐私设置，你没有权限查看对方留言");
            }
        }

        try {
            // List<GossipView> list = gossipDao.getList(guest.getId() ,
            // page*count, count,host.getId());
            // 默认只取公开的留言(chatId==0)
            List<GossipView> list = gossipDao.getPublicList(guest.getId(), page * count, count,
                    host.getId());
            boolean seewhisper = gossipRightBiz.hasRight(host.getId(), 184, 2);
            gossipBiz.setExtForGossipList(host.getId(), guest.getId(), true, list, seewhisper);
            return list;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<GossipView>();
    }

    /**
     * 取所有留言,当查看自己留言板时,包括聊天消息(做为悄悄话)
     * 
     * @param host
     * @param guest
     * @param page
     * @param count
     * @return
     * @throws NoPermissionException
     */
    public List<GossipView> getAllGossipList(User host, User guest, int page, int count)
            throws NoPermissionException {
        boolean seegossip = gossipRightBiz.hasRight(host.getId(), 162, 32);
        if (!seegossip && host.getId() != guest.getId()) {
            if (!gossipRightBiz.hasGossipRight(host, guest, GossipRight.Get)) {
                throw new NoPermissionException("由于对方隐私设置，你没有权限查看对方留言");
            }
        }

        try {

            List<GossipView> list = gossipDao.getAllList(guest.getId(), page * count, count, host
                    .getId());
            boolean seewhisper = gossipRightBiz.hasRight(host.getId(), 184, 2);
            gossipBiz.setExtForGossipList(host.getId(), guest.getId(), true, list, seewhisper);
            return list;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<GossipView>();
    }

    /**
     * @param owner
     * @param page 从1开始计
     * @param count
     * @return
     */
    public List<GossipView> getGossipByOwner(int owner, int page, int count) {
        try {
            page = page - 1 < 0 ? 0 : page - 1;
            return gossipDao.getGossipByOwner(owner, page * count, count);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<GossipView>();
    }

    /**
     * 获取和某个用户的留言记录
     * 
     * @param host 当前用户
     * @param guestId 目标用户
     * @param limit 翻页个数（1,4）
     * @param hostBeginId 翻页时，当前页面的host的最大留言id
     * @param hostEndId 翻页时，当前页面的host的最小留言id
     * @param guestBeginId 翻页时，当前页面的guest的最大留言id
     * @param guestEndId 翻页时，当前页面的guest的最小留言id
     * @param boundary 翻页时，如果翻到末页，true ,limit = 1，如果翻到首页 true，limit = -1
     * @return
     */
    public List<GossipView> getConversation(int host, int guestId, int limit, long hostBeginId,
            long hostEndId, long guestBeginId, long guestEndId, boolean boundary) {

        int count = Math.abs(limit) * 20;
        List<GossipView> conversationHost = gossipBiz.getConversation(host, guestId,
                limit > 0 ? hostEndId : hostBeginId, count, boundary);
        List<GossipView> conversationGuest = gossipBiz.getConversation(guestId, host,
                limit > 0 ? guestEndId : guestBeginId, count, boundary);

        if (logger.isDebugEnabled()) {
            logger.info("conversationHost:");
            for (GossipView gv : conversationHost) {
                logger.info(String.format("chat_name:%s,guestName:%s,body:%s", gv.getChatName(), gv
                        .getGuestName(), gv.getBody()));
            }
            logger.info("conversationGuest:");
            for (GossipView gv : conversationGuest) {
                logger.info(String.format("chat_name:%s,guestName:%s,body:%s", gv.getChatName(), gv
                        .getGuestName(), gv.getBody()));
            }
        }

        List<GossipView> conversation = conversationHost;
        conversation.addAll(conversationGuest);
        Collections.sort(conversation, new ConversationComparotr());
        if (conversation.size() > 20) {
            if (boundary) {
                if (limit > 0) {
                    // 取最后20个
                    conversation = conversation.subList(conversation.size() - 20, conversation
                            .size());
                } else {
                    // 取最前20个
                    conversation = conversation.subList(0, 20);
                }
            } else {
                if (limit > 0) {
                    // 取最后limit*20个之后的中前20个
                    int offset = (conversation.size() - count) > 0 ? 20 : conversation.size() % 20;
                    int from = count - 20;
                    int end = count - 20 + offset;
                    if (from >= 0 && end >= 0) {
                        conversation = conversation.subList(from, end);
                    } else {
                        System.out.println("error from: " + from + " end:" + end + " limit:"
                                + limit + "count:" + count);
                    }
                } else {
                    // 取最后limit*20个之前的中后20个
                    int begin = conversation.size() - count, end = conversation.size() - count + 20;
                    if (begin < 0) {
                        begin = 0;
                        end = conversation.size() > 20 ? 20 : conversation.size();
                    }
                    conversation = conversation.subList(begin, end);
                }
            }

        }
        return conversation;
    }

    /**
     * 取host和guest的最近留言
     * 
     * @param host
     * @param guestId
     * @param count
     * @return
     */
    public List<GossipView> getConversation(int host, int guestId, int count) {

        List<GossipView> conversationHost = gossipBiz.getConversation(host, guestId, -1, count,
                true);

        List<GossipView> conversationGuest = gossipBiz.getConversation(guestId, host, -1, count,
                true);

        List<GossipView> conversation = conversationHost;

        conversation.addAll(conversationGuest);

        Collections.sort(conversation, new ConversationComparotr());

        return (conversation.size() > 100) ? conversation.subList(0, 100) : conversation;
    }

    public int sendGossip(int hostid, int guestid, String message) throws NoPermissionException,
            FormValidationException, AntispamException {
        if (hostid == guestid) {
            return 0;
        }
        User host = WUserAdapter.getInstance().get(hostid);
        User guest = WUserAdapter.getInstance().get(guestid);
        UserBasic ub = WUserBasicAdapter.getInstance().get(host.getId());
        assert host != null;
        assert guest != null;
        assert ub != null;
        if (host != null && guest != null && ub != null) {
            Gossip gossip = new Gossip(guest.getId(), host.getId(), message);
            gossip.setName(host.getName());
            gossip.setUniv_name(ub.getUnivName());
            gossip.setTinyurl(ub.getTinyUrl());
            gossip.setChatId(hostid);
            if (!gossipRightBiz.hasGossipRight(host, guest, GossipRight.Post)) {
                throw new NoPermissionException("由于对方隐私设置，你没有权限留言");
            }
            gossipCheckBiz.checkBody(host, guest, gossip);
            message = gossip.getBody().getBody();
            gossip.getBody().setBody(message);
            // 存入对方留言板,不对gossipCount计数
            gossipBiz.save(gossip, false);
            //			gossipNotifyBiz.sendNotiy( host, guest, null,14,host.getId(),"");
            gossip.setOwner(hostid);
            gossip.setChatId(guestid);
            // 存入自己的留言板,不对gossipCount计数
            gossipBiz.save(gossip, false);
            gossipBiz.addRecentContact(hostid, guestid);
            gossipBiz.addRecentContact(guestid, hostid);
            return 1;
        }
        return 0;
    }

    public Gossip postGossip(User host, User guest, int cc, String body, String ip, String headUrl,
            String largeUrl, boolean page, boolean needAntiSpam, boolean whisper, String color)
            throws NoPermissionException, AntispamException, FormValidationException,
            PostGossipException {

        if (logger.isDebugEnabled()) {
            logger
                    .debug(String
                            .format(
                                    "host.getId():%d,guest.getId():%d,cc:%d,body:%s,ip:%s,headUrl:%s,largeUrl:%s,page:%s,needAntiSpam:%s,whisper:%s,color:%s",
                                    host.getId(), guest.getId(), cc, body, ip, headUrl, largeUrl,
                                    page, needAntiSpam, whisper, color));
        }

        UserBasic ub = WUserBasicAdapter.getInstance().get(host.getId());
        if (ub != null) {
            Gossip gossip = new Gossip(guest.getId(), host.getId(), body);
            gossip.setName(host.getName());
            gossip.setUniv_name(ub.getUnivName());
            gossip.setTinyurl(host.getTinyUrl());
            gossip.setHeadUrl(headUrl);
            gossip.setLarge_url(largeUrl);
            if (!page) {
                boolean hasGossipRight = gossipRightBiz.hasGossipRight(host, guest,
                        GossipRight.Post);
                if (logger.isDebugEnabled()) {
                    logger.debug(String.format("hasGossipRight:%s,params[host:%d,guest:%d]",
                            hasGossipRight, host.getId(), guest.getId()));
                }
                if (!hasGossipRight) {
                    throw new NoPermissionException("由于对方隐私设置，你没有权限留言");
                }
                // 非page的需要特殊判断，不能给page留言
                //                if (User.isPageId(guest.getId()) || User.isPageId(cc)) {
                //                    throw new NoPermissionException("请去page页面留言！");
                //                }
            }
            User ucc = null;
            if (cc > 0) {
                ucc = WUserAdapter.getInstance().get(cc);
            }
            GossipCheckResult result = null;
            if (needAntiSpam) {
                result = gossipCheckBiz.checkBody(host, guest, gossip);
            }
            body = gossip.getBody().getBody();
            if (StringUtils.isNotBlank(color)) {
                body = body + "<color:" + color + ":color/>";
            }
            String linkend = "";
            if (whisper) {
                body = body + "<xiaonei_only_to_me/><Toid/>" + (cc == 0 ? guest.getId() : cc);
                linkend = "&whisper=1";
            }
            if (logger.isDebugEnabled()) {
                logger.debug(String.format("whisper：%s,linkend:%s", whisper, linkend));
            }

            gossip.getBody().setBody(body);

            GossipUtils.formatGossipNoHerfWithNameAndId(host.getId(), gossip);

            if (page) {
                gossipBiz.save(gossip);
                return gossip;
            }
            long contentId = 0;
            long replyId = 0;
            int notifyType = 0;
            int source = 0;
            User owner = guest;
            if (ucc == null || guest.getId() == ucc.getId()) {
                gossipBiz.save(gossip);
                contentId = gossip.getId();
                if (host.getId() != guest.getId()) {
                    // 在别人的留言板直接留言
                    gossipBiz.addRecentContact(host.getId(), guest.getId());
                    gossipBiz.addRecentContact(guest.getId(), host.getId());
                    notifyType = 14;
                    source = host.getId();
                }
            } else {
                if (host.getId() == guest.getId()) {
                    // 在自己或者留言墙回复cc,如果是留言墙回复的，gossip对象要做特殊处理，owner要变成cc，在action中判断
                    gossipBiz.addRecentContact(host.getId(), ucc.getId());
                    gossipBiz.addRecentContact(ucc.getId(), host.getId());
                    gossip.setOwner(ucc.getId());
                    gossipBiz.save(gossip);
                    contentId = gossip.getId();
                    gossip.setOwner(host.getId());
                    gossipBiz.save(gossip);
                    replyId = gossip.getId();
                    notifyType = 14;
                    source = host.getId();
                    owner = ucc;
                } else {
                    // 在他人留言版回复cc
                    gossipBiz.addRecentContact(host.getId(), guest.getId());
                    gossipBiz.addRecentContact(guest.getId(), host.getId());
                    gossipBiz.save(gossip);
                    contentId = gossip.getId();
                    notifyType = 15;
                    source = host.getId();
                }
            }

            if (gossip != null && host != null && guest != null && !whisper) {
                if (logger.isDebugEnabled()) {
                    logger.info(String.format(
                            "准备发送@ cc:%d,host.getId():%d,guest.getId():%d,whisper:%s", cc, host
                                    .getId(), guest.getId(), whisper));
                }
                if (!whisper) {
                    //按照经纬的意思把source设置成留言的owner
                    GossipUtils.sendGossipAtNotify(gossip, host, guest, cc, gossip.getOwner(),
                            whisper);
                }
            }

            if (notifyType > 0 && source > 0) {
                /*
                 * gossipNotifyBiz.sendNotiy( host, owner,
                 * ucc,notifyType,source,linkend);
                 */
                // body.contains(xiaonei_wap) || body.contains(wap_gift_img)
                // if(gossip.getBody().getBody().contains(GossipView.xiaonei_wap)||gossip.getBody().getBody().contains(GossipView.wap_gift_img)){
                // //从wap过来的留言,发旧的留言提醒
                // gossipNotifyBiz.sendNotiy( host, owner,
                // ucc,notifyType,source,linkend);
                // }
                // else {
                //				 * @param host   当前用户wens
                //				 * @param guest  留言板所有者wens
                //				 * @param cc     被回复者wensong
                if (logger.isDebugEnabled()) {
                    logger.debug(String.format(
                            "ucc:%s,host.getId():%d,guest.getId():%d,ucc.getId():%d", ucc, host
                                    .getId(), guest.getId(), ucc.getId()));
                    logger
                            .debug("host.getId() != guest.getId():"
                                    + (host.getId() != guest.getId()));
                    logger.debug("guest.getId() != ucc.getId():" + (guest.getId() != ucc.getId()));
                    logger.debug("host.getId() != ucc.getId():" + (host.getId() != ucc.getId()));
                }
                if (ucc != null && host.getId() != guest.getId() && guest.getId() != ucc.getId()
                        && host.getId() != ucc.getId()) {
                    // a在b的留言板上回复c，发旧的留言提醒
                    gossipNotifyBiz.sendNotiy(host, owner, ucc, notifyType, source, linkend);
                } else {
                    // 开始webpager实时聊天
                    gossipNotifyBiz.sendWebpager(host, guest, ucc, gossip);
                }
                // }
            }
            if (gossip.getId() > 0) {
                gossipScoreBiz.addScore(host, guest, ucc);
            }
            if (result != null && result.audit) {
                gossipAuditBiz.saveAudit(replyId, contentId, host, guest, ucc, result.colorBody);
            }
            // try {
            // GossipModel model = new GossipModel();
            // model.setComment(gossip.getBody().getBody());
            // model.setFromID(host.getId());
            // model.setFromName(host.getName());
            // model.setGossipID(contentId);
            // model.setOwnerID(guest.getId());
            // model.setOwnerName(guest.getName());
            // GossipPublisher.publish(model);
            // } catch (Exception e) {
            // e.printStackTrace();
            // }
            return gossip;
        } else {
            return null;
        }

    }

    public int setRecentContactView(int host, List<RecentContactView> contactList) {

        int recentNewGossipGuestCount = 0;

        ContactInfo[] contacts = RecentContactAdapter.getInstance().get(host,
                ContactType.XNMESSAGE, 0, 10);

        List<Integer> userIds = new ArrayList<Integer>();
        for (ContactInfo contact : contacts) {
            contactList.add(new RecentContactView(contact));
            userIds.add(contact.guestId);
        }

        Map<Integer, WUserCache> usercache = WUserCacheAdapter.getInstance().getUserCacheMap(
                userIds);

        try {
            List<Integer> fromIds = NotifyAdapter.getInstance().getFromIdSeq(host,
                    new int[] { 472, 15 }, 0, 10);
            for (RecentContactView contact : contactList) {
                WUserCache user = usercache.get(contact.getGuestId());
                if (user != null) {
                    contact.setGuestName(user.getName());
                }
                if (fromIds.contains(contact.getGuestId())) {
                    contact.setResource(contact.getGuestId());
                } else {
                    contact.setResource(0);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return recentNewGossipGuestCount;
    }

    public class ConversationComparotr implements Comparator {

        public int compare(Object obj1, Object obj2) {
            GossipView gv1 = (GossipView) obj1;
            GossipView gv2 = (GossipView) obj2;
            if (gv1.getId() > gv2.getId()) return -1;
            else return 1;
        }
    }

    /**
     * 修复某个用户的留言数
     * 
     * @param owner
     * @return
     */
    public boolean fixGossipLevelCount(int owner) {
        try {
            final int levelCount = gossipBiz.getLevelCount(owner);
            gossipBiz.updateLevelCount(owner, levelCount);
            return true;
        } catch (Exception e) {
            try {
                final int levelCount = gossipBiz.getLevelCount(owner);
                gossipBiz.updateLevelCount(owner, levelCount);
                return true;
            } catch (Exception e1) {
                logger.error("可能DB出问题了，评论留言修复失败");
                return false;
            }

        }
    }

    /**
     * 保存白名单
     * 
     * @param owner
     * @return
     */
    public boolean saveGossipWhiteList(int owner) {
        try {
            gossipUserWhiteListBiz.save(owner);
            return true;
        } catch (Exception e) {
            try {
                gossipUserWhiteListBiz.save(owner);
                return true;
            } catch (Exception e1) {
                return false;
            }
        }
    }

    /**
     * 修改白名单的状态
     * 
     * @param user
     * @param enable
     * @return
     */
    public boolean updateGossipWhiteList(int user, int enable) {
        try {
            gossipUserWhiteListBiz.update(user, enable);
            return true;
        } catch (Exception e) {
            try {
                gossipUserWhiteListBiz.update(user, enable);
                return true;
            } catch (Exception e1) {
                return false;
            }
        }
    }

    /**
     * 让白名单失效
     * 
     * @param user
     * @return
     */
    public boolean disableGossipWhite(int user) {
        return updateGossipWhiteList(user, 0);
    }

    /**
     * 让白名单生效
     * 
     * @param user
     * @return
     */
    public boolean enableGossipWhite(int user) {
        return updateGossipWhiteList(user, 1);
    }

    /**
     * 获得白名单
     * 
     * @return
     */
    public List<Integer> findWhiteList() {
        return gossipUserWhiteListBiz.findUserWhiteList();
    }
}
