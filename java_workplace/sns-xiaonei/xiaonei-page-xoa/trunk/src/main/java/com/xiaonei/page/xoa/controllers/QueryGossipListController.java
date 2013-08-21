package com.xiaonei.page.xoa.controllers;

import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.xoa.domain.GossipViewX;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("qgl")
public class QueryGossipListController {

	@Autowired
	private GossipService gossipService;
	

	@Get
	public GossipViewX[] index(@Param("pageId") int pageId,@Param("offset") int offset, @Param("limit") int limit,@Param("opUserId")int opUserId) {
		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);
		
		List<GossipView> gossips = gossipService.getGossipList(puser, puser,offset, limit);
		List<GossipViewX> gossipViews = new LinkedList<GossipViewX>();

		if(gossips!=null){
			for (GossipView gossip : gossips) {
				GossipViewX gv = new GossipViewX();
				gv.setBody(gossip.getBody());
				gv.setCharity(gossip.isCharity());
				gv.setChatId(gossip.getChatId());
				gv.setChatName(gossip.getChatName());
				gv.setGuestId(gossip.getGuestId());
				gv.setGuestUniv(gossip.getGuestUniv());
				gv.setGuestName(gossip.getGuestName());
				gv.setId(gossip.getId());
				gv.setKeepUse(gossip.isKeepUse());
				gv.setOnline(gossip.isOnline());
				gv.setSuperEmotion(gossip.isSuperEmotion());
				gv.setTime(gossip.getTime());
				gv.setTinyUrl(gossip.getTinyUrl());
				gv.setVip(gossip.isVip());
				
				gossipViews.add(gv);
			}
			
			return gossipViews.toArray(new GossipViewX[gossips.size()]);
		}
		return null;
	}
}
