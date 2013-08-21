package com.xiaonei.page.xoa.controllers;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.identify.Identifier;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.identify.IdentifyService;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONObject;
import net.sf.json.JsonConfig;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * 用于获取Page的认证信息
 * User: shunlong.jin@renren-inc.com
 * Date: 11-12-13
 * Time: 下午3:30
 */
@Path("getpageidentifier")
public class GetPageIdentifierController {
    private final static Logger logger = Logger.getLogger("PAGEXOA");
    @Autowired
    private PageService pageService;

    @Autowired
    private IdentifyService identifyService;

    @Get
    public String index(@Param("pageId") int pageId) {
        Page page = this.pageService.get(pageId);
        if (page == null) {
            return "@NO_PAGE";
        }
        boolean isIdentifier = this.identifyService.isPageChecked(page);
        if (isIdentifier) {
            try {
                Identifier identifier = this.identifyService.getAndCreateIfNotExist(page);
                if (identifier == null) {
                    return "@NO_IDENTIFIER";
                }
                identifier.setApplyDate(null);
                identifier.setIdentifyDate(null);
                return "@" + JSONObject.fromObject(identifier).toString();
            } catch (Exception e) {
                logger.error(e.getMessage(),e);
                return "@NO_IDENTIFIER";
            }
        }
        return "@NO_IDENTIFIER";
    }
}
