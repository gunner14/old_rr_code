package com.xiaonei.xce.footprintunite;

import java.sql.Date;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.model.FootprintNewView;
import mop.hi.oce.domain.model.FootprintResult;

import com.xiaonei.xce.globalid.GlobalId;

import xce.footprint.FootprintFacadeManagerPrx;
import xce.footprint.FootprintFacadeManagerPrxHelper;
import xce.footprint.FootprintInfo;
import xce.footprint.HomeFootprintCachePrx;
import xce.footprint.HomeFootprintCachePrxHelper;
import xce.footprint.HomeInfo;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;

import com.xiaonei.xce.footprint.FootprintNewAdapter;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;

import com.xiaonei.xce.log.Oce;

public class FootprintUniteAdapter extends ReplicatedClusterAdapter {

	private Channel channel;
	private FootprintFacadeAdapter facade;
	private static final String ctr_endpoints = "ControllerFootprintUnite";
	private static int _interval = 120;
	private int _cluster = 10;
	private Object ownerId;

	/***
	 * 默认构造函数，构造一个FootprintUniteAdapter的实例
	 */
	public FootprintUniteAdapter() {
		super(ctr_endpoints, _interval);
		facade = new FootprintFacadeAdapter();
	}

	/***
	 * 获取owner用户的6个最近来访信息，不限时间
	 * @param owner， 需要获取最近来访信息的用户id
	 * @return 用户的最近来访信息，包括人气值，最近来访的用户id, 时间，访问的资源
	 */
	public HomeFootprintResult getAll(int owner) {
		return getAll(owner, 0, 6, 0);
	}

	/***
	 * 获取owner用户的最多limit个最近来访信息，开始的index是begin, 最近来访的用户从0开始编号，来访时间越晚的
	 * index值越小
	 * @param owner， 需要获取最近来访信息的用户id
	 * @param begin,  开始的index
	 * @param limit,  从begin开始最多获取多少个最近来访者的信息
	 * @return 从begin开始最多limit个最近来访信息，包括人气值，最近来访的用户id, 时间，访问的资源
	 */
	public HomeFootprintResult getAll(int owner, int begin, int limit) {
		return getAll(owner, begin, limit, 0);
	}

	/***
	 * 获取owner用户的最多nlimit个最近来访信息，开始的index是begin, 最近来访的用户从0开始编号，来访时间越晚的
   * index值越小。来访时间必须大于tlimit
	 * @param owner， 需要获取最近来访信息的用户id
	 * @param begin,  开始的index
	 * @param nLimit, 从begin开始最多获取多少个最近来访者的信息
	 * @param tLimit, 来访时间必须大于tlimit
	 * @return 从begin开始，来访时间大于tlimit的最多nlimit个最近来访信息，包括人气值，最近来访的用户id, 
	 * 时间，访问的资源
	 */
	public HomeFootprintResult getAll(int owner, int begin, int nLimit,
			int tLimit) {

		HomeFootprintResult res = null;
		HomeFootprintCachePrx prx = null;
		try {
			prx = getHomeFootprintManager(owner);
			HomeInfo info = prx.getAll(owner, begin,
					nLimit, tLimit);
			res = new HomeFootprintResult(owner, info.steps, info.visitcount);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			//Oce.getLogger().error(
		//			this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner, e);
		}

		return res;
	}

