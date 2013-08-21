package com.xiaonei.xce.feed;



import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedDataRes;
import xce.feed.FeedItem;
import xce.feed.FeedNaiveBayesPrx;
//import xce.feed.FeedNewsManagerPrx;
import xce.feed.FeedNaiveBayesPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

public class FeedNaiveBayesHaAdapter extends ReplicatedClusterAdapter {

	private static FeedNaiveBayesHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedNaiveBayesR";

	private static int _interval = 1;
	
	
	public static FeedNaiveBayesHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedNaiveBayesHaAdapter();
		}
		return instance;
	}

	private FeedNaiveBayesHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedChannel("ControllerFeedNaiveBayesR"));
		
	}
	

	private FeedNaiveBayesPrx getManagerOneway(int uid) {
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedNaiveBayesPrx prx = FeedNaiveBayesPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedNaiveBayes ha one way prx is null ! ");
			return null;
		}
	}

	private FeedNaiveBayesPrx getManager(int uid) {	
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedNaiveBayesPrx prx = FeedNaiveBayesPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedNaiveBayes ha prx is null ! ");
			return null;
		}
	}
	
	//map<int,int>  FeedNaiveBayesI::GetScoreDict(int fromid, const vector<int> & toids, int stype, const Ice::Current& )
	
	public Map< Integer, Integer> GetScoreDict(int fromid, int[] toids, int stype){
		Map< Integer, Integer> map = null;
		FeedNaiveBayesPrx prx = null;
		
		try {
			prx = getManager(fromid);
			map = prx.GetScoreDict(fromid, toids, stype);
		} catch(Exception e) {
			String err = this.getClass().getName() + "GetScoreDict [" + prx + "] userid=" + fromid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return map;
	}
	
	public Map< Integer, Integer> GetScoreDictReverse(int to, int[] fromids, int stype){
		Map< Integer, Integer> map = null;
		FeedNaiveBayesPrx prx = null;
		
		try {
			prx = getManager(to);
			map = prx.GetScoreDictReverse(to, fromids, stype);
		} catch(Exception e) {
			String err = this.getClass().getName() + "GetScoreDictReverse [" + prx + "] userid=" + to;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return map;
	}

//	public int GetSendFrequency(int fromid) {
//		FeedNaiveBayesPrx prx = null;
//		int res;	
//		try {
//			prx = getManager(fromid);
//			res = prx.GetSendFrequency(fromid);
//		} catch(Exception e) {
//			String err = this.getClass().getName() + "GetSendFrenquency [" + prx + "] userid=" + fromid;
//			Oce.getLogger().error(err + " " + e);
//			throw new RuntimeException(err + " " + e, e);
//		} 
//		return res;
//	 }
	
	public static void main(String[] args) {
		int stype = 502;
		int fromid = 263734453;
		int[] toids = {263734454, 258800330, 268153415, 313164530, 260741408,272395846,280274310,1,231,-1};
		Map< Integer, Integer > res = FeedNaiveBayesHaAdapter.getInstance().GetScoreDict(fromid, toids, stype);
		for (Map.Entry< Integer, Integer > entry : res.entrySet()) {
			System.out.println("id:" + entry.getKey() + ", score="+ entry.getValue());
		}
		
		res = FeedNaiveBayesHaAdapter.getInstance().GetScoreDictReverse(fromid, toids, stype);
		for (Map.Entry< Integer, Integer > entry : res.entrySet()) {
			System.out.println("rever id:" + entry.getKey() + ", score="+ entry.getValue());
		}
	}
	
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//		List<FeedData> res = FeedNaiveBayesHaAdapter.getInstance().getHotFeed(225092473, 
//   			0, 20);
//		FeedData fd = res.get(0);
//		System.out.println("fd:" + fd.feed + " size:" + res.size());
//	}
			
			
}
