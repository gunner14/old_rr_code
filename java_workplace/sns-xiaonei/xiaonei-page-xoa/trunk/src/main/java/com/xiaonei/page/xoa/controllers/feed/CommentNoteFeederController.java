package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.antispam.filter.Replace;
import com.xiaonei.blog.home.BlogHome;
import com.xiaonei.blog.model.Entry;
import com.xiaonei.blog.model.EntryComment;
import com.xiaonei.blog.util.BlogUtil;
import com.xiaonei.page.PageDomainType;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.blog.BlogService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.MetionEncrytedUtil;
import com.xiaonei.page.util.Util;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.core.model.User;

@Path("cnf")
public class CommentNoteFeederController {

    @Autowired
    private BlogService blogService;

    @Autowired
    private BlogHome blogHome;

    @Autowired
    private FeedManager feedManager;

    @Autowired
    private AuthService authService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("blogId") long blogId, @Param("commentId") long commentId,
            @Param("blogOwnerId") int blogOwnerId){
        //如果curHost不是page，直接不发
        if(!PageIdChecker.isPageDomainIn(curHost)){
            return "@done";
        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }

        Entry blog = blogHome.getBlog(blogId, blogOwnerId);
        if (blog == null) {
            logger.error(String.format("note-not-exist!blogId=%s,blogOwnerId=%s",blogId,blogOwnerId));
            return "@note-not-exist";
        }
        
        EntryComment comment =blogHome.getEntryComment(blogOwnerId, commentId);
        if (comment == null) {
            logger.error(String.format("comment-not-exist!commentId=%s,blogOwnerId=%s",comment,blogOwnerId));
            return "@notecomment-not-exist";
        }

        // 为了保证新鲜事模板中的url格式正确，添加一个"?"
        String linkurl = String.format("%s/%s/note/%s?", PageDomainType.getLocalDomain()
                .getDomainUrl(), blog.getOwner(), blog.getId());
        String blogBrief = BlogUtil.Html2Text(blog.getBody(), 60);
        if (!Util.validate(blogBrief)) {
            logger.error("send-feed-failed!because blogBrief");
            return "@send-feed-failed";
        }
        String blogBrief1 = Replace.replaceFacewithEmpty(blogBrief);
        if (!Util.validate(blogBrief1)) {
            logger.error("send-feed-failed!because blogBrief1");
            return "@send-feed-failed";
        }
        String brief = Replace.replaceLinkwithNoHref(blogBrief1);
        if ((brief == null) || (brief.length() == 0)) {
            logger.error("send-feed-failed!because brief");
            return "@send-feed-failed";
        }

        IContext context = new ContextImpl();
        context.put("brief", brief);
        context.put("blog", blog);
        context.put("content", MetionEncrytedUtil.subHtmlContent(comment.getBody(), 35));
        context.put("linkurl", linkurl);
        context.put("cid", commentId);
        context.put("blogOwnerId", blogOwnerId);

        boolean isAdmin = authService.isPageAdministrator(host, pageId);
        try {
            feedManager.publishForXoa(page, host, FeedDefUtil.getBLOG_COMMON_Def(page), isAdmin,
                    context);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";
        }

        return "@done";
    }
}
