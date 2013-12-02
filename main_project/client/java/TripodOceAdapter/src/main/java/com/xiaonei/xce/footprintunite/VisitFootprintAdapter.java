package com.xiaonei.xce.footprintunite;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.footprint.FootprintInfo;
import xce.footprint.UgcFootprintCachePrx;
import xce.footprint.UgcFootprintCachePrxHelper;
import xce.footprint.UgcInfo;

import com.sun.xml.internal.ws.transport.http.DeploymentDescriptorParser.AdapterFactory;
import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

public class VisitFootprintAdapter extends ReplicatedClusterAdapter {

	private FootprintFacadeAdapter facade;
	private static final String ctr_endpoints = "ControllerVisitFootprint";
	private static int _interval = 120;
	private int _cluster = 10;

	public VisitFootprintAdapter() {
		super(ctr_endpoints, _interval);
		facade = new FootprintFacadeAdapter();
	}
	
	public VisitFootprintResult getAll(int owner) {
		return getAll(owner, 0, 6, 0);
	}

	public VisitFootprintResult getAll(int owner, int begin, int limit) {
		return getAll(owner, begin, limit, 0);
	}

	public VisitFootprintResult getAll(int owner, int begin, int nLimit,
			int tLimit) {
		VisitFootprintResult res = null;
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(owner);
			UgcInfo info = prx.getAll(owner, begin,
					nLimit, tLimit);
			res = new VisitFootprintResult(owner, info.steps);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner, e);
		}

		return res;
	}

	public int getSize(int owner) {
		int size = 0;
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(owner);
			size = prx.getSize(owner);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx.toString() + "]" + owner, e);
		}
		return size;
	}

	public void remove(FootprintView fpt) {
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(fpt.getOwner());
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

	private void visit(FootprintView fpt) {
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(fpt.getOwner());
			prx.visit(fpt.getFootprint());
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + prx.toString() + "]" + fpt.getOwner()
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + prx.toString() + "]" + fpt.getOwner(), e);
		}
	}

	private void visitBatch(ArrayList<FootprintView> fpts) {
		UgcFootprintCachePrx prx = null;
		try {
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
				prx = getVisitFootprintManager(entry.getKey(), true);
				prx.visitBatch(
						entry.getValue().toArray(
								new FootprintInfo[entry.getValue().size()]));
			}

		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + prx.toString() + "]"
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + prx.toString() + "]" + e);
		}
	}

	private UgcFootprintCachePrx getVisitFootprintManager(int owner) {
		return getVisitFootprintManager(owner, false);
	}

	private UgcFootprintCachePrx getVisitFootprintManager(int owner,
			boolean isOneWay) {
//		Ice.ObjectPrx prx0 = locateProxy(_managers, "UFC", owner,
//				Channel.Invoke.Twoway, UgcFootprintCachePrxHelper.class, 300);
		Ice.ObjectPrx prx0 = getProxy(owner, 300);

		// System.out.println(prx0.ice_toString());
		if (prx0 != null) {
			UgcFootprintCachePrx prx;
			if (!isOneWay) {
				prx = UgcFootprintCachePrxHelper.uncheckedCast(prx0
						.ice_timeout(300).ice_twoway());
			} else {
				prx = UgcFootprintCachePrxHelper.uncheckedCast(prx0
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
	}
}
