package com.xiaonei.reg.guide.action;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.component.friends.selector.CharacterSelector;
import com.xiaonei.platform.component.friends.selector.filter.FriendsPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

public class GuideOfferRandomSelector extends CharacterSelector {

    @Override
    protected List<Integer> getCandidate(User host, Map<String, String> params) {
        int limit = 73;
        List<DefaultOffer> result = OfferFriendsAdapter.getInstance().getRandomOffer(host.getId(),limit);
        List<Integer> list = new ArrayList<Integer>();
        if (result != null) {
            for (int i = 0; i < result.size(); i++) {
                list.add(result.get(i).getUserId());
            }
        }
        return list;
    }

    @Override
    protected IPinyinFilter getPinyinFilter() {
        return new FriendsPinyinFilter();
    }

}
