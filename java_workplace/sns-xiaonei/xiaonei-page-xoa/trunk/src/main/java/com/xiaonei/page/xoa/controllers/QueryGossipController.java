package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.xoa.domain.GossipViewX;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("qg")
public class QueryGossipController {
	
	@Autowired
	private GossipService gossipService;
	
	@Get
	public GossipViewX index(@Param("pageId") int pageId,@Param("gossipId") long gossipId) {
		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);		
		GossipView g=gossipService.getGossip(puser.getId(), gossipId);
		if(g!=null){
			GossipViewX gv = new GossipViewX();
			gv.setBody(g.getBody());
			gv.setCharity(g.isCharity());
			gv.setChatId(g.getChatId());
			gv.setChatName(g.getChatName());
			gv.setGuestId(g.getGuestId());
			gv.setGuestUniv(g.getGuestUniv());
			gv.setGuestName(g.getGuestName());
			gv.setId(g.getId());
			gv.setKeepUse(g.isKeepUse());
			gv.setOnline(g.isOnline());
			gv.setSuperEmotion(g.isSuperEmotion());
			gv.setTime(g.getTime());
			gv.setTinyUrl(g.getTinyUrl());
			gv.setVip(g.isVip());
			
			return gv;
		}
		
		return null;
	}
}
