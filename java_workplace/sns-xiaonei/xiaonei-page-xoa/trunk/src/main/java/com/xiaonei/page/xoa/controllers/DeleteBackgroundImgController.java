package com.xiaonei.page.xoa.controllers;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageSkin;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.skin.PortalSkinHistoryService;
import com.xiaonei.page.service.skin.PortalSkinService;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * 用于安全删除非法的背景图片
 * User: shunlong.jin@renren-inc.com
 * Date: 11-11-29
 * Time: 下午6:02
 */
@Path("dbi")
public class DeleteBackgroundImgController {
    private final static Logger logger = Logger.getLogger("PAGEXOA");
    @Autowired
    private PageService pageService;
    @Autowired
    private PortalSkinService skinService;

    @Autowired
    private PortalSkinHistoryService psHistoryService;

    @Get
    public String index(@Param("pageId") int pageId) {
        Page page;
        logger.info("Audit delete page bkimg " + pageId);
        try {
            page = pageService.get(pageId);
            if (page != null) {
                PageSkin ps = skinService.getCustomTheme(pageId);
                int skinId = ps.getId();
                if (skinId != 0) {
                    String jsonSkin = ps.getContent();
                    if (!StringUtils.isEmpty(jsonSkin)) {
                        JSONObject jsonObject = (JSONObject) new JSONParser().parse(jsonSkin);
                        if (jsonObject != null) {
                            Object bgImgObj = jsonObject.get("bgImg");
                            if (bgImgObj != null) {
                                jsonObject.put("bgImg", "");
                                ps.setContent(jsonObject.toJSONString());
                                skinService.updateSkin(skinId, ps);
                                psHistoryService.setOrChangeSkin(pageId, skinId);
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
            logger.error("Audit delete page bkimg error "+ e.getMessage(), e);
            return "@OTHER_ERROR";
        }
        return "@true";
    }
}
