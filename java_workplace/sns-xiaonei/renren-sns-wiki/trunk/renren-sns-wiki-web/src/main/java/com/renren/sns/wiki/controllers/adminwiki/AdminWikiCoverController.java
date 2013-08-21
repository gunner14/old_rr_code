package com.renren.sns.wiki.controllers.adminwiki;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.WikiCover;
import com.renren.sns.wiki.service.WikiCoverService;
import com.renren.sns.wiki.service.WikiService;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

@Path("")
public class AdminWikiCoverController {

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiCoverService wikiCoverService;

    @Get("modifyWikiCover/${wikiId:\\d+}")
    @LoginRequired
    public String modifyWikiHead(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        Wiki wiki = wikiHolder.getWiki();
        WikiCover wikiCover = wikiCoverService.getWikiCover(wiki);

        inv.addModel("wiki", wiki);
        inv.addModel("wikiCover", wikiCover);
        return "modify-wiki-cover";
    }

    @Get("modifyWikiCover/${wikiId:\\d+}/pre")
    @LoginRequired
    public String modifyWikiHeadPre(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 1, wikiId, 0, 1, "desc");
        Wiki wiki = null;
        WikiCover wikiCover = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            wikiCover = wikiCoverService.getWikiCover(wiki);
        }

        inv.addModel("wiki", wiki);
        inv.addModel("wikiCover", wikiCover);
        return "modify-wiki-cover";
    }

    @Get("modifyWikiCover/${wikiId:\\d+}/next")
    @LoginRequired
    public String modifyWikiHeadNext(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 0, wikiId, 1, 1, "desc");
        Wiki wiki = null;
        WikiCover wikiCover = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            wikiCover = wikiCoverService.getWikiCover(wiki);
        }

        inv.addModel("wiki", wiki);
        inv.addModel("wikiCover", wikiCover);
        return "modify-wiki-cover";
    }

}
