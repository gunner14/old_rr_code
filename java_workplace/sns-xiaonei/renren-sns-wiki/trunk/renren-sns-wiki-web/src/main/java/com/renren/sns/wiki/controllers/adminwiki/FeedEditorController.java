package com.renren.sns.wiki.controllers.adminwiki;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.service.admin.AdminFeedEditorService;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * Feed运维工具
 * 
 * @author yi.li@renren-inc.com since Aug 6, 2012
 * 
 */
@Path("${wikiId:\\d+}/fdeditor")
public class FeedEditorController {

    @Autowired
    private AdminFeedEditorService adminFeedEditorService;

    @Get("")
    @Post("")
    public String index(@Param("wikiId") int wikiId) {
        return "feed-editor";
    }

    @Post("del")
    public String deleteHotFeedsByKeyWords(Invocation inv, @Param("wikiId") int wikiId,
            @Param("kw") String keyWords) {
        keyWords = keyWords.trim();
        if (StringUtils.isEmpty(keyWords)) {
            inv.addModel("msg", "查询条件不能为空");
            return "f:/adminwiki/" + wikiId + "/fdeditor";
        }

        adminFeedEditorService.deleteHotFeedByKeyWords(wikiId, keyWords);
        inv.addModel("msg", "任务已提交，稍等5-60秒后刷新wiki页面查看结果;-)");

        return "f:/adminwiki/" + wikiId + "/fdeditor";
    }
}
