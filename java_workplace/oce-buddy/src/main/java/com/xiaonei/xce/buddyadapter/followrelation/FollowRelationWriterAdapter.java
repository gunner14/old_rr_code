package com.xiaonei.xce.buddyadapter.followrelation;

import xce.buddy.FollowRelationWriterPrx;
import xce.buddy.FollowRelationWriterPrxHelper;
import xce.clusterstate.ReplicatedClusterAdapter;

import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

/**
 * FollowRelationWriterAdapter
 * 
 */

public class FollowRelationWriterAdapter extends ReplicatedClusterAdapter {

	private static final String endpoints = "ControllerFollowRelationWriter";

	private static int interval = 120;

	private static FollowRelationWriterAdapter instance = null;

	private static final int TIME_OUT = 5000;

	/**
	 * @return
	 */
	public static FollowRelationWriterAdapter getInstance() {
		if (instance == null) {
			synchronized (FollowRelationWriterAdapter.class) {
				if (instance == null) {
					try {
						instance = new FollowRelationWriterAdapter();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}

			}
		}
		return instance;
	}

	/**
	 * 
	 */
	private FollowRelationWriterAdapter() {
		super(endpoints, interval);
	}

	/**
	 * @param id
	 * @return
	 */
	private FollowRelationWriterPrx getFollowRelationWriterPrxTwoWay(int id) {
		Ice.ObjectPrx prx0 = getValidProxy(id, TIME_OUT);
		if (prx0 != null) {
			FollowRelationWriterPrx prx = FollowRelationWriterPrxHelper
					.uncheckedCast(prx0.ice_timeout(TIME_OUT).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}

	/**
	 * 添加关注
	 * 
	 * @param uid 用户ID
	 * @param gid 资源ID对象
	 * @return
	 */
	public void addFollowRelation(int uid, GlobalId gid) {

		FollowRelationWriterPrx proxy = null;
		MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

		try {
			proxy = getFollowRelationWriterPrxTwoWay(0);
			proxy.add(myGid, uid);

		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					getClass().getName() + ".addFollowRelation "
							+ myGid.toString() + " " + uid
							+ " Ice.ConnectTimeoutException "
							+ proxy.ice_toString());
			e.printStackTrace();
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					getClass().getName() + ".addFollowRelation "
							+ myGid.toString() + " " + uid
							+ " Ice.TimeoutException " + proxy.ice_toString());
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 删除关注
	 * 
	 * @param uid 用户ID
	 * @param gid 资源ID对象
	 * @return
	 */
	public void removeFollowRelation(int uid, GlobalId gid) {

		FollowRelationWriterPrx proxy = null;
		MyUtil.GlobalId myGid = new MyUtil.GlobalId(gid.getType(), gid.getId());

		try {
			proxy = getFollowRelationWriterPrxTwoWay(0);
			proxy.remove(myGid, uid);

		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					getClass().getName() + ".removeFollowRelation "
							+ myGid.toString() + " " + uid
							+ " Ice.ConnectTimeoutException "
							+ proxy.ice_toString());
			e.printStackTrace();
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					getClass().getName() + ".removeFollowRelation "
							+ myGid.toString() + " " + uid
							+ " Ice.TimeoutException " + proxy.ice_toString());
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {

		GlobalId x = GlobalId.fromMini(12345678);
		FollowRelationWriterAdapter.getInstance().addFollowRelation(222309500,
				x);
//		FollowRelationWriterAdapter.getInstance().removeFollowRelation(349440824,
//				x);
		System.exit(0);

	}
}
