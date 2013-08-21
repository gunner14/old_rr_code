package com.xiaonei.page.xoa.controllers.feed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.blog.model.Entry;
import com.xiaonei.blog.util.BlogUtil;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.blog.BlogService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.tools.StringUtil;
import com.xiaonei.platform.core.model.User;

@Path("npf")
public class NotePublishFeederController {

    @Autowired
    private FeedManager feedManager;

    @Autowired
    private BlogService blogService;

    @Autowired
    private AuthService authService;

    private static final int CUT_WORD_ENTRY_TITLE_LENGTH = 35;

    private static final int CUT_WORD_ENTRY_BODY_LENGTH = 35;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,
            @Param("blogId") long blogId) {
        if (!PageIdChecker.isPageDomainIn(curHost)) {
            logger.info(String.format("the curHost is not a page!pageId=%s,curHost=%s,blogId=%s",
                    pageId, curHost, blogId));
            return "done";
        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }
        Entry blog = blogService.getBlogWithoutLog(blogId, pageId);
        if (blog == null) {
            return "@note-not-exist";
        }
        String b = BlogUtil.Html2Text(blog.getBody(), 180);
        String bodyCuted = StringUtil.cutLongEngWord(b, CUT_WORD_ENTRY_BODY_LENGTH);
        String titleCuted = StringUtil.cutLongEngWord(blog.getTitle(), CUT_WORD_ENTRY_TITLE_LENGTH);

        //新的新鲜事分发
        try {
            IContext context = new ContextImpl();
            context.put("titleCuted", titleCuted);
            context.put("bodyCuted", bodyCuted);
            context.put("blog", blog);

            boolean isAdmin = authService.isPageAdministrator(host, pageId);
            feedManager.publishForXoa(page, host, FeedDefUtil.getBLOG_PUBLISH_Def(page), isAdmin,
                    context);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return "@send-feed-failed";

        }
        return "@done";
    }
}
