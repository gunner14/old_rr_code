package com.xiaonei.platform.component.gossip.home;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.buddy.BuddyRelation;
import net.paoding.rose.jade.annotation.SQLParam;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.log4j.Logger;
import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Service;

import xce.recentcontact.ContactInfo;
import xce.recentcontact.ContactType;


import com.xiaonei.platform.component.gossip.biz.GossipCheckBiz;
import com.xiaonei.platform.component.gossip.biz.GossipRightBiz;
import com.xiaonei.platform.component.gossip.dao.GossipCountDAO;
import com.xiaonei.platform.component.gossip.dao.GossipExtensionDAO;
import com.xiaonei.platform.component.gossip.exception.NoPermissionException;
import com.xiaonei.platform.component.gossip.exception.PostGossipException;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.model.GossipRight;
import com.xiaonei.platform.component.gossip.service.GossipService;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.component.gossip.view.RecentContactView;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.platform.framework.exception.AntispamException;
import com.xiaonei.platform.framework.exception.FormValidationException;
import com.xiaonei.xce.notify.NotifyAdapter;
import com.xiaonei.xce.recentcontact.RecentContactAdapter;

/**
 * 留言业务组件对外的Home接口类，外界通过此Home接口类访问留言业务组件<br>
 * 请开发者将真正需要暴露给调用者的方法，转移到此Home接口类中，实现对外提供服务<br>
 * 原Logic逻辑类只负责留言业务组件内部调用，不再对外发布
 * 
 * @author enle.lv@opi-corp.com 2008-3-4 下午03:56:11
 */
@Service
public class GossipHome implements ApplicationContextAware {

    private static final Logger logger = Logger.getLogger(GossipHome.class);
    
    private static final Logger gossiplog = logger.getLogger("info");

    private static GossipHome instance = null;

    private static ApplicationContext applicationContext;

