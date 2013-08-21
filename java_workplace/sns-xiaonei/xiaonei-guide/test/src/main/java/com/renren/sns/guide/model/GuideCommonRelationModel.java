package com.renren.sns.guide.model;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.offerfriends.DefaultOffer;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午10:57:34  * 
 */
public class GuideCommonRelationModel extends GuideAbstractModel {

    public GuideCommonRelationModel(User host, int limit) {
        super(host, limit);
    }

    @Override
    public List<GuideIRelation> getRelations(User host, int offset, int limit) {
        this.setBegin(System.currentTimeMillis());
        final List<DefaultOffer> friends = SnsAdapterFactory.getOfferFriendsAdapter().getBestOffer(host.getId(), limit);
        if (friends == null || friends.size() == 0) {
            return this.getResult();
        }
        GuideSearchRelation relation = new GuideSearchRelation();
        relation.setList(parse(friends));
        relation.setTitle("你可能认识的人");
        relation.setMoreLink("");
        relation.setTypeName("你可能认识的人");
        relation.setType("mayknow");
        relation.setCount(friends.size());
        getResult().add(relation);
        this.setEnd(System.currentTimeMillis());
        return this.getResult();
    }

    private List<GuideIRelationFriend> parse(final List<DefaultOffer> friends) {
        final List<GuideIRelationFriend> ret = new ArrayList<GuideIRelationFriend>();
        List<Integer> ids = new ArrayList<Integer>();
        for (DefaultOffer fm : friends) {
            ids.add(fm.getUserId());
        }
        Map<Integer, WUserCache> wucMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(ids);
        for (int i = 0; i < friends.size(); i++) {
            final DefaultOffer friendCommon = friends.get(i);
            final WUserCache wuc = wucMap.get(friendCommon.getUserId());
            if (wuc == null) {
                continue;
            }
            ret.add(new GuideIRelationFriend() {
                public String getTiny_Url() {
                    return wuc.getTiny_Url();
                }
                public String getHead_Url() {
                    return null;
                }
                public int getId() {
                    return friendCommon.getUserId();
                }
                public String getName() {
                    return wuc.getName();
                }
                @Override
                public String getGender() {
                    return wuc.getGender();
                }
            });
        }
        return ret;
    }
}
