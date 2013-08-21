package com.xiaonei.xce.buddyrelationcache;

import java.awt.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.Iterator;
import mop.hi.oce.domain.buddy.BuddyRelation;
import mop.hi.oce.domain.buddy.CheckedBuddyRelation;
import xce.buddy.BuddyDesc;
import xce.buddy.Relationship;

import xce.buddy.RelationshipEntry;

import xce.buddy.BuddyRelationCacheManagerPrx;
import xce.buddy.BuddyRelationCacheManagerPrxHelper;
import xce.buddy.BuddyRelationLoaderPrx;
import xce.buddy.BuddyRelationLoaderPrxHelper;

import Ice.ObjectPrx;

import com.xiaonei.xce.buddybyidcache.BuddyByIdCacheAdapter;
import com.xiaonei.xce.log.Oce;
import com.xiaonei.xce.log.TimeCost;
import xce.clusterstate.ReplicatedClusterAdapter;

public class BuddyRelationCacheAdapter extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerBuddyRelationCache";

    private static int _interval = 120;

    public BuddyRelationCacheAdapter() {
	super(ctr_endpoints, _interval);
    }

    // 避免重复连接，应用Singleton模式
    private static BuddyRelationCacheAdapter _instance = new BuddyRelationCacheAdapter();

    public static BuddyRelationCacheAdapter getInstance() {
	return _instance;
    }

    public BuddyRelation getRelation(int fromId, int toId) {
	com.xiaonei.xce.domain.buddy.BuddyRelation br = com.xiaonei.xce.buddyadapter.buddyrelationcache.BuddyRelationCacheAdapter
		.getInstance().getRelation(fromId, toId);
	if (br.isApplied()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.Applied));
	} else if (br.isApply()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.Apply));
	} else if (br.isBlock() && br.isBlocked()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.BiBlock));
	} else if (br.isBlock()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.Block));
	} else if (br.isBlocked()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.Blocked));
	} else if (br.isFriend()) {
	    return new BuddyRelation(fromId, toId,
		    BuddyRelation.parseBuddyDesc(BuddyDesc.Friend));
	}
	return new BuddyRelation(fromId, toId,
		BuddyRelation.parseBuddyDesc(BuddyDesc.NoPath));
    }

    public CheckedBuddyRelation getCheckedRelation(int fromId, int toId) {
	BuddyRelationCacheManagerPrx prx = null;
	try {
	    prx = getBuddyRelationCacheManagerTwoway(fromId);
	} catch (Ice.ConnectTimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelation ["
			    + prx.toString() + "]" + " fromId: " + fromId
			    + "toId" + toId + " Ice.ConnectTimeoutException");
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelation ["
			    + prx.toString() + "]" + " fromId: " + fromId
			    + "toId" + toId + " Ice.TimeoutException");
	} finally {
	    // stat.endFinally();
	}
	return getRelation(fromId, toId, prx);
    }

    private CheckedBuddyRelation getRelation(int fromId, int toId, ObjectPrx prx) {
	// TimeCost stat =
	// TimeCost.begin("[BuddyRelationCache::getRelation] relation[" + fromId
	// + ":" + toId + "]");
	Relationship relation = new Relationship();
	relation.from = fromId;
	relation.to = toId;
	xce.buddy.BuddyDesc desc = xce.buddy.BuddyDesc.NoPath;
	boolean sucess = false;
	try {
	    desc = BuddyRelationCacheManagerPrxHelper.uncheckedCast(prx)
		    .getRelation(relation);
	    sucess = true;
	} catch (Ice.ConnectTimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelation ["
			    + prx.toString() + "]" + " fromId: " + fromId
			    + "toId" + toId + " Ice.ConnectTimeoutException");
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelation ["
			    + prx.toString() + "]" + " fromId: " + fromId
			    + "toId" + toId + " Ice.TimeoutException");
	} finally {
	    // stat.endFinally();
	}

	BuddyRelation r = new BuddyRelation(relation.from, relation.to,
		mop.hi.oce.domain.buddy.BuddyRelation.parseBuddyDesc(desc));
	return new CheckedBuddyRelation(r, sucess);
    }

    public ArrayList<BuddyRelation> getRelationBatch(Map<Integer, Integer> ids) {
	ArrayList<BuddyRelation> list = new ArrayList<BuddyRelation>();
	BuddyRelationCacheManagerPrx prx = null;
	try {
	    prx = getBuddyRelationCacheManagerTwoway(0);
	    RelationshipEntry[] ret = prx.getRelationBatch(ids);
	    for (RelationshipEntry one : ret) {
		BuddyRelation obj = new BuddyRelation(one.relation.from,
			one.relation.to,
			mop.hi.oce.domain.buddy.BuddyRelation
				.parseBuddyDesc(one.desc));
		list.add(obj);
	    }

	} catch (Ice.ConnectTimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelationBatch ["
			    + prx.toString() + "]" + "ids.size " + ids.size()
			    + " Ice.ConnectTimeoutException");
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".getRelationBatch ["
			    + prx.toString() + "]" + " ids.size " + ids.size()
			    + " Ice.TimeoutException");
	} finally {
	}
	return list;
    }

    public boolean checkRelation(int fromId, int toId) {
	ArrayList<ObjectPrx> prxs = getAllProxy(fromId);
	if (prxs.size() == 0) {
	    System.out.println("no prx");
	    return true;
	}
	CheckedBuddyRelation firstRelation = getRelation(fromId, toId,
		prxs.get(0));
	if (firstRelation == null) {
	    System.out.println("first prx == null");
	    return false;
	}
	for (int i = 1; i < prxs.size(); ++i) {
	    CheckedBuddyRelation r = getRelation(fromId, toId, prxs.get(i));
	    if (!firstRelation.equals(r)) {
		return false;
	    }
	    System.out.println("check ok " + i);
	}
	return true;
    }

    public void setRelation(BuddyRelation relation) {
	TimeCost stat = TimeCost
		.begin("[BuddyRelationCache::setRelation] relation["
			+ relation.fromId + ":" + relation.toId + "]");
	try {
	    BuddyRelationLoaderAdapter.getInstance().setRelation(relation);
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".setRelation relation["
			    + relation.fromId + ":" + relation.toId
			    + " Ice.TimeoutException");
	} finally {
	    stat.endFinally();
	}
    }

    public void reload(int userId) {
	BuddyRelationLoaderAdapter.getInstance().reload(userId);
    }

    protected BuddyRelationCacheManagerPrx getBuddyRelationCacheManagerTwoway(
	    int userId) {
	Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	if (prx0 != null) {
	    BuddyRelationCacheManagerPrx prx = BuddyRelationCacheManagerPrxHelper
		    .uncheckedCast(prx0.ice_timeout(300).ice_twoway());
	    return prx;
	} else {
	    return null;
	}
    }

    protected BuddyRelationCacheManagerPrx getBuddyRelationCacheManagerOneway(
	    int userId) {
	Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	if (prx0 != null) {
	    BuddyRelationCacheManagerPrx prx = BuddyRelationCacheManagerPrxHelper
		    .uncheckedCast(prx0.ice_timeout(300).ice_oneway());
	    return prx;
	} else {
	    return null;
	}
    }
    
