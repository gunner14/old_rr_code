package com.xiaonei.xce.footprintunite;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Vector;
import java.util.Map.Entry;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.footprint.FootprintFacadeManagerPrx;
import xce.footprint.FootprintFacadeManagerPrxHelper;
import xce.footprint.FootprintInfo;
import xce.footprint.IpFootprintInfo;
import xce.footprint.HomeFootprintCachePrx;
import xce.footprint.HomeFootprintCachePrxHelper;
import xce.footprint.HomeInfo;
import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;

import Ice.ConnectTimeoutException;
import Ice.TimeoutException;

import com.xiaonei.services.FriendInput.*;
import com.xiaonei.xce.log.Oce;

public class FootprintFacadeAdapter extends ReplicatedClusterAdapter {

	private static final String ctr_endpoints = "ControllerFootprintFacade";
	private static int _interval = 120;
	private int _cluster = 10;

	public FootprintFacadeAdapter() {

		super(ctr_endpoints, _interval);
	}

	private FootprintFacadeManagerPrx getFootprintFacadeManager(int owner) {
		Ice.ObjectPrx prx0 = getProxy(owner, 300);

		// System.out.println(prx0.ice_toString());
		if (prx0 != null) {
			FootprintFacadeManagerPrx prx;
			prx = FootprintFacadeManagerPrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}

	private FootprintFacadeManagerPrx getFootprintFacadeManagerOneWay(int owner) {
		Ice.ObjectPrx prx0 = getProxy(owner, 300);

		// System.out.println(prx0.ice_toString());
		if (prx0 != null) {
			FootprintFacadeManagerPrx prx;
			prx = FootprintFacadeManagerPrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	/***
	 * 向系统中增加一条访问信息，服务端会将这条访问信息通知到HomeFootprint, VisitFootprint和FootprintView服务
	 * 里面去，除了相关业务部门，其他用户不能随便调用此接口
	 * @param fpt， 一条访问信息，包括访问者，被访问者，以及时间访问资源等信息
	 */
	public void visit(FootprintInfo fpt) {
		getFootprintFacadeManagerOneWay(fpt.owner).visit(fpt);
	}

	/***
	 * 向系统中添加一条VisitByIp的访问信息，除了相关业务部门，其他用户不能随便调用此接口
	 * @param fpt，访问信息，包括被访问者，访问者的ip, 时间， 资源等信息
	 */
	public void visitByIp(IpFootprintInfo fpt) {
		getFootprintFacadeManagerOneWay(fpt.owner).visitByIp(fpt);
	}

	/***
	 * 批量向系统中添加访问信息，除了相关业务部门，其他用户不能随便调用此接口
	 * @param fpts， 访问信息列表，每一项包括一个访问者，被访问者，时间，访问资源信息等
	 */
	public void visitBatch(ArrayList<FootprintInfo> fpts) {
		HashMap<Integer, ArrayList<FootprintInfo>> bucket = new HashMap<Integer, ArrayList<FootprintInfo>>();
		for (FootprintInfo fpt : fpts) {

			ArrayList<FootprintInfo> list = bucket.get(fpt.owner % _cluster);
			if (list == null) {
				list = new ArrayList<FootprintInfo>();
				bucket.put(fpt.owner % _cluster, list);
			}
			list.add(fpt);
		}
		for (Entry<Integer, ArrayList<FootprintInfo>> entry : bucket.entrySet()) {
			getFootprintFacadeManagerOneWay(entry.getKey()).visitBatch(
					entry.getValue().toArray(
							new FootprintInfo[entry.getValue().size()]));
		}
	}

	/***
	 * 添加一条访问信息，同时返回被访问者的最近来访信息，除了相关业务部门，其他人员不应该调用
	 * @param fpt， 一条访问信息，包括来访者，被访问者，时间，资源信息等
	 * @return 被访问者的最近来访信息，包括来访人，时间，资源信息等
	 */
	public HomeInfo visitAndGet(FootprintInfo fpt) {
		return getFootprintFacadeManager(fpt.owner).visitAndGet(fpt);
	}
	
	/***
	 * 删除一个用户访问其他用户的所有信息，除了相关业务，其他人员不应该调用
	 * @param visitor， 访问者，要删除这个用户所有访问其他人的信息
	 */
	public void removeAllByVisitor(int visitor) {
		getFootprintFacadeManager(visitor).removeAllByVisitor(visitor);
	}
}