    public static GossipHome getInstance() {
        if (instance == null) {
            instance = (GossipHome) BeanFactoryUtils.beanOfType(applicationContext,
                    GossipHome.class);
        }
        return instance;
    }

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        GossipHome.applicationContext = applicationContext;
    }

    @Autowired
    GossipService gossipService;

    @Autowired
    GossipCheckBiz gossipCheckBiz;

    @Autowired
    GossipRightBiz gossipRightBiz;

    @Autowired
    GossipExtensionDAO gossipExtensionDao;

    public String checkBody(int host, int guest, String body) throws FormValidationException,
            AntispamException {
        return gossipCheckBiz.checkBody(host, guest, body);
    }

    /**
     * 发表留言的接口(包含权限验证，内容验证,最近联系人逻辑，发新鲜事和回复提示)
     * 
     * @param host 当前用户
     * @param guest 留言板所有者
     * @param cc 被回复者
     * @param body
     * @param ip
     * @param headUrl
     * @param largeUrl
     * @param page 是否page true：不发新鲜事和回复提示，不加最近联系人
     * @param needAntiSpam 是否进行antispam过滤
     * @return
     * @throws NoPermissionException
     * @throws AntispamException
     * @throws FormValidationException
     * @throws PostGossipException
     */
    public Gossip postGossip(User host, User guest, int cc, String body, String ip, String headUrl,
            String largeUrl, boolean page, boolean needAntiSpam) throws NoPermissionException,
            AntispamException, FormValidationException, PostGossipException {
        return postGossip(host, guest, cc, body, ip, headUrl, largeUrl, page, needAntiSpam, false);
    }

    public Gossip postGossip(User host, User guest, int cc, String body, String ip, String headUrl,
            String largeUrl, boolean page, boolean needAntiSpam, boolean whisper)
            throws NoPermissionException, AntispamException, FormValidationException,
            PostGossipException {
        return postGossip(host, guest, cc, body, ip, headUrl, largeUrl, page, needAntiSpam,
                whisper, null);
    }

    /**
     * 发表留言的接口(包含权限验证，内容验证,最近联系人逻辑，发新鲜事和回复提示)
     * 
     * @param host 当前用户wens
     * @param guest 留言板所有者wens
     * @param cc 被回复者wensong
     * @param body
     * @param ip
     * @param headUrl
     * @param largeUrl
     * @param page 是否page true：不发新鲜事和回复提示，不加最近联系人
     * @param needAntiSpam 是否进行antispam过滤
     * @param whisper 是否是悄悄话
     * @param color 颜色
     * @return
     * @throws NoPermissionException
     * @throws AntispamException
     * @throws FormValidationException
     * @throws PostGossipException
     */
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

        boolean isBlocked = false;
        try {
            if (host.getId() != guest.getId()) {
                BuddyRelation a = AdapterFactory.getBuddyCoreAdapter().getRelation(host.getId(),
                        guest.getId());
                isBlocked = a.isBlocked();
            }
            if (cc > 0 && !isBlocked) {
                BuddyRelation a = AdapterFactory.getBuddyCoreAdapter()
                        .getRelation(host.getId(), cc);
                isBlocked = a.isBlocked();
            }
        } catch (Exception e) {
            logger.warn("判断是否拉黑异常:" + e);
        }

        if (logger.isDebugEnabled()) {
            logger.debug("postGossip isBlocked:" + isBlocked);
        }

        if (isBlocked) {
            throw new NoPermissionException("由于对方隐私设置，你没有权限留言");
        }
        gossiplog.info("gossip from " + host + " to " + cc );
        return gossipService.postGossip(host, guest, cc, body, ip, headUrl, largeUrl, page,
                needAntiSpam, whisper, color);
    }

    public int sendGossip(int host, int guest, String body) {
        try {
            return gossipService.sendGossip(host, guest, body);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return 0;
        }
    }

    public int initGossipCount(int host) {
        return gossipService.initGossipCount(host);
    }

    public int getConversationCount(int host, int guest) {
        return gossipService.getConversationCount(host, guest);
    }

    public boolean hasGossipRight(User host, User guest, GossipRight right) {
        return gossipRightBiz.hasGossipRight(host, guest, right);
    }

    /**
     * 获取最近联系人
     * 
     * @param host 当前用户id
     * @return
     */
    public List<RecentContactView> getRecentContactList(int host) {
        ContactInfo[] contacts = RecentContactAdapter.getInstance().get(host,
                ContactType.XNMESSAGE, 0, 10);
        List<RecentContactView> contactList = new ArrayList<RecentContactView>();
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
            return contactList;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
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
        return gossipService.getConversation(host, guestId, limit, hostBeginId, hostEndId,
                guestBeginId, guestEndId, boundary);
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

        return gossipService.getConversation(host, guestId, count);
    }

    public JSONArray list2JSONArray(List<GossipView> list) throws Exception {

        JSONArray array = new JSONArray();
        if (null != list && list.size() > 0) {
            for (GossipView gossip : list) {
                array.put(getGossipJson(gossip));
            }
        }
        return array;
    }

    public JSONObject getGossipJson(GossipView gossip) {
        try {
            Map map = BeanUtils.describe(gossip);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm");
            map.put("time", format.format(gossip.getTime()));
            map.put("body", map.get("filterdBody"));
            map.remove("class");
            map.remove("htmlBody");
            return new JSONObject(map);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 取得用户留言（会过滤悄悄话）,不包括聊天消息
     * 
     * @param host 当前用户
     * @param guest 被访问用户
     * @param page 第几页(0,1,2...)
     * @param count 多少个
     * @throws NoPermissionException
     * @return
     */
    public List<GossipView> getGossipList(User host, User guest, int page, int count)
            throws NoPermissionException {

        return gossipService.getGossipList(host, guest, page, count);
    }

    /**
     * 取所有留言,包括聊天消息(做为悄悄话)
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

        return gossipService.getAllGossipList(host, guest, page, count);
    }

    /**
     * 只取出自己的留言,page用接口
     * 
     * @param ownerId
     * @param page
     * @param count
     * @param isArchive
     * @return
     */
    public List<GossipView> getOwnedGossipList(int owner, int page, int count, boolean isArchive) {
        return gossipService.getGossipByOwner(owner, page, count);
    }

    /**
     * page用接口
     * 
     * @param host
     * @param gossipId
     * @return
     */
    public GossipView getGossip(int host, long gossipId) {
        return gossipService.getGossip(host, gossipId);
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
        return gossipService.delete(id, userId, hostID);
    }

    /**
     * 得到留言总数<br>
     * 可以得到近期留言数或历史留言数
     * 
     * @deprecated
     * @param ownerId 留言所有者的用户Id
     * @param isArchive 废弃的参数
     */
    public int getGossipTotal(int ownerId, boolean isArchive) {
        return gossipService.getGossipCount(ownerId);
    }

    /**
     * 获取ownerid的留言数,不包括聊天记录
     * 
     * @param ownerId 被访问的用户
     * @param host 当前登录用户
     * @return
     */
    public int getGossipTotal(int ownerId, int host) {
        return gossipService.getGossipCount(ownerId, host);
    }

    /**
     * 获取ownerid的留言数 如果ownerid==host,返回包括聊天记录的全部留言数 否则,返回不包括聊天记录的留言数
     * 
     * @param ownerId 被访问的用户
     * @param host 当前登录用户
     * @return
     */
    public int getAllGossipCount(int ownerId, int host) {
        return gossipService.getAllGossipCount(ownerId, host);
    }
  
    /**
     * 得到留言总数(除去悄悄话)<br>
     * 可以得到近期留言数或历史留言数（除去悄悄话）
     * 
     * @param ownerId 留言所有者的用户Id
     * BY wenzhiwei
     */
    public int getPublicGossipCountForPage(int ownerId) {
    	return gossipService.getPublicGossipCount(ownerId);
    }
    
    /**
     * 获取ownerid的留言数
     * @param ownerId 所有者
     * @return
     * 2012-11-19
     */
    public int getGossipCountForpage(int ownerId) {
    	return gossipService.getGossipCountNew(ownerId);
    }
    
    public static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");

    String getTody() {
        return sdf.format(new Date());
    }

    public int getTodayStrangerNumber(int userId, int step) {
        try {
            MemCacheFactory fac = MemCacheFactory.getInstance();
            MemCacheManager mgr = fac.getManagerByPoolName(MemCacheKeys.pool_user_space);
            Integer gossipNum = null;
            String gossipkey = "user_stranger_gossip_number_everyday_" + getTody() + "_";
            if (mgr != null) {
                gossipNum = (Integer) mgr.get(gossipkey + userId);
                if (gossipNum != null && gossipNum > 0) {
                    gossipNum = gossipNum + step;
                } else {
                    gossipNum = step;
                }
                mgr.set(gossipkey + userId, Integer.valueOf(gossipNum),
                        new Date(MemCacheKeys.day*1));
                return gossipNum;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public int getTodayGossipNumber(int userId) {
        return getTodayGossipNumber(userId, 1);
    }

    public int getTodayGossipNumber(int userId, int cnt) {
        try {
            MemCacheFactory fac = MemCacheFactory.getInstance();
            MemCacheManager mgr = fac.getManagerByPoolName(MemCacheKeys.pool_user_space);
            Integer gossipUser = null;
            Integer gossipNum = null;
            String gossipkey = "user_gossip_everyday_" + getTody() + "_";;
            String gossipkey2 = "user_gossip_number_everyday_" + getTody() + "_";;
            if (mgr != null) {
                gossipUser = (Integer) mgr.get(gossipkey + userId);
                if (gossipUser == null || gossipUser == 0) {
                    mgr
                            .set(gossipkey + userId, Integer.valueOf(userId), new Date(
                                    MemCacheKeys.day));
                    gossipNum = cnt;
                } else {
                    gossipNum = (Integer) mgr.get(gossipkey2 + userId);
                    if (gossipNum != null && gossipNum > 0) {
                        gossipNum = gossipNum + cnt;
                    } else {
                        gossipNum = cnt;
                    }
                }
                mgr
                        .set(gossipkey2 + userId, Integer.valueOf(gossipNum), new Date(
                                MemCacheKeys.day));
                return gossipNum;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public GossipView getGossipByTime(int owner, int sender, Date time) {
        return gossipService.getGossipByTime(owner, sender, time);
    }
    
    /**
     * 按时间区间获取给别人留言的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    public List<GossipView> getOwnerGossipByTimeInterval(int owner,Date startime,Date endtime,int limit,int offset) {
    	return gossipService.getOwnerGossipByTimeInterval(owner, startime, endtime,limit,offset);
    }
    
    /**
     * 按时间区间获取别人给我留言的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    public List<GossipView> getOtherGossipByTimeInterval(int owner,Date startime,Date endtime,int limit,int offset) {
    	return gossipService.getOtherGossipByTimeInterval(owner, startime, endtime,limit,offset);
    }
    
    public void deleteGossipExt(long id, int owner) {
        gossipExtensionDao.remove(id, owner);
    }

    public static void main(String[] args) {
       // com.xiaonei.platform.core.cache.MemCacheFactory fac = com.xiaonei.platform.core.cache.MemCacheFactory
        //        .getInstance();
       // com.xiaonei.platform.core.cache.MemCacheManager mgr = fac
       //         .getManagerByPoolName(com.xiaonei.platform.core.cache.MemCacheKeys.pool_user_space);
       // mgr.delete("user_gossip_everyday_");
      //  mgr.delete("user_gossip_number_everyday_");
    	GossipHome ghome = new GossipHome();
    	try {
    	ghome.gossipService.sendGossip(267262005, 267262005, "hello");
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    }
}