//    public static void main(String[] args) {
//	int fromId = 437960975;
//	int toId = 454604294;
//	BuddyRelation br = BuddyRelationCacheAdapter.getInstance().getRelation(fromId, toId);
//	System.out.println(br.isBlock());
//	System.out.println(br.isBlocked());
//	System.exit(0);
//    }
}

class BuddyRelationLoaderAdapter extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerBuddyRelationLoader";

    private static int _interval = 120;

    private BuddyRelationLoaderAdapter() {
	super(ctr_endpoints, _interval);
    }

    private static BuddyRelationLoaderAdapter _instance = new BuddyRelationLoaderAdapter();

    public static BuddyRelationLoaderAdapter getInstance() {
	return _instance;
    }

    public void reload(int userId) {
	try {
	    getBuddyRelationLoaderOneway(userId).reload(userId);
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".Reload userId: " + userId
			    + " Ice.TimeoutException");
	}
    }

    public void setRelation(BuddyRelation relation) {
	BuddyRelationLoaderPrx prx = null;
	try {
	    Relationship _relation = new Relationship();
	    _relation.from = relation.fromId;
	    _relation.to = relation.toId;
	    BuddyDesc _desc = BuddyRelation.parseBuddyDesc(relation.getDesc());
	    prx = getBuddyRelationLoaderTwoway(_relation.from);
	    prx.setRelation(_relation, _desc);
	} catch (Ice.TimeoutException e) {
	    Oce.getLogger().error(
		    this.getClass().getName() + ".setRelation ["
			    + prx.toString() + "] relation[" + relation.fromId
			    + ":" + relation.toId + " Ice.TimeoutException");
	}
    }

    protected BuddyRelationLoaderPrx getBuddyRelationLoaderOneway(int userId) {
	Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	if (prx0 != null) {
	    BuddyRelationLoaderPrx prx = BuddyRelationLoaderPrxHelper
		    .uncheckedCast(prx0.ice_timeout(300).ice_oneway());
	    return prx;
	} else {
	    return null;
	}
    }

    protected BuddyRelationLoaderPrx getBuddyRelationLoaderTwoway(int userId) {
	Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	if (prx0 != null) {
	    BuddyRelationLoaderPrx prx = BuddyRelationLoaderPrxHelper
		    .uncheckedCast(prx0.ice_timeout(300).ice_twoway());
	    return prx;
	} else {
	    return null;
	}
    }
    //
    // public static void main(String args[]) {
    // BuddyRelationCacheAdapter.getInstance().checkRelation(67913, 342549289);
    // BuddyRelationCacheAdapter.getInstance().checkRelation(67913, 67913);
    // BuddyRelationCacheAdapter.getInstance().checkRelation(67913, 123459845);
    // CheckedBuddyRelation r = BuddyRelationCacheAdapter.getInstance()
    // .getCheckedRelation(67913, 342549289);
    // System.out.println(r.getSuccess() + " " + r.getRelation().getDesc());
    // }
}
