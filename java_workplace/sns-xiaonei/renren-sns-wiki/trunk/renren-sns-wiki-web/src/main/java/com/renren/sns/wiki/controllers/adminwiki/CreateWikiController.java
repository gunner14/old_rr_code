package com.renren.sns.wiki.controllers.adminwiki;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiRegisterService;

@Path("")
public class CreateWikiController {

    private static final Log logger = LogFactory.getLog(CreateWikiController.class);

    @Autowired
    private WikiRegisterService wikiRegisterService;

    @Get("createWikiPage")
    public String createWikiPage(Invocation inv) {
        inv.addModel("navName", "createWiki");
        return "create-wiki";
    }

    @Post("createWiki")
    public String createWiki(Invocation inv, @Param("name") String name, @Param("type") int type,
            @Param("isbn") String isbn, @Param("imdb") String imdb) {
        inv.addModel("navName", "createWiki");

        String msg = null;
        try {
            //检查分类是否s有
            WikiType wikiType = WikiType.getEnum(type);
            logger.debug("wikiType=" + wikiType + ", imdb==null:" + (imdb == null)
                    + ", StringUtils.isBlank(imdb)=" + StringUtils.isBlank(imdb));
            if (StringUtils.isBlank(name)) {
                msg = "创建失败,原因:名称不能为空";
            } else if (wikiType != null) {
                if (wikiType == WikiType.BOOK && StringUtils.isBlank(isbn)) {
                    msg = "创建失败,原因:图书需要传isbn";
                } else if (wikiType == WikiType.MOVIE && StringUtils.isBlank(imdb)) {
                    msg = "创建失败,原因:电影需要传imdb";
                } else {
                    Wiki wiki = wikiRegisterService.createWiki(name, wikiType, isbn, imdb);
                    msg = wiki.getName() + " 创建成功, wikiId:" + wiki.getId();
                }
            } else {
                msg = "创建失败,原因:类型不对";
            }
        } catch (Exception e) {
            logger.error("", e);
            msg = "创建失败,原因:" + e.getMessage();
        }

        inv.addModel("msg", msg);
        return "create-wiki";
    }
}
