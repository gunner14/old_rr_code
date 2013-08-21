/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.network;

import org.json.JSONObject;

import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * NetworkBuilder <br>
 * 
 * @see INetwrokBuilder
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:08:18 AM
 */
public class NetworkBuilder implements INetwrokBuilder {

	private NetworkModel network;

	private UserBasic ub;

	public NetworkBuilder(int userId) {
		this.network = new NetworkModel();
		this.network.setUserId(userId);
		this.ub = SnsAdapterFactory.getUserBasicAdapter().get(userId);
	}

	@Override
	public void buildUnivId() {
		if (null != ub) {
			network.setTypeId(ub.getUniv());
			network.setType(getUnivType(ub.getUniv()));
		}
	}

	@Override
	public void buildUnivName() {
		if (null != ub) {
			network.setName(ub.getUnivName());
		}
	}

	@Override
	public NetworkModel getNetwork() {
		return this.network;
	}

	private String getUnivType(int networkId) {
		if (networkId >= 1001 && networkId <= 19999999) {
			return "学校";
		} else if (networkId >= 20000001 && networkId <= 29999999) {
			return "公司";
		} else if (networkId >= 60000001 && networkId <= 69999999) {
			return "城市";
		} else if (networkId >= 80000000 && networkId <= 90000000) {
			return "学校";
		} else {
			return "";
		}
	}

	@Override
	public JSONObject getNetworkJson() {
		JSONObject jo = new JSONObject();
		jo.put("userId", network.getUserId());
		jo.put("typeId", network.getTypeId());
		jo.put("type", network.getType());
		jo.put("name", network.getName());

		return jo;
	}

}
