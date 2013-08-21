package com.xiaonei.xce.buddyadapter.followresource;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import xce.buddy.FollowResourceCachePrx;
import xce.buddy.FollowResourceCachePrxHelper;
import xce.clusterstate.ReplicatedClusterAdapter;

import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

/**
 * <p>
 * Title: FollowResourceCacheAdapter.java
 * </p>
 * <p>
 * Description: 用户关注资源的缓存。与FollowResourceCache共用FollowRelatonWriter
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-8-30
 */
public class FollowResourceCacheAdapter extends ReplicatedClusterAdapter {

    public static final int MAX_IDSEQ_SIZE = 10; // 批量传送ID的最大长度

    private static final String endpoints = "ControllerFollowResourceCache";

    private static int interval = 120;

    private static FollowResourceCacheAdapter instance = null;

    private static final int TIME_OUT = 300;

    public static FollowResourceCacheAdapter getInstance() {
        if (instance == null) {
            synchronized (FollowResourceCacheAdapter.class) {
                if (instance == null) {
                    try {
                        instance = new FollowResourceCacheAdapter();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

            }
        }
        return instance;
    }

    private FollowResourceCacheAdapter() {
        super(endpoints, interval);
    }

    private FollowResourceCachePrx getFollowResourceCachePrxTwoWay(int id) {
        Ice.ObjectPrx prx0 = getValidProxy(id, TIME_OUT);
        if (prx0 != null) {
            FollowResourceCachePrx prx = FollowResourceCachePrxHelper.uncheckedCast(prx0
                    .ice_timeout(TIME_OUT).ice_twoway());
            return prx;
        } else {
            return null;
        }
    }

    /**
     * 验证参数是否合法
     */
    private boolean checkParam(int uid, int type) {
        if (uid < 0 || type < 0) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * 验证参数是否合法
     */
    private boolean checkParam(int uid, int type, int begin, int limit) {
        // limit 不能为负数
        if (!checkParam(uid, type) || begin < 0 || limit <= 0) {
            return false;
        }

        return true;
    }

    //TODO 打印TYPE对应的字符串，而不是TYPE

    /**
     * 获取用户关注的某类型资源数
     * 
     * @param userId 用户ID
     * @param type 资源类型ID
     * @return 资源数量
     */
    private int getCount(int userId, int type) {
        int count = 0;

        if (!checkParam(userId, type)) {
            return 0;
        }

        FollowResourceCachePrx proxy = null;
        try {
            proxy = getFollowResourceCachePrxTwoWay(userId);
            count = proxy.getCount(userId, type);
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getCount userId:" + userId + " type:" + type
                            + e.ice_name() + " " + proxy.ice_toString());
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getCount userId:" + userId + " type:" + type
                            + e.ice_name() + " " + proxy.ice_toString());
        } catch (Exception e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getCount userId:" + userId + " type:" + type
                            + e.toString() + " " + proxy.ice_toString());
            e.printStackTrace();
        }
        return count;
    }

