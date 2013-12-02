package com.xiaonei.xce.ad;

import xce.ad.AdEdmSenderPrx;
import xce.ad.AdEdmSenderPrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;

import java.util.Map;
import java.util.List;
public class AdEdmSenderAdapter extends
		AbstractModProxyHolder<Integer, AdEdmSenderPrx> {

	private static AdEdmSenderAdapter instance;
	private static String endpoints = "@AdEdmSender";

	public AdEdmSenderAdapter() {
		super(ChannelFactory.getChannel(ChannelType.EDM), "ADEDM" + endpoints, 2);
	}

	public static AdEdmSenderAdapter getInstance() {
		if (instance == null) {
			instance = new AdEdmSenderAdapter();
		}
		return instance;
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public AdEdmSenderPrx uncheckedCast(ObjectPrx baseProxy) {
		return AdEdmSenderPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}

	private AdEdmSenderPrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway());
	}

	private AdEdmSenderPrx getManager(int uid) {
		return getProxy(uid);
	}

  //AdResultMap GetAdsTest(int uid,

  public void SendEdm(int user_id, Map<String, String> params)
    throws Exception {
      //getManagerOneway(user_id).Receive(user_id, params);
      getManagerOneway(user_id).SendEdm(user_id, params);
    }

  public void SendEdmTest(int user_id, Map<String, String> params)
    throws Exception {
  //   getManagerOneway(user_id).SendEdmTest(user_id, params);
      getManagerOneway(user_id).SendEdmTest(user_id, params);
  }

}
