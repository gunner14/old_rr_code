package com.xiaonei.xce.footprintunite;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.footprint.FootprintInfo;
import xce.footprint.HomeFootprintCachePrx;
import xce.footprint.HomeFootprintCachePrxHelper;
import xce.footprint.HomeInfo;
import xce.util.channel.Channel;

import com.sun.xml.internal.ws.transport.http.DeploymentDescriptorParser.AdapterFactory;
import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

public class FootprintUniteAdapter extends ReplicatedClusterAdapter {

  private FootprintTripodClient footprintTripodClient_ = FootprintTripodClient.getInstance();
	private Channel channel;
	private FootprintFacadeAdapter facade;
	private static final String ctr_endpoints = "ControllerFootprintUnite";
	private static int _interval = 120;
	private int _cluster = 10;
	private Object ownerId;

	public FootprintUniteAdapter() {
		super(ctr_endpoints, _interval);
		facade = new FootprintFacadeAdapter();
	}

	public HomeFootprintResult getAll(int owner) {
		return getAll(owner, 0, 6, 0);
	}

	public HomeFootprintResult getAll(int owner, int begin, int limit) {
		return getAll(owner, begin, limit, 0);
	}

	public HomeFootprintResult getAll(int owner, int begin, int nLimit,
			int tLimit) {
	  return footprintTripodClient_.getAll(owner, begin, nLimit, tLimit);
	}

	public int getSize(int owner) {
	  return footprintTripodClient_.getAll(owner, 0, FootprintTripodClient.ALL_FOOTPRINT_SIZE, 0).
	    getFootprintViews().size();
	}

	public int getUserVisitCount(int owner) {
	  return footprintTripodClient_.getUserViewCount(owner);
	}

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

	private void printHomeFootprintResult(int userId, final HomeFootprintResult result) {
	  System.out.println("----------------------------------------------------------------------");
	  System.out.println("Owner = " + userId + ", ViewCount = " + result.getVisitCount() + ", footprints.size = "
	      + result.getFootprintViews().size());
	  for (FootprintView view : result.getFootprintViews()) {
	    System.out.println("Owner = " + view.getOwner() + ", Visitor = " + view.getVisitor()
	        + ", Time = " + view.getTime() + ", GlobalId = " + view.getSource());
	  }
	}
	public static void main(String[] args) {
	  FootprintUniteAdapter adapter = new FootprintUniteAdapter();
	  System.out.println(adapter.getUserVisitCount(100460655));
//	  HomeFootprintResult footprintResult = adapter.getAll(334694606);
//	  adapter.printHomeFootprintResult(334694606, footprintResult);
//	  adapter.printHomeFootprintResult(334694606, adapter.getAll(334694606, 0, 48));
//	  adapter.printHomeFootprintResult(334694606, adapter.getAll(334694606, 3, 8));
//	  adapter.printHomeFootprintResult(334694606, adapter.getAll(334694606, 10, 12));
	  System.exit(0);
	}
}
