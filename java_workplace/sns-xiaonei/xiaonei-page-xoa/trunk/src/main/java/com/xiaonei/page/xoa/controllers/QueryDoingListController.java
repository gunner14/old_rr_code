package com.xiaonei.page.xoa.controllers;

import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.xoa.domain.DoingView;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("qdl")
public class QueryDoingListController {

	@Autowired
	private DoingService doingService;

	private static final Logger logger = Logger.getLogger("XNPAGE");
	@Get
	public DoingView[] index(@Param("pageId") int pageId,
			@Param("offset") int offset, @Param("limit") int limit) {
		logger.info("begin execute QueryDoing!");

		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);
		try {
			List<Doing> doings = doingService.getDoingListWithoutLog(puser,
					offset, limit);
			List<DoingView> doingViews = new LinkedList<DoingView>();
			if (doings != null) {
				for (Doing doing : doings) {
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

					doingViews.add(dv);
				}
				
				return doingViews.toArray(new DoingView[doings.size()]);
			}
		} catch (SQLException e) {
			logger.error("Get doings failed!", e);
		}
		return null;
	}
}
