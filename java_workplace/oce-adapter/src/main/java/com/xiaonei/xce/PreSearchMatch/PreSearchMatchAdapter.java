package com.xiaonei.xce.PreSearchMatch;


import java.util.Random;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;

import com.renren.search.PreMatchManagerPrx;
import com.renren.search.PreMatchManagerPrxHelper;

public class PreSearchMatchAdapter extends ReplicatedClusterAdapter {
	
	private static PreSearchMatchAdapter _instance = new PreSearchMatchAdapter();
	private static final String _endpoints = "ControllerPreMatch";
	
	private static int _interval = 120;

	protected PreSearchMatchAdapter() {
		super(_endpoints, _interval, 
				//Channel.newSearchChannel(_endpoints)
				Channel.newSearchControllerChannel(_endpoints)
			);
		// TODO Auto-generated constructor stub
	}
	
	public static PreSearchMatchAdapter getInstance() {
		return _instance;
	}
	
	protected PreMatchManagerPrx getSearchMatchTwoWay(int index) {
		Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		if(prx0 != null) {
			PreMatchManagerPrx prx = PreMatchManagerPrxHelper.uncheckedCast(prx0.ice_timeout(500));
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}
	
	//slice文件中提供的接口
	public String[] searchPreMatch(int userid, String query) {  
		String[] matchStr = null;
		PreMatchManagerPrx prx = null;		
		try {
			prx = getSearchMatchTwoWay(userid);
			if(prx != null) {
				matchStr = prx.searchPreMatch(userid, query);
			} else {
				System.out.println("prx is null");
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
		return matchStr;
	} 

	/**
	 * @param args
	 */
	public static void main(String[] args)  {
		// TODO Auto-generated method stub
		
		System.setProperty("isTestSearchChannel", "true");
		try{
			String[] matchStr = null; 
			
			matchStr = PreSearchMatchAdapter.getInstance().searchPreMatch(202007480, "阿");
			if(matchStr != null) {
				System.out.println(matchStr.length);
				for(String u : matchStr) {
					System.out.println("len="+u);
				}
			} else {
				System.out.println("result is null");
			}
			
		} catch(Exception e) {
			System.out.println("catch:"+e);
		}		
	}

}
