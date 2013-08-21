
package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedDataByte;
import xce.feed.FeedDataRes;
import xce.feed.FeedDataByteRes;
import xce.feed.FeedViewPrx;
import xce.feed.FeedViewPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

public class FeedViewForTimelineHaAdapter extends ReplicatedClusterAdapter {

	private static FeedViewForTimelineHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedViewR";

	private static int _interval = 1;
	
	
	public static FeedViewForTimelineHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedViewForTimelineHaAdapter();
		}
		return instance;
	}

	private FeedViewForTimelineHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedChannel("ControllerFeedViewR"));
		
	}
	


	private FeedViewPrx getManagerOneway(int uid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedViewPrx prx = FeedViewPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedView ha one way prx is null !");
			return null;
		}
	}

	private FeedViewPrx getManager(int uid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		//System.out.println("FeedView prx:" + prx0);
		if (prx0 != null) {
			FeedViewPrx prx = FeedViewPrxHelper.uncheckedCast(prx0.ice_timeout(400).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedView ha prx is null !");
			return null;
		}
	}

        public Map<Integer, List<Integer> > getTimelineYearMonth(int uid) {
                Map<Integer, int[]> uidYearMonth;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        uidYearMonth = prx.GetTimelineYearMonth(uid);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineYearMonth [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                Map<Integer, List<Integer> > res = new HashMap<Integer, List<Integer> >();
                for(Map.Entry<Integer, int[]> item : uidYearMonth.entrySet()) {
                        List<Integer> monthList = new ArrayList<Integer>();
                        int[] months = item.getValue();
                        for(int i = 0; i < months.length; ++i) {
                                monthList.add(months[i]);
                        }
                        res.put(item.getKey(), monthList);
                }
                return res;
         
        }

        public List<String> getTimelineData(int ownerid, int uid, String name, int year, int month,
                        int render, int begin, int limit) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetTimelineData(ownerid, uid, name, year, month, render, begin, limit);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineData [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }
        public List<String> getTimelineDataWithAdmin(int ownerid, int uid, String name, int year, int month,
                        int render, int begin, int limit, boolean isAdmin) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetTimelineDataWithAdmin(ownerid, uid, name, year, month, render, begin, limit, isAdmin);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineData [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }

	public List<String> getBySourceStypeAndActor(int uid, String name, long source, int stype, int actor, int render, int view) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetBySourceStypeAndActor(uid, name, source, stype, actor, render, view);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getBySourceStypeAndActor [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e); 
                        throw new RuntimeException(err + " " + e, e); 
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }
}
