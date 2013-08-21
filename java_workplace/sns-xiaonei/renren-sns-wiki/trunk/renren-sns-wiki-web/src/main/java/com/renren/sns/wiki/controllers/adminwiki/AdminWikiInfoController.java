package com.renren.sns.wiki.controllers.adminwiki;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.info.WikiInfoProperty;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.info.WikiInfoService;

@Path("")
public class AdminWikiInfoController {

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Get("modifyWikiInfo/${wikiId:\\d+}")
    public String modifyWikiInfo(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        Wiki wiki = wikiHolder.getWiki();

        List<WikiInfoProperty> fullWikiInfoPropertylist = wikiInfoService
                .getFullWikiInfoService(wiki);

        inv.addModel("wiki", wiki);
        inv.addModel("wikiInfoPropertylist", fullWikiInfoPropertylist);

        return "modify-wiki-info";

    }

    @Get("modifyWikiInfo/${wikiId:\\d+}/pre")
    public String modifyWikiInfoPre(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 1, wikiId, 0, 1, "desc");
        Wiki wiki = null;
        List<WikiInfoProperty> fullWikiInfoPropertylist = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            fullWikiInfoPropertylist = wikiInfoService.getFullWikiInfoService(wiki);
        }

        inv.addModel("wiki", wiki);
        inv.addModel("wikiInfoPropertylist", fullWikiInfoPropertylist);

        return "modify-wiki-info";
    }

    @Get("modifyWikiInfo/${wikiId:\\d+}/next")
    public String modifyWikiInfoNext(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {
        inv.addModel("navName", "index");

        if (light == null) {
            light = true;
        }
        inv.addModel("light", light);
        inv.addModel("wikiType", wikiType);

        List<Wiki> wikiList = wikiService.getWikiList(light, wikiType, 0, wikiId, 1, 1, "desc");
        Wiki wiki = null;
        List<WikiInfoProperty> fullWikiInfoPropertylist = null;
        if (wikiList != null && wikiList.size() > 0) {
            wiki = wikiList.get(0);
            fullWikiInfoPropertylist = wikiInfoService.getFullWikiInfoService(wiki);
        }

        inv.addModel("wiki", wiki);
        inv.addModel("wikiInfoPropertylist", fullWikiInfoPropertylist);

        return "modify-wiki-info";
    }

    /**
     * 添加wiki的一个info资料
     * 
     * @param inv
     * @param name
     * @param value
     * @return
     */
    @Post("setWikiInfoProperty/${wikiId:\\d+}")
    public String addWikiInfoPropetry(Invocation inv, @Param("wikiId") int wikiId,
            @Param("name") String name, @Param("value") String value) {
        if (StringUtils.isBlank(name) || StringUtils.isBlank(value)) {
            return "@不能为空";
        }

        Wiki wiki = wikiHolder.getWiki();
        WikiType wikiType = WikiType.getEnum(wiki.getType());
        wikiInfoService.saveOrUpdateWikiInfoProperty(wikiId, name, value, wikiType);
        return "r:http://wiki.renren.com/adminwiki/modifyWikiInfo/" + wikiId;
    }

    /**
     * 删除wiki资料的某个属性
     * 
     * @param inv
     * @param wikiId
     * @param wikiInfoPropertyId
     * @return
     */
    @Get("deleteWikiInfoProperty/${wikiId:\\d+}/${wikiInfoPropertyId:\\d+}")
    public String deleteWikiInfoProperty(Invocation inv, @Param("wikiId") int wikiId,
            @Param("wikiInfoPropertyId") int wikiInfoPropertyId) {

        wikiInfoService.deleteWikiInfoPropertyById(wikiInfoPropertyId);
        return "r:http://wiki.renren.com/adminwiki/modifyWikiInfo/" + wikiId;
    }

    @Post("/set-wiki-info")
    public String setWikiInfoByText(Invocation inv, @Param("wikiId") int wikiId,
            @Param("info") String info) {

        //解析info
        String[] infos = info.split("[\r\n]{1,2}");
        if (infos != null && infos.length > 0) {
            for (String oneInfo : infos) {
                String[] oneInfos = oneInfo.split(":\\s*", 2);
                if(oneInfos.length==2){
                    String name = oneInfos[0];
                    String value = oneInfos[1];
                    Wiki wiki = wikiHolder.getWiki();
                    WikiType wikiType = WikiType.getEnum(wiki.getType());
                    wikiInfoService.saveOrUpdateWikiInfoProperty(wikiId, name, value, wikiType);
                }
            }
        }

        return "r:http://wiki.renren.com/" + wikiId;
    }

}
