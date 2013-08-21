package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;

import com.xiaonei.xce.offerfriends.DefaultOffer;


/**
 * 新版本“二度好友”
 * @author Michael(liyong@opi-corp.com)
 * @author tai.wang@opi-corp.com Sep 14, 2010 - 10:51:52 AM
 */
public interface IOfferFriendsAdapter extends IRegisterable {

    List<DefaultOffer> getBestOffer(int userId, int limit);
    
}