    /**
     * 获取用户关注的某类资源的ID列表。
     * 
     * @param userId 用户ID
     * @param type 用户关注的资源类型
     * @param begin 分页参数，从列表begin位置返回
     * @param limit 分页参数，返回列表长度为limit
     * @return 资源ID列表。该ID为资源在类别内部的ID，而不是gid
     */
    private List<Long> getList(int userId, int type, int begin, int limit) {
        List<Long> idList = new ArrayList<Long>();

        if (!checkParam(userId, type, begin, limit)) {
            return idList;
        }

        FollowResourceCachePrx proxy = null;
        try {
            proxy = getFollowResourceCachePrxTwoWay(userId);
            long[] ids = proxy.getList(userId, type, begin, limit);
            if (ids != null && ids.length != 0) {
                for (long i : ids) {
                    idList.add(i);
                }
            }
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getList userId:" + userId + " type:" + type
                            + " begin:" + begin + " limit:" + limit + e.ice_name() + " "
                            + proxy.ice_toString());
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getList userId:" + userId + " type:" + type
                            + " begin:" + begin + " limit:" + limit + e.ice_name() + " "
                            + proxy.ice_toString());
        } catch (Exception e) {
            Oce.getLogger().error(
                    getClass().getName() + ".getList userId:" + userId + " type:" + type
                            + " begin:" + begin + " limit:" + limit + e.toString() + " "
                            + proxy.ice_toString());
            e.printStackTrace();
        }

        return idList;
    }

    /**
     * 返回Integer类型的id列表
     */
    private List<Integer> getIntegerList(int userId, int type, int begin, int limit) {
        List<Integer> idList = new ArrayList<Integer>();

        List<Long> longIdList = getList(userId, type, begin, limit);

        for (Iterator<Long> it = longIdList.iterator(); it.hasNext();) {
            long longId = it.next();
            idList.add((int) longId);
        }

        return idList;
    }

    /**
     * 判断一个用户是否跟某类型下的一批资源有关系 限制传入的资源ID列表长度为 MAX_IDSEQ_SIZE， 超过的ID将被截断
     * 
     * @param userId 用户ID
     * @param type 资源类型
     */
    private Map<Integer, Boolean> hasRelationBatch(int userId, int type, List<Integer> idList) {

        Map<Integer, Boolean> resultMap = new HashMap<Integer, Boolean>();

        if (!checkParam(userId, type) || idList.size() == 0) {
            return resultMap;
        }

        int[] ids;
        if (idList.size() > MAX_IDSEQ_SIZE) {
            ids = new int[MAX_IDSEQ_SIZE];
        } else {
            ids = new int[idList.size()];
        }

        FollowResourceCachePrx proxy = null;
        try {
            proxy = getFollowResourceCachePrxTwoWay(userId);
            for (int i = 0; i < ids.length; ++i) {
                ids[i] = idList.get(i);
            }
            boolean[] result = proxy.hasRelationBatch(userId, type, ids);
            if (result != null && result.length != 0) {
                for (int i = 0; i < result.length; ++i) {
                    resultMap.put(ids[i], result[i]);
                }
            }
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".hasRelationBatch userId:" + userId + " type:" + type
                            + " ids.length:" + ids.length + e.ice_name() + " "
                            + proxy.ice_toString());
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".hasRelationBatch userId:" + userId + " type:" + type
                            + " ids.length:" + ids.length + e.ice_name() + " "
                            + proxy.ice_toString());
        } catch (Exception e) {
            Oce.getLogger().error(
                    getClass().getName() + ".hasRelationBatch userId:" + userId + " type:" + type
                            + " ids.length:" + ids.length + e.toString() + " "
                            + proxy.ice_toString());
            e.printStackTrace();
        }

        return resultMap;
    }

    /**
     * 获取用户关注的Wiki数量
     */
    public int getSocialWikiCount(int userId) {
        return getCount(userId, GlobalId.fromSocialWiki(0).getType());
    }

    /**
     * 获取用户关注的Wiki列表
     */
    public List<Integer> getSocialWikiList(int userId, int begin, int limit) {
        return getIntegerList(userId, GlobalId.fromSocialWiki(0).getType(), begin, limit);
    }

    /**
     * 获取用户正在进行的Wiki数
     */
    public int getSocialWikiDoingCount(int userId) {
        return getCount(userId, GlobalId.fromSocialWikiDoing(0).getType());
    }

    /**
     * 获取用户正在进行的Wiki列表
     */
    public List<Integer> getSocialWikiDoingList(int userId, int begin, int limit) {
        return getIntegerList(userId, GlobalId.fromSocialWikiDoing(0).getType(), begin, limit);
    }

    /**
     * 获取用户想做的wiki数
     */
    public int getSocialWikiWillDoCount(int userId) {
        return getCount(userId, GlobalId.fromSocialWikiWillDo(0).getType());
    }

    /**
     * 获取用户想做的wiki数
     */
    public List<Integer> getSocialWikiWillDoList(int userId, int begin, int limit) {
        return getIntegerList(userId, GlobalId.fromSocialWikiWillDo(0).getType(), begin, limit);
    }

    /**
     * 获取用户做过的wiki数
     */
    public int getSocialWikiDoneCount(int userId) {
        return getCount(userId, GlobalId.fromSocialWikiDone(0).getType());
    }

    /**
     * 获取用户做过的wiki列表
     */
    public List<Integer> getSocialWikiDoneList(int userId, int begin, int limit) {
        return getIntegerList(userId, GlobalId.fromSocialWikiDone(0).getType(), begin, limit);
    }

    /**
     * 判断一个用户是否与一组WIKI资源有关系
     * 
     * @param userId 用户ID
     * @param idList 资源ID列表，限制最大长度为MAX_IDSEQ_SIZE,超出将被截断
     * @return
     */
    public Map<Integer, Boolean> hasRelationSocialWiki(int userId, List<Integer> idList) {
        return hasRelationBatch(userId, GlobalId.fromSocialWiki(0).getType(), idList);
    }

    /**
     * 判断一个用户是否正在做一组WIKI资源
     * 
     * @param userId 用户ID
     * @param idList 资源ID列表，限制最大长度为MAX_IDSEQ_SIZE,超出将被截断
     * @return
     */
    public Map<Integer, Boolean> hasRelationSocialWikiDoing(int userId, List<Integer> idList) {
        return hasRelationBatch(userId, GlobalId.fromSocialWikiDoing(0).getType(), idList);
    }

    /**
     * 判断一个用户是否想做一组WIKI资源
     * 
     * @param userId 用户ID
     * @param idList 资源ID列表，限制最大长度为MAX_IDSEQ_SIZE,超出将被截断
     * @return
     */
    public Map<Integer, Boolean> hasRelationSocialWikiWillDo(int userId, List<Integer> idList) {
        return hasRelationBatch(userId, GlobalId.fromSocialWikiWillDo(0).getType(), idList);
    }

    /**
     * 判断一个用户是否做过一组WIKI资源
     * 
     * @param userId 用户ID
     * @param idList 资源ID列表，限制最大长度为MAX_IDSEQ_SIZE,超出将被截断
     * @return
     */
    public Map<Integer, Boolean> hasRelationSocialWikiDone(int userId, List<Integer> idList) {
        return hasRelationBatch(userId, GlobalId.fromSocialWikiDone(0).getType(), idList);
    }

    /**
     * 重新加载缓存数据
     * 
     * @param userId 用户ID
     * @param type 用户关注的资源类型
     */
    public void reload(int userId, int type) {
        if (!checkParam(userId, type)) {
            return;
        }

        FollowResourceCachePrx proxy = null;

        try {
            proxy = getFollowResourceCachePrxTwoWay(userId);
            proxy.reload(userId, type);
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".reload userId:" + userId + " type:" + type
                            + e.ice_name() + " " + proxy.ice_toString());
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    getClass().getName() + ".reload userId:" + userId + " type:" + type
                            + e.ice_name() + " " + proxy.ice_toString());
        } catch (Exception e) {
            Oce.getLogger().error(
                    getClass().getName() + ".reload userId:" + userId + " type:" + type
                            + e.toString() + " " + proxy.ice_toString());
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        int userId = 123;
        int type = 20;

        int count = FollowResourceCacheAdapter.getInstance().getCount(userId, type);
        System.out.println(count);
        List<Long> idList = FollowResourceCacheAdapter.getInstance().getList(userId, type, 0, 10);

        for (Iterator<Long> it = idList.iterator(); it.hasNext();) {
            System.out.println("--" + it.next());
        }

        List<Integer> list = new ArrayList<Integer>();
        list.add(0);
        list.add(1);
        list.add(2);
        list.add(3);
        list.add(4);
        list.add(5);
        list.add(6);
        list.add(7);
        list.add(8);
        list.add(9);
        list.add(10);
        list.add(11);
        list.add(12);

        Map<Integer, Boolean> result = FollowResourceCacheAdapter.getInstance().hasRelationBatch(
                userId, type, list);

        for (Map.Entry<Integer, Boolean> e : result.entrySet()) {
            System.out.println(e.getKey() + " " + e.getValue());
        }

        FollowResourceCacheAdapter.getInstance().reload(userId, type);

        System.exit(0);
    }
}
