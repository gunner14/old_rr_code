package com.xiaonei.page.xoa.controllers;

import java.sql.SQLException;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;

import com.xiaonei.page.xoa.domain.DoingView;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("qd")
public class QueryDoingController {

	private final static Logger logger = Logger.getLogger("XNPAGE");

	@Get
	public DoingView index(@Param("pageId") int pageId,@Param("doingId") int doingId) {		
		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);		
		try {
			Doing doing = DoingHome.getInstance().getDoingById(doingId,puser.getId());			
			if (doing != null) {
				DoingView dv = new DoingView();

				dv.setCommentCount(doing.getCommentCount());
				dv.setContent(doing.getContent());
				dv.setDoTime(doing.getDoTime());
				dv.setId(doing.getId());
				dv.setIp(doing.getIp());
				dv.setLink(doing.getLink());
				dv.setMarkbit(doing.getMarkbit());
				dv.setOwnerName(doing.getOwnerName());
				dv.setRepeatCount(doing.getRepeatCount());
				dv.setRepeatCountTotal(doing.getRepeatCountTotal());
				dv.setTinyUrl(doing.getTinyUrl());
				dv.setType(doing.getType());
				dv.setUserId(doing.getUserId());
				return dv;
			}

		} catch (SQLException e) {
			logger.error(e.getMessage(),e);
		}
		return null;
	}
}
