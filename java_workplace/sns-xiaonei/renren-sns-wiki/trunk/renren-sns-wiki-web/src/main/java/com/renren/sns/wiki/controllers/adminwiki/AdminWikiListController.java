package com.renren.sns.wiki.controllers.adminwiki;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiService;

/**
 * 根据查询条件,列出查询列表
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-3
 * 
 */
@Path("")
public class AdminWikiListController {

    private static final Log logger = LogFactory.getLog(AdminWikiListController.class);

    @Autowired
    private WikiService wikiService;

    /**
     * 查询类型,type=1是高亮,2是根据条件查询
     * 
     * @param inv
     * @param type
     * @param wikiIdStr
     * @return
     */
    @Get("list")
    @Post("list")
    public String index(Invocation inv, @Param("wikiIdStr") String wikiIdStr,
            @Param("wikiName") String wikiName, @Param("light") Boolean light,
            @Param("wikiType") int wikiType, @Param("curPage") int curPage,
            @Param("endId") int endId, @Param("page") int page, @Param("pageSize") int pageSize) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }

        if (pageSize <= 0) {
            pageSize = 24;
        }

        if (logger.isDebugEnabled()) {
            logger.debug("light=" + light + ", wikiType=" + wikiType + ", curPage=" + curPage
                    + ", endId=" + endId + ", page=" + page + ", pageSize=" + pageSize);
        }

        List<Wiki> wikiList = new ArrayList<Wiki>();

        if (!StringUtils.isBlank(wikiIdStr) && wikiIdStr.trim().matches("\\d+")) {
            int wikiId = Integer.parseInt(wikiIdStr);
            Wiki wiki = wikiService.getWikiById(wikiId);
            wikiList.add(wiki);
        } else if (!StringUtils.isBlank(wikiName)) {
            wikiList = wikiService.getWikiListByName(wikiName);
        } else {
            //高亮词库
            wikiList = wikiService.getWikiList(light, wikiType, curPage, endId, page, pageSize,
                    "desc");
        }

        inv.addModel("wikiIdStr", wikiIdStr);
        inv.addModel("wikiName", wikiName);
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        curPage = page;
        if (wikiList != null && wikiList.size() > 0) {
            endId = wikiList.get(wikiList.size() - 1).getId();
        } else {
            curPage = 0;
            endId = 0;
        }
        inv.addModel("curPage", curPage);
        inv.addModel("endId", endId);
        inv.addModel("pageSize", pageSize);

        inv.addModel("wikiList", wikiList);

        return "adminwiki-list";
    }

}
