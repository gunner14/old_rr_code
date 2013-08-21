package com.xiaonei.platform.component.gossip.biz;

import org.springframework.stereotype.Service;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.wservice.userscore.client.StrategyType;
import com.xiaonei.wservice.userscore.client.UserScoreAdapter;

@Service
public class GossipScoreBiz {
	public void addScore(User host,User guest,User ucc){
		if(host.getId()!=guest.getId()){
			UserScoreAdapter.getInstance().addScore(host.getId(), StrategyType.UGC_GOSSIP, 1);
			UserScoreAdapter.getInstance().addScore(guest.getId(), StrategyType.UGC_REPLY, 1);
			if(  ucc != null&&  host.getId() != ucc.getId() && guest.getId()!= ucc.getId()){
				UserScoreAdapter.getInstance().addScore(ucc.getId(), StrategyType.UGC_REPLY, 1);
			}
		}
		else{
			if( ucc != null && host.getId() != ucc.getId() ){
				UserScoreAdapter.getInstance().addScore(ucc.getId(), StrategyType.UGC_REPLY, 1);
			}
		}
	}
}
