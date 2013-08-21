package com.renren.sns.wiki.controllers.adminwiki;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;

import com.renren.sns.wiki.service.admin.utils.HttpToolkit;

@Path("/douban")
public class DoubanController {

    @Get("/get-info")
    @Post("/get-info")
    public String getDoubanInfo(Invocation inv, @Param("wikiId") int wikiId,
            @Param("doubanUrl") String doubanUrl) {

        String info = "";

        if (doubanUrl != null && doubanUrl.startsWith("http")) {
            String url = doubanUrl;

            String res = new HttpToolkit().get(url);

            Document doc = Jsoup.parse(res);
            Element infoEle = doc.getElementById("info");
            String infoHtml = infoEle.html();

            String[] infoHtmls = infoHtml.split("<br />");
            StringBuilder sb = new StringBuilder();
            for (String singleInfoHtml : infoHtmls) {
                sb.append(Jsoup.parse(singleInfoHtml).text()).append("\n");
            }
            info = sb.toString();
        }

        inv.addModel("wikiId", wikiId);
        inv.addModel("info", info);
        return "douban/douban-info";
    }

}
