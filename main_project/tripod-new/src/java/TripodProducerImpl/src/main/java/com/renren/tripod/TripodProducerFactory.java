package com.renren.tripod;

import java.util.Map;
import java.util.HashMap;
import com.renren.tripod.impl.TestTripodProducer;
import com.renren.tripod.impl.BadooTipsTripodProducer;
import com.renren.tripod.impl.mau.viprecall.LoginUserProducer;
import com.renren.tripod.impl.mau.viprecall.ActiveFriProducer;
import com.renren.tripod.impl.newbie.helper.HelperProducer;

public class TripodProducerFactory {
	private Map<String, TripodProducer> _map = new HashMap<String, TripodProducer>();;
	
  public TripodProducerFactory() {
		_map.put("TEST", new TestTripodProducer());
		_map.put("BADOO", new BadooTipsTripodProducer());
		_map.put("VIP_RECALL_LOGIN_USER", new LoginUserProducer());
		_map.put("VIP_RECALL_ACTIVE_USER", new ActiveFriProducer());
		_map.put("newbie_helper", new HelperProducer());
	}

  TripodProducer get(String business) {
    return _map.get(business);
  }
}
