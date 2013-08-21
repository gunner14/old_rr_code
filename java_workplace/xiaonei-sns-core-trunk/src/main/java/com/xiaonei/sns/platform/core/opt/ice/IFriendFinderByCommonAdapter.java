package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;

import com.xiaonei.xce.friendfinderbycommon.CommonFriendData;

public interface IFriendFinderByCommonAdapter extends IRegisterable {

    List<CommonFriendData> getCommonFriendItem(int userId, int limit);

    List<Integer> getFriendsByCommon(int guestId, int hostId, int limit);

}
