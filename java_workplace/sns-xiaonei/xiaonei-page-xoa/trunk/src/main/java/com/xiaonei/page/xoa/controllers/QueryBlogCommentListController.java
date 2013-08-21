package com.xiaonei.page.xoa.controllers;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.blog.view.CommentListView;
import com.xiaonei.page.service.blog.BlogService;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

//TODO 完善功能
@Path("qbcl")
public class QueryBlogCommentListController {

    @Autowired
    private BlogService blogService;
    
    private final static Logger logger = Logger.getLogger("XNPAGE");
    
    @Get
    public String index(@Param("blogId")long blogId,@Param("ownerId")int ownerId){
        List<CommentListView> list=blogService.getBlogCommentList(blogId, ownerId, 0, 10);
        for (CommentListView commentListView : list) {
            logger.info(commentListView.getId());
            logger.info(commentListView.getBody());
            logger.info(commentListView.getEntry());
            logger.info("*********************************");
        }
        
        return "@done";
    }
}
