package com.renren.sns.wiki.delegator.xce;

import java.util.Map;

import org.springframework.stereotype.Service;

import xce.ilikeit.LikeInfo;

import com.xiaonei.xce.ilikeit.ILikeItCacheAdapter;
import com.xiaonei.xce.ilikeit.ILikeItLogicAdapter;


/**
 *喜欢封装的接口
 * @author weiwei.wang@renren-inc.com since 2012-5-10
 *
 */
@Service
public class LikeLogicDelegator {


    /**
     * 喜欢操作
     * 
     * @param gidStr
     * @param uid
     */
    public void addLike(String gidStr, int userId) {
        ILikeItLogicAdapter.instance().addLike(gidStr, userId);
    }

    /**
     * 取消喜欢操作
     * 
     * @param gidStr
     * @param uid
     */
    public void removeLike(String gidStr, int userId) {
        ILikeItLogicAdapter.instance().removeLike(gidStr, userId);
    }

    /**
     * 根据gidStrs和userId,获取喜欢的具体信息
     * @param gidStrs
     * @param userid
     * @param limit
     * @param detailNum
     * @return
     */
    public Map<String, LikeInfo> getLikeDetailMapBatch(String[] gidStrs, int userId) {
        return ILikeItCacheAdapter.instance().getLikeDetailMapBatch(gidStrs, userId, 999999,
                gidStrs.length);
    }
}
