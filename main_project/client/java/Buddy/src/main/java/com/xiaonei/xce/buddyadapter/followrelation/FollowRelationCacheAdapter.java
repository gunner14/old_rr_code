package com.xiaonei.xce.buddyadapter.followrelation;

import java.util.ArrayList;
import java.util.List;

import xce.buddy.FollowRelationCachePrx;
import xce.buddy.FollowRelationCachePrxHelper;
import xce.clusterstate.ReplicatedClusterAdapter;

import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

/**
 * <p>
 * Title: FollowRelationCacheAdapter.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-11-13
 */
public class FollowRelationCacheAdapter extends ReplicatedClusterAdapter {

    private static final String endpoints = "ControllerFollowRelationCache";

    private static int interval = 120;

    private static FollowRelationCacheAdapter instance = null;

    private static final int TIME_OUT = 5000;

    public static final int LONG_TO_INT_MOD = 10000;

    public static FollowRelationCacheAdapter getInstance() {
        if (instance == null) {
            synchronized (FollowRelationCacheAdapter.class) {
                if (instance == null) {
                    try {
                        instance = new FollowRelationCacheAdapter();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

            }
        }
        return instance;
    }

    private FollowRelationCacheAdapter() {
        super(endpoints, interval);
    }

    private FollowRelationCachePrx getFollowRelationCachePrxTwoWay(int id) {
        Ice.ObjectPrx prx0 = getValidProxy(id, TIME_OUT);
        if (prx0 != null) {
            FollowRelationCachePrx prx = FollowRelationCachePrxHelper.uncheckedCast(prx0
                    .ice_timeout(TIME_OUT).ice_twoway());
            return prx;
        } else {
            return null;
        }
    }

    /**
     * 验证参数
     * 
     * @param uid
     * @param pid
     * @param begin
     * @param limit
     * @return
     */
    private boolean checkParam(int uid, GlobalId gid, int begin, int limit) {
        // limit 不能为负数
        if (uid < 0 || begin < 0 || limit < 0) {
            return false;
        }
        return true;
    }

    private String getMethodName() {
        // 0 for getStackTrace
        // 1 for getMethodName
        // 2 for Method name that invoke this method
        return Thread.currentThread().getStackTrace()[2].getMethodName();
    }

    /**
     * 取userId好友中是GlobalId好友的人 请注意begin,limit的使用
     * 
     * @param uid
     * @param gid
     * @param begin 起始位置（从0开始）
     * @param limit 获取长度（必须指定）
     * @return 共同好友ID列表
     * @throws Exception
     */
    public List<Integer> getCommonFriend(int uid, GlobalId gid, int begin, int limit)
            throws FollowRelationException {

        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + uid + ", "
                + gid.toLong() + ", " + begin + ", " + limit + ")";

        if (!checkParam(uid, gid, begin, limit)) {
            return new ArrayList<Integer>();
        }

        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        List<Integer> commonFansList = new ArrayList<Integer>();
        FollowRelationCachePrx proxy = null;
        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            int[] array = proxy.getCommon(uid, myGid, begin, limit);
            if (array != null && array.length != 0) {
                for (int i : array)
                    commonFansList.add(i);
            }
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }
        return commonFansList;

    }

    /**
     * 取userId好友中是GlobalId好友的人。只取在time时间之后与GlobalId有关系的好友。请注意begin,limit的使用
     * 
     * @param uid
     * @param gid
     * @param time 时间(unix时间戳,从1970年1月1日00:00:00开始的秒数)
     * @param begin 起始位置（从0开始）
     * @param limit 获取长度（必须指定）
     * @return 共同好友ID列表
     * @throws Exception
     */
    public List<Integer> getCommonFriendAfterTime(int uid, GlobalId gid, long time, int begin,
            int limit) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + uid + ", "
                + gid.toLong() + ", " + time + ", " + begin + ", " + limit + ")";

        if (!checkParam(uid, gid, begin, limit)) {
            return new ArrayList<Integer>();
        }

        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        List<Integer> commonFansList = new ArrayList<Integer>();
        FollowRelationCachePrx proxy = null;
        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            int[] array = proxy.getCommonAfterTime(uid, myGid, time, begin, limit);
            if (array != null && array.length != 0) {
                for (int i : array)
                    commonFansList.add(i);
            }
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }
        return commonFansList;
    }

    /**
     * 获取与uid共同关注gid的好友数量
     * 
     * @param uid
     * @param gid
     * @return 共同好友数量
     * @throws Exception
     */
    public int getCommonFriendCount(int uid, GlobalId gid) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + uid + ", "
                + gid.toLong() + ")";

        int count = 0;
        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            count = proxy.getCommonCount(uid, myGid);

        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }

        return count;
    }

    /**
     * 获取与uid共同关注gid的好友数量。只取在time时间之后与gid有关系的好友数量
     * 
     * @param uid
     * @param gid
     * @param time 时间(unix时间戳,从1970年1月1日00:00:00开始的秒数)
     * @return 共同好友数量
     * @throws Exception
     */
    public int getCommonFriendCountAfterTime(int uid, GlobalId gid, long time)
            throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + uid + ", "
                + gid.toLong() + ", " + time + ")";

        int count = 0;
        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            count = proxy.getCommonCountAfterTime(uid, myGid, time);
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }

        return count;
    }

    /**
     * 获取关注gid的用户数量
     * 
     * @param gid
     * @return 用户数量
     * @throws Exception
     */
    public int getCount(GlobalId gid) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + gid.toLong() + ")";

        int count = 0;
        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            count = proxy.getCount(myGid);

        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }

        return count;
    }

    /**
     * 获取关注gid的用户id列表
     * 
     * @param gid
     * @param begin 起始位置（从0开始）
     * @param limit 获取长度（必须指定）
     * @return 用户id列表
     * @throws Exception
     */
    public List<Integer> getList(GlobalId gid, int begin, int limit) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + gid.toLong()
                + ", " + begin + ", " + limit + ")";

        if (!checkParam(0, gid, begin, limit)) {
            return new ArrayList<Integer>();
        }

        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        List<Integer> result = new ArrayList<Integer>();
        int[] idList;
        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            idList = proxy.getList(myGid, begin, limit);
            if (idList != null && idList.length != 0) {
                for (int i : idList) {
                    result.add(i);
                }
            }

        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }

        return result;
    }

    /**
     * 判断uid是否是gid的关注者
     * 
     * @param uid
     * @param gid
     * @return 若uid关注了gid，返回true；否则返回false
     * @throws Exception
     */
    public boolean hasRelation(int uid, GlobalId gid) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + uid + ", "
                + gid.toLong() + ")";

        boolean hasRelation = false;
        MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (gid.toLong() % LONG_TO_INT_MOD));
            hasRelation = proxy.hasRelation(uid, myGid);

        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }

        return hasRelation;
    }

    /**
     * 重新加载粉丝数据
     * 
     * @param gid
     * @throws Exception
     */
    public void reload(long globalId) throws FollowRelationException {
        String invokeInfo = getClass().getName() + "." + getMethodName() + "(" + globalId + ")";

        FollowRelationCachePrx proxy = null;

        try {
            proxy = getFollowRelationCachePrxTwoWay((int) (globalId % LONG_TO_INT_MOD));
            proxy.reload(globalId);
        } catch (Ice.ConnectTimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        } catch (Exception e) {
            Oce.getLogger().error(invokeInfo + " " + e.toString() + " " + proxy.ice_toString());
            throw new FollowRelationException(e);
        }
    }

    /**
     * main 函数主要提供给脚本调用
     */
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: FollowRelationCacheAdapter Command Args...");
            System.exit(0);
        }

        String cmd = args[0];

        String[] leftArgs = new String[args.length - 1];
        for (int i = 0; i < leftArgs.length; ++i) {
            leftArgs[i] = args[i + 1];
        }

        /************************************************
         * getList
         ***********************************************/
        if (cmd.equals("getList")) {
            if (leftArgs.length < 3) {
                System.out.println("Usage: getList gid begin limit");
                System.exit(0);
            }

            long globalId = Long.parseLong(leftArgs[0]);
            GlobalId gid = new GlobalId(globalId);
            int begin = Integer.parseInt(leftArgs[1]);
            int limit = Integer.parseInt(leftArgs[2]);

            List<Integer> list = new ArrayList<Integer>();
            try {
                list = FollowRelationCacheAdapter.getInstance().getList(gid, begin, limit);
            } catch (Exception e) {
                e.printStackTrace();
            }
            for (Integer i : list) {
                System.out.println(i);
            }
            System.exit(0);
        }

        /************************************************
         * getCommonFriend
         ***********************************************/
        if (cmd.equals("getCommonFriend")) {
            if (leftArgs.length < 4) {
                System.out.println("Usage: getCommonFriend userid gid begin limit");
                System.exit(0);
            }

            int userid = Integer.parseInt(leftArgs[0]);
            long globalId = Long.parseLong(leftArgs[1]);
            GlobalId gid = new GlobalId(globalId);
            int begin = Integer.parseInt(leftArgs[2]);
            int limit = Integer.parseInt(leftArgs[3]);

            List<Integer> list = new ArrayList<Integer>();
            try {
                list = FollowRelationCacheAdapter.getInstance().getCommonFriend(userid, gid, begin,
                        limit);
            } catch (Exception e) {
                e.printStackTrace();
            }
            for (Integer i : list) {
                System.out.println(i);
            }
            System.exit(0);
        }

        /************************************************
         * getCommonFriendAfterTime
         ***********************************************/
        if (cmd.equals("getCommonFriendAfterTime")) {
            if (leftArgs.length < 5) {
                System.out.println("Usage: getCommonFriendAfterTime userid gid time begin limit");
                System.exit(0);
            }

            int userid = Integer.parseInt(leftArgs[0]);
            long globalId = Long.parseLong(leftArgs[1]);
            GlobalId gid = new GlobalId(globalId);
            long time = Long.parseLong(leftArgs[2]);
            int begin = Integer.parseInt(leftArgs[3]);
            int limit = Integer.parseInt(leftArgs[4]);
            List<Integer> list = new ArrayList<Integer>();
            try {
                list = FollowRelationCacheAdapter.getInstance().getCommonFriendAfterTime(userid,
                        gid, time, begin, limit);
            } catch (Exception e) {
                e.printStackTrace();
            }

            for (Integer i : list) {
                System.out.println(i);
            }
            System.exit(0);
        }

        /************************************************
         * getCount
         ***********************************************/
        if (cmd.equals("getCount")) {
            if (leftArgs.length < 1) {
                System.out.println("Usage: getCount gid");
                System.exit(0);
            }

            long globalId = Long.parseLong(leftArgs[0]);
            GlobalId gid = new GlobalId(globalId);

            int count = 0;
            try {
                count = FollowRelationCacheAdapter.getInstance().getCount(gid);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(count);
            System.exit(0);
        }

        /************************************************
         * getCommonFriendCount
         ***********************************************/
        if (cmd.equals("getCommonFriendCount")) {
            if (leftArgs.length < 2) {
                System.out.println("Usage: getCommonFriendCount userid gid");
                System.exit(0);
            }

            int userid = Integer.parseInt(leftArgs[0]);
            long globalId = Long.parseLong(leftArgs[1]);
            GlobalId gid = new GlobalId(globalId);

            int count = 0;
            try {
                count = FollowRelationCacheAdapter.getInstance().getCommonFriendCount(userid, gid);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(count);
            System.exit(0);
        }

        /************************************************
         * getCommonFriendCountAfterTime
         ***********************************************/
        if (cmd.equals("getCommonFriendCountAfterTime")) {
            if (leftArgs.length < 3) {
                System.out.println("Usage: getCommonFriendCountAfterTime userid gid time");
                System.exit(0);
            }

            int userid = Integer.parseInt(leftArgs[0]);
            long globalId = Long.parseLong(leftArgs[1]);
            GlobalId gid = new GlobalId(globalId);
            long time = Long.parseLong(leftArgs[2]);

            int count = 0;
            try {
                count = FollowRelationCacheAdapter.getInstance().getCommonFriendCountAfterTime(
                        userid, gid, time);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println(count);
            System.exit(0);
        }

        /************************************************
         * hasRelation
         ***********************************************/
        if (cmd.equals("hasRelation")) {
            if (leftArgs.length < 2) {
                System.out.println("Usage: hasRelation userid gid");
                System.exit(0);
            }

            int userid = Integer.parseInt(leftArgs[0]);
            long globalId = Long.parseLong(leftArgs[1]);
            GlobalId gid = new GlobalId(globalId);

            boolean result = false;
            try {
                result = FollowRelationCacheAdapter.getInstance().hasRelation(userid, gid);
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (result) {
                System.out.println("1");
            } else {
                System.out.println("0");
            }
            System.exit(0);
        }

        /************************************************
         * reload
         ***********************************************/
        if (cmd.equals("reload")) {
            if (leftArgs.length < 1) {
                System.out.println("Usage: reload globalId");
                System.exit(0);
            }

            long globalId = Long.parseLong(leftArgs[0]);

            try {
                FollowRelationCacheAdapter.getInstance().reload(globalId);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.exit(0);
        }

        System.exit(0);
    }
}
