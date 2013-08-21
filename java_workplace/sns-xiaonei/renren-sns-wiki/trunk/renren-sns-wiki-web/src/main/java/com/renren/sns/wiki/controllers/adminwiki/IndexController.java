package com.renren.sns.wiki.controllers.adminwiki;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiService;

@Path("")
public class IndexController {

    @Autowired
    private WikiService wikiService;

    @Get("")
    public String index(Invocation inv) {
        inv.addModel("navName", "index");

        List<Wiki> wikiList = new ArrayList<Wiki>();

        boolean light = true;
        int wikiType = 0;
        int page = 0;
        int curPage = 0;
        int endId = 0;
        int pageSize = 24;
        //默认吐高亮数据
        //高亮词库
       wikiList = wikiService.getWikiList(light, wikiType, curPage, endId, page, pageSize,
                "desc");

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
