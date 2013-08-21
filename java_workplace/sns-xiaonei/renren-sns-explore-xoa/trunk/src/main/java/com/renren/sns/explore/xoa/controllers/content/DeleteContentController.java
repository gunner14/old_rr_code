package com.renren.sns.explore.xoa.controllers.content;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.mongodb.DBObject;
import com.renren.sns.explore.service.ContentService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;


/**
 *	根据fromId和resourceId删除某条内容 
 * @author weiwei.wang
 */
public class DeleteContentController {

	private static final Logger logger = Logger.getLogger(DeleteContentController.class);
	
	@Autowired
	private ContentService contentService;
	
	@Get
	@Post
	public Object index(@Param("from")int from, @Param("rid")long resourceId){
		try {
			logger.debug("from="+from+"\tresouceId="+resourceId);
			DBObject dbObject = contentService.getContentDBObject(from, resourceId);
			if(dbObject==null){
				return new Boolean(false);
			}else{
				contentService.deleteContent(from, resourceId);
				return new Boolean(true);
			}
		} catch (Exception e) {
			logger.error("delete conent by fromId and resourceId failure", e);
			return new XoaBizErrorBean("delete content error");
		}
	}
	
}
