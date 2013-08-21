package com.renren.sns.wiki.controllers.adminwiki;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.dict.WikiWord;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.dic.WikiDictionaryService;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

@Path("")
public class AdminWikiWordController {

    private static final Log logger = LogFactory.getLog(AdminWikiWordController.class);

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiDictionaryService wikiDictionaryService;

    @Autowired
    private WikiService wikiService;

    @Get("modifyWikiWord/${wikiId:\\d+}")
    public String modifyWikiInfo(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        Wiki wiki = wikiHolder.getWiki();

        List<WikiWord> wikiWordList = wikiDictionaryService.getWikiWordListById(wikiId);

        inv.addModel("wiki", wiki);
        inv.addModel("wikiWordList", wikiWordList);

        return "modify-wiki-word";
    }

    /**
     * 添加切词库
     * 
     * @param name
     * @return
     */
    @Post("wikiWord/add")
    public String addWikiWord(@Param("wikiId") int wikiId, @Param("name") String name) {

        try {
            List<WikiWord> wikiWordList = wikiDictionaryService.getWikiWordListByName(name);
            if (wikiWordList != null && wikiWordList.size() > 0) {} else {
                wikiDictionaryService.addWikiWord(wikiId, name);
            }
            //            return "@添加成功";
            return "r:http://wiki.renren.com/adminwiki/modifyWikiWord/" + wikiId;
        } catch (Exception e) {
            logger.error("", e);
            return "@" + e.getMessage();
        }
    }

    @Get("wikiWord/delete")
    @Post("wikiWord/delete")
    public String deleteWikiWord(@Param("wikiId") int wikiId, @Param("wikiWordId") int wikiWordId) {
        try {
            int result = wikiDictionaryService.deleteWikiWord(wikiWordId);
            if (result > 0) {
                return "r:http://wiki.renren.com/adminwiki/modifyWikiWord/" + wikiId;
            } else {
                return "@删除失败";
            }
        } catch (Exception e) {
            logger.error("", e);
            return "@" + e.getMessage();
        }
    }

    @Get("modifyWikiWord/${wikiId:\\d+}/pre")
    @LoginRequired
    public String modifyWikiWordPre(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 1, wikiId, 0, 1, "desc");
        Wiki wiki = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            List<WikiWord> wikiWordList = wikiDictionaryService.getWikiWordListById(wikiId);
            inv.addModel("wikiWordList", wikiWordList);
        }

        inv.addModel("wiki", wiki);
        return "modify-wiki-word";
    }

    @Get("modifyWikiWord/${wikiId:\\d+}/next")
    @LoginRequired
    public String modifyWikiWordNext(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 0, wikiId, 1, 1, "desc");
        Wiki wiki = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            List<WikiWord> wikiWordList = wikiDictionaryService.getWikiWordListById(wikiId);
            inv.addModel("wikiWordList", wikiWordList);
        }

        inv.addModel("wiki", wiki);
        return "modify-wiki-word";
    }

}
