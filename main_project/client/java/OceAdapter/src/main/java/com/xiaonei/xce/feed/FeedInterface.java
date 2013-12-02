package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;

import java.util.Map;
import java.util.HashMap;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedDataRes;
import xce.feed.FeedContent;
import xce.feed.FeedInterfacePrx;
import xce.feed.FeedInterfacePrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class FeedInterfaceHaAdapter extends  ReplicatedClusterAdapter{

	private static FeedInterfaceHaAdapter instance;
	private static final String ctr_endpoints = "ControllerFeedInterfaceR";
	private static int _interval = 1;

	public static FeedInterfaceHaAdapter getInstance() {
		if (instance == null) {
			synchronized (FeedInterfaceHaAdapter.class) {
				if (instance == null) 
					instance = new FeedInterfaceHaAdapter();
			}
		}
		return instance;
	}

	private FeedInterfaceHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerFeedInterfaceR"));

	}

	private FeedInterfacePrx getManager(long feedid) {

		Ice.ObjectPrx prx0 = getValidProxy((int)feedid, 180);
		//System.out.println("prx:" + prx0);
		if (prx0 != null) {
			FeedInterfacePrx prx = FeedInterfacePrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedInterface ha prx is null !");
			return null;
		}
	}

	public HashMap<Long, FeedContent> GetFeedDict(long[] feedids) {
		FeedInterfacePrx prx = null;
		HashMap<Long, FeedContent> result = new HashMap<Long, FeedContent> ();
		ArrayList<Long> feedid_batch = new ArrayList<Long>();

		for(int i=0; i<feedids.length; ++i) {
			feedid_batch.add(feedids[i]);
			if (feedid_batch.size() >= 100 || i >=feedids.length-1) {
				try {
					prx = getManager(feedids[i]);
					long[] temp_batch ;
					temp_batch = new long[feedid_batch.size()];
					for(int j=0; j<feedid_batch.size(); ++j) {
						temp_batch[j] = feedid_batch.get(j).longValue();
					}
					//Map<Long, FeedContent> temp = prx.GetFeedDict( feedid_batch.toArray( ) );
					Map<Long, FeedContent> temp_result = prx.GetFeedDict( temp_batch );
					result.putAll(temp_result);
				} catch(Exception e) {
					String err = this.getClass().getName() + "GetFeedDict [" + prx + "] " ;
					Oce.getLogger().error(err + " " + e);
					throw new RuntimeException(err + " " + e, e);
				}
				feedid_batch.clear();
			}

		}

		return result;
	}

//
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//	}
//      
}
