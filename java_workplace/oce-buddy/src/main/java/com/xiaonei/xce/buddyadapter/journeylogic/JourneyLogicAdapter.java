package com.xiaonei.xce.buddyadapter.journeylogic;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.journey.JourneyLogicPrx;
import xce.journey.JourneyLogicPrxHelper;
import xce.util.channel.Channel;

import com.xiaonei.xce.globalid.GlobalId;
import com.xiaonei.xce.log.Oce;

/**
 * <p>
 * Title: JourneyLogicAdapter.java
 * </p>
 * <p>
 * Description: 足迹相关adapter。
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author jipeng.lin@renren-inc.com
 * @date 2012-11-12
 */


public class JourneyLogicAdapter extends ReplicatedClusterAdapter {
	private static final String endpoints = "ControllerJourneyLogic";
	private static int interval = 120;
	private static JourneyLogicAdapter instance = null;
	private static final int TIME_OUT = 300;

	public static JourneyLogicAdapter getInstance() {
		if (instance == null) {
			synchronized (JourneyLogicAdapter.class) {
				if (instance == null) {
					try {
						instance = new JourneyLogicAdapter();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		}
		return instance;
	}

	private JourneyLogicAdapter() {
		super(endpoints, interval);
	}

	private String getInvokeMethodName() {
		return Thread.currentThread().getStackTrace()[2].getMethodName();
	}

	private JourneyLogicPrx getJourneyLogicPrxTwoway(long key) {
		// Todo:
		int modId = (int) (key % 100);
		Ice.ObjectPrx proxy0 = getValidProxy(modId, TIME_OUT);
		if (proxy0 != null) {
			JourneyLogicPrx prx = JourneyLogicPrxHelper.uncheckedCast(proxy0
					.ice_timeout(TIME_OUT).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}

	private JourneyLogicPrx getJourneyLogicPrxOneway(long key) {
		// Todo:
		int modId = (int) (key % 100);
		Ice.ObjectPrx proxy0 = getValidProxy(modId, TIME_OUT);
		if (proxy0 != null) {
			JourneyLogicPrx prx = JourneyLogicPrxHelper.uncheckedCast(proxy0
					.ice_timeout(TIME_OUT).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	/**
	 * getCount接口 返回两次登录之间访问人数
	 * 
	 * @param gid
	 * @return
	 * @throws JourneyLogicException
	 */
	public int getCount(GlobalId gid) throws JourneyLogicException {
		String invokeInfo = getClass().getName() + "." + getInvokeMethodName()
				+ "( " + gid.toString() + ")";
		int result = 0;
		long key = gid.toLong();
		JourneyLogicPrx proxy = JourneyLogicAdapter.getInstance()
				.getJourneyLogicPrxTwoway(key);
		try {

			result = proxy.getCount(key);
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Exception e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		}
		return result;
	}

	/**
	 * getList接口 返回两次直接的反问人员的id列表（0-limit）
	 * 
	 * @param gid
	 * @param limit
	 * @return
	 * @throws JourneyLogicException
	 */
	public List<Integer> getList(GlobalId gid, int limit)
			throws JourneyLogicException {
		String invokeInfo = getClass().getName() + "." + getInvokeMethodName()
				+ "( " + gid.toString() + " ," + limit + ")";
		List<Integer> result = new ArrayList<Integer>();
		long key = gid.toLong();
		JourneyLogicPrx proxy = JourneyLogicAdapter.getInstance()
				.getJourneyLogicPrxTwoway(key);
		try {

			int[] tempArray = proxy.getList(key, limit);
			for (int temp : tempArray) {
				result.add(temp);
			}
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Exception e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		}
		return result;
	}

	/**
	 * invote接口 每次guestId访问需要调用该接口，修改缓存中count和list的数据
	 * 
	 * @param gid
	 * @param guestId
	 * @param limit
	 * @throws JourneyLogicException
	 */
	public void invote(GlobalId gid, int guestId, int limit)
			throws JourneyLogicException {
		String invokeInfo = getClass().getName() + "." + getInvokeMethodName()
				+ "( " + gid.toString() + " ," + guestId + " ," + limit + ")";
		long key = gid.toLong();
		JourneyLogicPrx proxy = JourneyLogicAdapter.getInstance()
				.getJourneyLogicPrxTwoway(key);
		try {
			proxy.invote(key, guestId, limit);

		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Exception e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		}
	}

	/**
	 * reset接口 用户登录查看后的清零
	 * 
	 * @param gid
	 * @throws JourneyLogicException
	 */
	public void reset(GlobalId gid) throws JourneyLogicException {
		String invokeInfo = getClass().getName() + "." + getInvokeMethodName()
				+ "( " + gid.toString() + ")";
		long key = gid.toLong();
		JourneyLogicPrx proxy = JourneyLogicAdapter.getInstance()
				.getJourneyLogicPrxTwoway(key);
		try {
			proxy.reset(key);
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		} catch (Exception e) {
			Oce.getLogger().error(
					invokeInfo + " " + e.toString() + " "
							+ proxy.ice_toString());
			throw new JourneyLogicException(e);
		}
	}

//	public static void main(String[] argc) {
//		try {
//			System.out.println("Please input a testNum");
//			BufferedReader br = new BufferedReader(new InputStreamReader(
//					System.in));
//			int userId = Integer.parseInt(br.readLine());
//			System.out.println(GlobalId.fromZujiVisit(userId).toLong());
//			JourneyLogicAdapter.getInstance().reset(
//					GlobalId.fromZujiVisit(userId));
//
//			JourneyLogicAdapter.getInstance().invote(
//					GlobalId.fromZujiVisit(userId), 1, 10);
//			JourneyLogicAdapter.getInstance().invote(
//					GlobalId.fromZujiVisit(userId), 123456789, 10);
//			JourneyLogicAdapter.getInstance().invote(
//					GlobalId.fromZujiVisit(userId), 13579, 10);
//			JourneyLogicAdapter.getInstance().invote(
//					GlobalId.fromZujiVisit(userId), 12345, 10);
//			System.out.println("userId is :"
//					+ JourneyLogicAdapter.getInstance().getList(
//							GlobalId.fromZujiVisit(userId), 10));
//			System.out.println("userId is: "
//					+ JourneyLogicAdapter.getInstance().getCount(
//							GlobalId.fromZujiVisit(userId)));
//
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//		System.exit(0);
//	}
}
