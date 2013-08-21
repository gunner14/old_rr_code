package com.renren.sns.wiki.controllers.adminwiki;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.wiki.dao.adminwiki.WikiUserIdDAO;

/**
 * wiki白名单管理
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-13
 * 
 */
@Path("wikiWhite")
public class WikiWhiteController {

    @Get("addWhiteUser")
    @Post("addWhiteUser")
    public String addUser(Invocation inv, @Param("userId") int userId) {

        inv.addModel("navName", "wikiWhite");

        String result = "";
        if (userId > 0) {
            WikiUserIdDAO wikiUserIdDAO = WikiUserIdDAO.getInstance();
            int uid = wikiUserIdDAO.getUserIdByUserId(userId);
            if (uid > 0) {
                result = userId + "已存在";
            } else {
                int insertCount = wikiUserIdDAO.insert(userId, 1);
                if (insertCount > 0) {
                    result = userId + "添加成功";
                } else {
                    result = userId + "添加失败";
                }
            }
        } else {
            result = userId + "格式不对";
        }
        inv.addModel("result", result);
        return "wiki-white";
    }

    @Get("")
    public String wikiWhite(Invocation inv) {
        inv.addModel("navName", "wikiWhite");
        inv.addModel("result", null);
        return "wiki-white";
    }

}
