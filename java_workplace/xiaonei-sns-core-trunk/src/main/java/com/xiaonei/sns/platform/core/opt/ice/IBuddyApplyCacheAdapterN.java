package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;

import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheDataN;
import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheWithTotalN;

/**
 * 好友申请（）
 * @author Michael(liyong@opi-corp.com)
 *
 * @param <T>
 * @param <W>
 */
public interface IBuddyApplyCacheAdapterN<T extends BuddyApplyCacheDataN, W extends BuddyApplyCacheWithTotalN<T>>
        extends IRegisterable {

    List<T> getBuddyApplyCache(int userId, int begin, int limit);

    BuddyApplyCacheWithTotalN<T> getBuddyApplyCacheWithTotal(int userId, int begin, int limit);

    void removeBuddyApplyCache(int userId, int applicant);

    void clear(int userId);

    void rebuildBuddyApplyCache(int userId);

    int getHardLimit();

}
