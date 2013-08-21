package com.xiaonei.xce.followerinput;

import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.clusterstate.ClientInterface;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;
import Ice.ConnectTimeoutException;
import Ice.TimeoutException;

import com.xiaonei.services.FollowerInput.FollowerInputManagerPrx;
import com.xiaonei.services.FollowerInput.FollowerInputManagerPrxHelper;
import com.xiaonei.xce.log.Oce;

public class FollowerInputAdapter extends ReplicatedClusterAdapter {
	private static Log logger = LogFactory.getLog(FollowerInputAdapter.class);
	private static final String ctr_endpoints = "ControllerFollowerInput";

	private static int _interval = 120;
	private static final int TIMEOUT = 10000;

	private static FollowerInputAdapter _instance = new FollowerInputAdapter();

	public static FollowerInputAdapter getInstance() {
		return _instance;
	}

	private FollowerInputAdapter() {
		super(ctr_endpoints, _interval, Channel
				.newGadgetsChannel(ctr_endpoints));
	}

	public ClientInterface getClientInterface() {
		return _csClient;
	}

	private FollowerInputManagerPrx getFollowerInputManager(int owner) {
		Ice.ObjectPrx prx0 = getValidProxy(owner, TIMEOUT);
		if (prx0 != null) {
			FollowerInputManagerPrx prx = FollowerInputManagerPrxHelper
					.uncheckedCast(prx0.ice_timeout(TIMEOUT).ice_twoway());
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}

	public int[] getFollowerInput(int hostId, String condition, int limit) {
		int[] x = null;
		FollowerInputManagerPrx prx = null;
		try {
			prx = getFollowerInputManager(hostId);
			x = prx.getFollowerInput(hostId, condition, limit);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getFriendInput [" + prx
							+ "] userId=" + hostId + " " + e);
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getFriendInput [" + prx
							+ "] userId=" + hostId + " " + e);
		}
		return x;
	}

	public static void main(String[] args) {
		try {

			Scanner cin = new Scanner(System.in);
			String str = cin.nextLine();
			String[] s = str.split(" ");
			for (int i = 0; i < s.length; i++) {
				int[] x = FollowerInputAdapter.getInstance().getFollowerInput(
						220840375, s[i], 10);
				System.out.println(x.length);
				for (int u : x)
					System.out.println(u);
			}
		} catch (Exception e) {
			System.out.println(e);
		}

		System.exit(0);
	}
}
