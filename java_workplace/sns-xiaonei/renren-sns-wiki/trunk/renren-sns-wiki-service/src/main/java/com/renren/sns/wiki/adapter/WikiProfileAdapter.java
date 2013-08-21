package com.renren.sns.wiki.adapter;

import java.util.HashSet;
import java.util.Set;

import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;
import com.xiaonei.xce.buddyadapter.followrelation.FollowRelationWriterAdapter;

public class WikiProfileAdapter {

    private Set<Integer> profileIdSet = null;

    private static WikiProfileAdapter instance = null;

    public static WikiProfileAdapter getInstance() {
        if (instance == null) {
            synchronized (FollowRelationWriterAdapter.class) {
                if (instance == null) {
                    try {
                        instance = new WikiProfileAdapter();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }
        return instance;
    }

    private WikiProfileAdapter() {
        int[] wikiProfileIds = { 740550185, 740586967, 740588656, 740700332, 740721112, 740771419,
                740804152, 740837843, 740923528, 741056710, 741073939, 741272968, 741275728,
                741324389, 741345665, 741546095, 741575955, 741772408, 741853191, 741949499,
                742009764, 742090055, 742432896, 742487740, 742502630, 742535173, 742549057,
                742596928, 742684946, 742688663, 742725312, 742821005, 742862366, 742958414,
                742985247, 743067137, 743149284, 743320568, 743344157, 743351053, 743454183,
                743549693, 743559397, 743562890, 743564826, 743570789, 743652294, 743664959,
                743742667, 743791491, 743807640, 743813941, 743875506, 743877775, 743889259,
                743915052, 743957905, 743966097, 743995627, 744042748, 744098836, 744113616,
                744120416, 744129125, 744205370, 744243155, 744276765, 744325069, 744464149,
                744643507, 744849579, 744916048, 745054456, 745074454, 745121407, 745348812,
                745400158, 745439762, 745624231, 745804586, 745825105, 745871648, 745941575,
                746149297, 746154659, 746304981, 746338613, 746375771, 746457113, 746466493,
                746475861, 746725264, 746747925, 746771523, 746833471, 747145443, 751598356,
                753623629, 754016552, 754541223, 890302489, 912108214, 913672543, 945686721,
                1035010612, 1038382609, 1038894374, 1103722835, 1150510524, 1170161744, 1171198712,
                1176636711, 1187738287, 1194441403, 1280856965, 1318423899, 1389256229, 1470542455,
                1474941930, 1480965096, 1499982156, 1565234265, 1647124619, 1657816825, 1662870760,
                1679205954, 1709294933, 1752605701, 1776952468, 1787493919, 1855197675, 1877026020,
                1889156040, 1890086254, 1937329664, 1964055167, 1982964269, 1984825401, 1985421547,
                2057093090, 2082864250, 735385707, 735452817, 735482675, 735633751, 735654527,
                736442888, 736478742, 736514258, 736729857, 736780307, 736796272, 736882369,
                736927399, 736984740, 737082326, 737104532, 737142130, 737181615, 737267704,
                737291671, 737372927, 737492270, 737533876, 737578367, 737799971, 737867724,
                738030714, 738287431, 738362975, 738388525, 738670702, 738743810, 738768488,
                739006821, 739150816, 739177885, 739323806, 739353358, 739425438, 739433152,
                739633646, 739955883, 739965526, 740047976, 740082706, 740093376, 740267938,
                740366681, 740398872, 740399343, 740454806, 733746471, 733777997, 734263180,
                734567653, 734936358, 735281869, 735356390, 733704461, 431716353, 428141914,
                431716353, 229528266 };

        profileIdSet = new HashSet<Integer>();
        for (int userId : wikiProfileIds) {
            profileIdSet.add(userId);
        }
    }

    /**
     * 是否在主页显示的用户
     * 
     * @return
     */
    public boolean isProfileUser(int userId) {
        //可以设置一个过期日期,过期后一定返回false

        boolean result = false;
        if (profileIdSet.contains(userId)) {
            result = true;
        } else {
            //获取缓存列表
            Set<Integer> cacheUserIdSet = getCacheUserIdSet();
            if (cacheUserIdSet.contains(userId)) {
                result = true;
            } else if (cacheUserIdSet.size() >= 200) {
                result = false;
            } else {
                cacheUserIdSet.add(userId);
                setCacheUserIdSet(cacheUserIdSet);
                result = true;
            }
        }
        return result;
    }

    /**
     * 用户是否在缓存里
     * 
     * @return
     */
    public boolean isCacheUser(int userId) {
        Set<Integer> cacheUserIdSet = getCacheUserIdSet();
        return cacheUserIdSet.contains(userId);
    }

    //获取缓存列表
    @SuppressWarnings("unchecked")
    public Set<Integer> getCacheUserIdSet() {
        Set<Integer> set = CacheDelegator.get(new QueryAction<Set>(CacheNameSpace.WIKI,
                CachePrefix.WIKI_PROFILE_USER_LIST, Set.class, "temp") {

            @Override
            public Set<Integer> getFromOther() {
                return null;
            }
        }, null);
        if (set == null) {
            set = new HashSet<Integer>();
        }
        return set;
    }

    public void setCacheUserIdSet(Set<Integer> set) {
        CacheDelegator.set(CacheNameSpace.WIKI, CachePrefix.WIKI_PROFILE_USER_LIST, Set.class,
                "temp", set);
    }

}
