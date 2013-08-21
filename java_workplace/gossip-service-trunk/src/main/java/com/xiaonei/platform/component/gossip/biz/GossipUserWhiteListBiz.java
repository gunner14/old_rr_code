package com.xiaonei.platform.component.gossip.biz;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.platform.component.gossip.dao.GossipUserWhiteListDAO;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

@Service
public class GossipUserWhiteListBiz {

    private Log logger = LogFactory.getLog(this.getClass());

    private MemCacheManager cache = MemCacheFactory.getInstance().getManagerByPoolName(
            MemCacheKeys.pool_user_space);

    @Autowired
    private GossipUserWhiteListDAO gossipUserWhiteListDao;

    public int save(int user) {
        cache.delete(getKey());
        return gossipUserWhiteListDao.save(user);
    }

    public int update(int user, int enable) {
        cache.delete(getKey());
        return gossipUserWhiteListDao.update(user, enable);
    }

    /**
     * 获得留言白名单
     * 
     * @return
     */
    public List<Integer> findUserWhiteList() {
        List<Integer> list = (List<Integer>) cache.get(getKey());
        if (list != null) {
            return list;
        } else {
            try {
                list = gossipUserWhiteListDao.findWhiteList();
            } catch (Exception e) {
                try {
                    list = gossipUserWhiteListDao.findWhiteList();
                    return list;
                } catch (Exception e1) {
                    logger.error("DB发生异常:" + e1.getMessage());
                    return null;
                }
            }
            if (list != null) {
                cache.set(getKey(), list);
            }
            return list;
        }
    }

    private String getKey() {
        return "user_gossip_user_white_list";
    }
}
