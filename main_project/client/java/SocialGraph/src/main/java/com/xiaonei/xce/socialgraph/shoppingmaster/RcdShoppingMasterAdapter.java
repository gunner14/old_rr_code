package com.xiaonei.xce.socialgraph.shoppingmaster;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RcdMessageServicePrx;
import xce.socialgraph.RcdMessageServicePrxHelper;
import xce.util.channel.Channel;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.socialgraph.BusinessCacheAdapter;
import com.xiaonei.xce.socialgraph.common.RcdBlockBase;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

/**
 * 为shopping master提供逛街达人页面的添加block表
 * @author xiaoyun.yang
 * @email xiaoyun.yang@renren-inc.com
 */
public class RcdShoppingMasterAdapter extends ReplicatedClusterAdapter implements
		RcdBlockBase {
	private static final String endpoints = "ControllerSocialGraphRcdMessageService";
	private static int interval = 120;
	private static RcdShoppingMasterAdapter instance = null;
	private static final String rcdType = "RCDSM";
	private static final String biz = "RCDSM";
	
	private RcdShoppingMasterAdapter() {
		super(endpoints, interval, Channel.newSocialGraphChannel(endpoints));
	}

	public static RcdShoppingMasterAdapter getInstance() {
		if (null == instance) {
			synchronized (RcdShoppingMasterAdapter.class) {
				if (null == instance) {
					try {
						instance = new RcdShoppingMasterAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}

	
	/**
	 * 删除首页shoppingmaster推荐类型接口,先删cache数据，再存储block表
	 */
	@Override
	public void removeRcd(long source, long block) {
		RcdMessageServicePrx prx =  getRcdMessageServicePrxOneWay(source);
		if(prx == null){
			System.out.println("1.prx null");
			return;
		} else{
			prx.removeBizRcd(source, block, rcdType);
			prx.addSgBlock(source, block, rcdType);
		}
//		getRcdMessageServicePrxOneWay(source).removeBizRcd(source, block, rcdType);	//删除cache数据
//		getRcdMessageServicePrxOneWay(source).addSgBlock(source, block, rcdType);	//加入socialgraph block表
	}
	
	/**
	 * 获取twoway代理
	 * @param source
	 * @return
	 */
	private RcdMessageServicePrx getRcdMessageServicePrxTwoWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_twoway());
			return prx;
		}
		return null;
	}
	
	/**
	 * 获取oneway代理
	 * @param source
	 * @return
	 */
	private RcdMessageServicePrx getRcdMessageServicePrxOneWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_oneway());
			return prx;
		}
		return null;
	}

	public static void main(String[] args) {
		int hostId = 232491085;
		int blockId = 229196311;
		RcdShoppingMasterAdapter.getInstance().removeRcd(hostId,blockId);
		System.out.println("remove finished!");
	}
}
