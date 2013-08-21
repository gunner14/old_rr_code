package com.xiaonei.xce.footprintview;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.footprint.FootprintViewManagerPrx;
import xce.footprint.FootprintViewManagerPrxHelper;
import xce.footprint.VisitCountInfo;

public class FootprintViewAdapter extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerFootprintView";

    private static int _interval = 120;

    private int _cluster = 10;

    /***
     * 默认构造函数
     */
    public FootprintViewAdapter() {

        super(ctr_endpoints, _interval);
    }

    private FootprintViewManagerPrx getFootprintViewManager(int owner) {
        Ice.ObjectPrx prx0 = getProxy(owner, 300);

        if (prx0 != null) {
            FootprintViewManagerPrx prx;
            prx = FootprintViewManagerPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_twoway());
            return prx;
        } else {
            return null;
        }
    }

    private FootprintViewManagerPrx getFootprintViewManagerOneWay(int owner) {
        Ice.ObjectPrx prx0 = getProxy(owner, 300);

        if (prx0 != null) {
            FootprintViewManagerPrx prx;
            prx = FootprintViewManagerPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_oneway());
            return prx;
        } else {
            return null;
        }
    }

    /***
     * 获取一个用户最近一段时间的人气增长历史
     * @param owner， 需要获取最近一段时间人气增长历史的用户id
     * @return List<VisitCountView>,用户最近一段时间人气增长的历史，每一项是一个时间，一个人气值，表示用户人气在那个
     * 时间点的时候的人气值
     */
    public List<VisitCountView> getVisitCountHistory(int owner) {
        try {
            List<VisitCountView> return_value = new ArrayList<VisitCountView>();
            List<VisitCountInfo> history = getFootprintViewManager(owner).getVisitCountHistory(
                    owner);

            for (VisitCountInfo info : history) {
                return_value.add(new VisitCountView(info));
            }
            return return_value;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<VisitCountView>();
    }

    /***
     * 获取用户最近一次人气的信息
     * @param owner， 需要获取人气信息的用户id
     * @return VisitCountView, 包括人气信息，时间，以及state
     */
    public VisitCountView getRecentVisitCount(int owner) {
        try {
            return new VisitCountView(getFootprintViewManager(owner).getRecentVisitCount(owner));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new VisitCountView(new VisitCountInfo());
    }

    /***
     * 将用户最近一次人气信息的state设置为STATELIMIT, 这个函数现在对外界来说没有什么用，大家可以忽略
     * @param owner, 需要设置人气信息的用户id
     */
    public void setVisitCountLimit(int owner) {
        try {
            getFootprintViewManager(owner).setVisitCountLimit(owner);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /***
     * 设置用户最近一次人气信息的state， 这个函数对外界也没有什么用，大家忽略即可
     * @param owner， 需要设置state的用户id
     * @param state, 需要设置成的state值
     */
    public void setVisitCountState(int owner, int state) {
        try {
            getFootprintViewManager(owner).setVisitCountState(owner, state);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /***
     * 设置一些用户当前的人气信息
     * @param counts， Map中的每一项表示一个用户id和人气信息<userId, count>
     * @return 是否全部设置成功，因为设置人气的时候，一定要比之前的人气值大，否则认为人气不合法
     */
    public boolean verifyVisitCount(Map<Integer, Integer> counts) {
        Map<Integer, Map<Integer, Integer>> count_map = new HashMap<Integer, Map<Integer, Integer>>();
        for (int i = 0; i < this._cluster; i++) {
            count_map.put(i, new HashMap<Integer, Integer>());
        }

        for (Entry<Integer, Integer> en : counts.entrySet()) {
            int index = en.getKey() % _cluster;
            count_map.get(index).put(en.getKey(), en.getValue());
        }

        boolean return_value = true;
        for (int i = 0; i < count_map.size(); i++) {
            try {
                if (!count_map.get(i).isEmpty()) {
                    boolean value = getFootprintViewManager(i).verifyVisitCount(count_map.get(i));
                    if (!value) {
                        return_value = false;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return return_value;
    }
}