	/***
	 * 获取系统中保存有多少个owner用户的最近来访信息，因为考虑到存储空间以及效率因素，系统中对每一个用户的最近来访
	 * 信息的数目是有限制的
	 * @param owner， 需要获取系统中保存的最近来访数目的用户id
	 * @return 系统中保存了多少个owner用户的最近来访信息
	 */
	public int getSize(int owner) {
		int size = 0;
		HomeFootprintCachePrx prx = null;
		try {
			prx = getHomeFootprintManager(owner);
			size = prx.getSize(owner);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx.toString() + "]" + owner, e);
		}
		return size;
	}

	/***
	 * 获取用户的人气信息
	 * @param owner， 需要获取人气信息的用户id
	 * @return 该用户的人气信息
	 */
	public int getUserVisitCount(int owner) {
		HomeFootprintCachePrx prx = null;
		try {
			prx = getHomeFootprintManager(owner);
			return prx.getUserVisitCount(owner);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getUserVisitCount [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getUserVisitCount [" + prx.toString() + "]" + owner,
					e);
		}
		return 0;
	}

	/***
	 * 设置用户的人气信息， 这个函数不能随便调用，因为用户人气值的设置必须是递增的，还有一些条件的限制，慎用
	 * @param owner， 需要设置人气信息的用户
	 * @param count， 人气信息值
	 */
	public void setUserVisitCount(int owner, int count) {
		HomeFootprintCachePrx prx = null; 
		try {
			prx = getHomeFootprintManager(owner);
			prx.setUserVisitCount(owner, count);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setUserVisitCount [" + prx.toString() + "]"  + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setUserVisitCount [" + prx.toString() + "]"  + owner,
					e);
		}
	}

	/***
	 * 删除系统中的一条访问信息， 除了相关业务部门，其他人员不应该调用
	 * @param fpt， 需要删除的访问信息，包括来访者，被访问者，时间等信息
	 */
	public void remove(FootprintView fpt) {
		HomeFootprintCachePrx prx = null;
		try {
			prx = getHomeFootprintManager(fpt.getOwner());
			// System.out.println("visitor:"+fpt.getVisitor()
			// +" owner: "+fpt.getOwner());
			prx.remove(fpt.getFootprint());
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".remove [" + prx.toString() + "]" + fpt.getOwner()
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".remove [" + prx.toString() + "]" + fpt.getOwner(), e);
		}
	}

	/***
	 * 向系统中增加一条访问信息， 除了相关业务部门，其他人员不应该调用
	 * @param fpt， 需要增加的访问信息， 包括来访者，被访问者，时间等信息
	 */
	public void visit(FootprintView fpt) {

		try {
			if (facade != null) {
				facade.visit(fpt.getFootprint());
			} else {
				HomeFootprintCachePrx prx=null;
				try{
					prx=getHomeFootprintManager(fpt.getOwner());
					prx.visit(fpt.getFootprint());
				}catch (Ice.TimeoutException e)
				{
					Oce.getLogger().error(
							this.getClass().getName() + ".visit [" + prx + "] id+ "  + fpt.getOwner()
									+ " Ice.TimeoutException");
				}
			}
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + facade.toString() + "]"  + fpt.getOwner()
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + facade.toString() + "]"  + fpt.getOwner(), e);
		}
	}

	/***
	 * 批量增加访问信息， 除了相关业务部门，其他人员不应该调用
	 * @param fpts， 访问信息列表，每一项包括访问者，被访问者，时间等信息
	 */
	public void visitBatch(ArrayList<FootprintView> fpts) {

		try {
			if (facade != null) {
				ArrayList<FootprintInfo> list = new ArrayList<FootprintInfo>(
						fpts.size());
				for (FootprintView view : fpts) {
					list.add(view.getFootprint());
				}
				facade.visitBatch(list);
			} else {
				HashMap<Integer, ArrayList<FootprintInfo>> bucket = new HashMap<Integer, ArrayList<FootprintInfo>>();
				for (FootprintView fpt : fpts) {

					ArrayList<FootprintInfo> list = bucket.get(fpt.getOwner()
							% _cluster);
					if (list == null) {
						list = new ArrayList<FootprintInfo>();
					}
					list.add(fpt.getFootprint());
				}
				for (Entry<Integer, ArrayList<FootprintInfo>> entry : bucket
						.entrySet()) {
					HomeFootprintCachePrx prx=null;
					try{
						prx=getHomeFootprintManager(entry.getKey(), true);
						prx.visitBatch(entry.getValue().toArray(
												new FootprintInfo[entry.getValue()
														.size()]));
					}catch (Ice.TimeoutException e)
					{
						Oce.getLogger().error(
								this.getClass().getName() + ".visitBatch [" + prx + "] id+ "  + entry.getKey()
										+ " Ice.TimeoutException");
					}
				}

			}
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + facade.toString() + "]"
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + facade.toString() + "]" + e);
		}
	}

	/***
	 * 向系统中增加一条访问信息，并返回被访问者的最近来访信息， 除了相关业务部门，其他人员不应该调用
	 * @param fpt，包括访问者，被访问者，时间等信息
	 * @return 被访问者的最近来访信息
	 */
	public HomeFootprintResult visitAndGet(FootprintView fpt) {

		HomeFootprintResult res = null;

		HomeInfo info = null;
		try {
			if (facade != null) {
				info = facade.visitAndGet(fpt.getFootprint());
			} else {
				HomeFootprintCachePrx prx=null;
				try{
					prx=getHomeFootprintManager(fpt.getOwner());
					info = prx.visitAndGet(
						fpt.getFootprint(), 0, 9, 0);
				}catch (Ice.TimeoutException e)
				{
					Oce.getLogger().error(
							this.getClass().getName() + ".visitAndGet [" + prx + "] id+ "  + fpt.getOwner()
									+ " Ice.TimeoutException");
				}
			}
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitAndGet [" + facade.toString() + "]"
							+ fpt.getOwner() + " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitAndGet [" + facade.toString() + "]"
							+ fpt.getOwner(), e);
		}
		if (info != null)
			res = new HomeFootprintResult(fpt.getOwner(), info.steps,
					info.visitcount);
		return res;
	}

	public HomeFootprintCachePrx getHomeFootprintManager(int owner) {
		return getHomeFootprintManager(owner, false);
	}

	public HomeFootprintCachePrx getHomeFootprintManager(int owner,
			boolean isOneWay) {

		Ice.ObjectPrx prx0 = getProxy(owner, 300);

		// System.out.println(prx0.ice_toString());
		if (prx0 != null) {
			HomeFootprintCachePrx prx;
			if (!isOneWay) {
				prx = HomeFootprintCachePrxHelper.uncheckedCast(prx0
						.ice_timeout(300).ice_twoway());
			} else {
				prx = HomeFootprintCachePrxHelper.uncheckedCast(prx0
						.ice_timeout(300).ice_oneway());
			}
			return prx;
		} else {
			return null;
		}
	}

	/*
	 * public void reloadCache(int host) {public FootprintResult
	 * visitHomepageAndGet(FootprintNewView fpt, boolean friendflag, String
	 * ref); int mod = host % _cluster; int category = 0; ObjectCachePrx objPrx
	 * = channel.locate("SC", "@FootprintN"+mod, Channel.Invoke.Twoway,
	 * ObjectCachePrxHelper.class, 300); objPrx.reloadObject(category, host); }
	 */

	public static void main(String[] args) {

		
		int hostId = 0;
		int owner=0;
		int begin=0;
		int limit = 0;
		FootprintUniteAdapter test = AdapterFactory.getHomeFootprintAdapter();
		test.setUserVisitCount(261702304, 3405);
		System.exit(0);
		int ugcid;
		int visitor = 0;
		//int owner =223500512;
		// test.getUserVisitCount(owner);
		// int timestamp = (int) (System.currentTimeMillis() / 1000L);
		GlobalId gid = GlobalId.fromAlbum(1008);
		// // GlobalId gid = GlobalId.fromBlog(ugcId);
		// // GlobalId gid = GlobalId.fromAlbum(ugcId);
		//
		// FootprintView fpt = new FootprintView(owner, visitor, timestamp,
		// gid);
		// test.visit(fpt);
		int ownerId = 282097083;
		// int guestId = 10; // 特别好友
		//
//		System.out.println("VisitCount of " + ownerId + " : "
//				+ test.getUserVisitCount(ownerId));
		// 设置人气(慎用！！)
		// test.setUserVisitCount(ownerId, 9999);
		// System.exit(0);

		// 取人气
		// System.out.println("VisitCount of " + ownerId + " : "
		// + test.getUserVisitCount(ownerId));
		// test.setUserVisitCount(ownerId, 9000);
		// System.out.println("VisitCount of " + ownerId + " : "
		// + test.getUserVisitCount(ownerId));
		// System.exit(0);
		// 莫文蔚, 蔡健雅, SHE, 游鸿明, 周迅, 张信哲, 光良, 陈奕迅, 李宇春
		//int[] guests = new int[] { 600002664, 600002252, 600002300, 600002216,
		//		600002221, 600002349, 600002233, 600002342, 600002235 };
		int []guests = new int []{281703832};
		ArrayList<FootprintInfo> fpts = new ArrayList<FootprintInfo>();
		for (int guest : guests) {
			FootprintInfo fpt = new FootprintInfo();
			fpt.owner = ownerId;
			fpt.gid = ownerId;
			fpt.timestamp = (int) (System.currentTimeMillis() / 1000L);
			fpt.visitor = guest;
			FootprintView view = new FootprintView(fpt.owner, fpt.visitor,
					(int) (System.currentTimeMillis() / 1000L), GlobalId
							.fromBlog(468823778));
			test.visit(view);
			//fpts.add(fpt);
			//facade.visitBatch(fpts);
			// test.remove(view);
		}
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// test.remove(view);
		// System.out.println(fpt.visitor);
		// break;
		// test.visitHomepage(fpnv, false, "UNKNOWN");
		// result = test.visitHomepageAndGet(fpnv, false, "UNKNOWN");

		// System.exit(0);
		//
//		System.out.println("VisitCount of " + ownerId + " : "
//				+ test.getUserVisitCount(ownerId));

		// 取脚印
		// HomeFootprintResult result = test.getAll(ownerId, 0, 96);
		// System.out.println("Visit Count of " + ownerId + " is :"
		// + result.visitcount);

		System.out.println(test.getSize(ownerId) + " footprint(s)\n[");
		HomeFootprintResult result = test.getAll(ownerId, 0, 96);
		System.out.println(result.getVisitCount());
		for (FootprintView view : result.getFootprintViews()) {
			System.out.println(view.getVisitor() + " " + view.getOwner() + " "
					+ view.getTime());
		}
		System.out.println("]\n");
	}
}
