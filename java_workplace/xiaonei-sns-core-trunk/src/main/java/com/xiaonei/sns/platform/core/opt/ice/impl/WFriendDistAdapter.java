package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WFriendDistWithTotal;
import com.xiaonei.platform.core.model.factory.WFriendDistFactory;
import com.xiaonei.sns.platform.core.opt.ice.IFriendDistAdapter;
import com.xiaonei.xce.frienddist.FriendDistAdapter;

public class WFriendDistAdapter implements IFriendDistAdapter {

    /**
     * @param factory
     */

    private final FriendDistAdapter<WFriendDistCount, WFriendDistWithTotal> adapter = new FriendDistAdapter<WFriendDistCount, WFriendDistWithTotal>(
            new WFriendDistFactory());

    protected WFriendDistAdapter() {

    }

    private static IFriendDistAdapter instance = new WFriendDistAdapter();
    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getFriendDistAdapter()}
     */
    public static IFriendDistAdapter getInstance() {
        //return WFriendDistAdapter.instance;
    	return SnsAdapterFactory.getFriendDistAdapter();
    }

    private static boolean registered = false;

    public synchronized static void register(IFriendDistAdapter newInstance) {
        //拆分core包，如果以后sns还要再拆分，那么就不用一个一个的改类包路径，实现了自己的Adapter然后在函数里注册一下
        if (!registered && (newInstance != null) && !newInstance.equals(instance)) {
            instance = newInstance;
            registered = true;
        }
    }

    /* (non-Javadoc)
     * @see com.xiaonei.sns.platform.core.opt.ice.impl.IFriendDistAdapter#getFriendDistWithTotal(int)
     */
    public WFriendDistWithTotal getFriendDistWithTotal(int userId) {
        return adapter.getFriendDistWithTotal(userId);

    }

    /* (non-Javadoc)
     * @see com.xiaonei.sns.platform.core.opt.ice.impl.IFriendDistAdapter#getFriendDistListAll(int)
     */
    public List<WFriendDistCount> getFriendDistListAll(int userId) {
        return adapter.getFriendDistListAll(userId);
    }

    /* (non-Javadoc)
     * @see com.xiaonei.sns.platform.core.opt.ice.impl.IFriendDistAdapter#getFriendDistByUniversity(int, int)
     */
    public int getFriendDistByUniversity(int userId, int universityId) {
        return adapter.getFriendDistByUniversity(userId, universityId);
    }

    /* (non-Javadoc)
     * @see com.xiaonei.sns.platform.core.opt.ice.impl.IFriendDistAdapter#rebuildFriendDist(int)
     */
    public void rebuildFriendDist(int userId) {
        adapter.rebuildFriendDist(userId);
    }

}
