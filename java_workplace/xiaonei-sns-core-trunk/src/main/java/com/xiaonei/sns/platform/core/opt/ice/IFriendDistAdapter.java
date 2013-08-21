/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;

import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WFriendDistWithTotal;

/**
 * @author lookis (comic.liu@gmail.com)
 *
 */
public interface IFriendDistAdapter  extends IRegisterable{

    public abstract WFriendDistWithTotal getFriendDistWithTotal(int userId);

    public abstract List<WFriendDistCount> getFriendDistListAll(int userId);

    public abstract int getFriendDistByUniversity(int userId, int universityId);

    public abstract void rebuildFriendDist(int userId);

}
