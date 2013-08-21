package com.xiaonei.page.xoa.controllers;

import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.page.xoa.domain.DoingReplyView;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.ReplyDoing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("qdrl")
public class QueryDoingReplyListController {	
	@Get
	@Post
	public DoingReplyView[] index(@Param("pageId") int pageId,
			@Param("doingId") int doingId, @Param("offset") int offset,
			@Param("limit") int limit) {
		User user = SnsAdapterFactory.getUserAdapter().get(pageId);
		if (user != null) {
			List<ReplyDoing> replyList = DoingHome.getInstance()
					.getDoingReplyList(doingId, user.getId(), offset, limit);
			List<DoingReplyView> replyViewList=new LinkedList<DoingReplyView>();
			if(replyList!=null){
				for (ReplyDoing replyDoing : replyList) {
					DoingReplyView drv=new DoingReplyView();
					
					drv.setId(replyDoing.getId());
					drv.setDoingId(replyDoing.getDoingId());
					drv.setMeta(replyDoing.getMeta());
					drv.setOwnerId(replyDoing.getOwnerId());
					drv.setOwnerName(replyDoing.getOwnerName());
					drv.setReplyerId(replyDoing.getReplyerId());
					drv.setReplyerName(replyDoing.getReplyerName());
					drv.setReplyerTinyurl(replyDoing.getReplyerTinyurl());
					drv.setReplyTime(replyDoing.getReplyTime());
					drv.setReplyContent(replyDoing.getReplyContent());
					
					replyViewList.add(drv);
				}
				
				return replyViewList.toArray(new DoingReplyView[replyList.size()]);
			}
		}

		return null;
	}
}
