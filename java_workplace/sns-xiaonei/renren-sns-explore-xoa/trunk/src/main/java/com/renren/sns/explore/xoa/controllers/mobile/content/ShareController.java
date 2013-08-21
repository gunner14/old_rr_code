package com.renren.sns.explore.xoa.controllers.mobile.content;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ExploreCommentService;
import com.renren.sns.explore.service.ExploreUserService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;

public class ShareController {

	private static final Logger logger = Logger
			.getLogger(ShareController.class);

	@Autowired
	private ExploreCommentService exploreCommentService;
	
	@Autowired
	private ExploreUserService exploreUserService;
	
	@Get
	@Post
	public Object index(@Param("uid") int userId,
			@Param("cid") String contentId, @Param("body") String commentBody) {
		try {
			
			if(userId==0||contentId==null){
				return new Boolean(false);
			}

			//安全
			CheckResult cr = com.xiaonei.antispam.AntiSpamAdapter.getInstance().antispamFilter(userId, 0, CheckType.SHARE_REPLY, commentBody, null);
			
			//违禁词,不能发布
			if(CheckResult.PROHIBITED==cr.getFlag()){
				 JSONObject jsonObject = new JSONObject();
				 jsonObject.put("code", 1);
				 jsonObject.put("msg", cr.getShowMessage());
				 return "@"+jsonObject.toString();
			}
			Date commentDate = new Date();
			DateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

			//是否需要审核
			boolean audit = false;
			if(CheckResult.CONTENT_NEED_AUDIT==cr.getFlag()||CheckResult.AUDIT_AND_NOT_NOTIFY==cr.getFlag()){
				audit = true;
			}
			long commentId = exploreCommentService.addComment(userId, contentId, commentBody, commentDate, audit);
			if(commentId!=-1){
				return new Boolean(true);
			}else{
				return new Boolean(false);
			}
		} catch (Exception e) {
			logger.error("share content failtrue", e);
			return new XoaBizErrorBean("share content error");
		}
	}

}
