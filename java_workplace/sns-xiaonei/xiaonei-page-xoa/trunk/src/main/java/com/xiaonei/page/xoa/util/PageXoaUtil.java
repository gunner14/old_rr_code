package com.xiaonei.page.xoa.util;

import java.io.IOException;

import org.apache.log4j.Logger;
import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

public class PageXoaUtil {

    private PageService pageService = PageService.getInstance();

    private AuthService authService = AuthService.getInstance();

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    private static final PageXoaUtil instance = new PageXoaUtil();

    private PageXoaUtil() {

    }

    public static final PageXoaUtil getInstance() {
        return instance;
    }

    public String getJsonString(ObjectMapper mapper, Object obj) {
        try {
            return mapper.writeValueAsString(obj);
        } catch (JsonGenerationException e) {
            e.printStackTrace();
        } catch (JsonMappingException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public JSONObject getJsonRs(int code, Object msg) {
        JSONObject json = new JSONObject();
        json.put("code", code);
        json.put("msg", msg);
        return json;
    }

    /**
     * 
     * 封装获取一个page的逻辑。原因在于这段代码太常用。
     * 
     * @param pageId
     * @return 成功，返回page对象；失败，返回null
     */
    public Page getPage(int pageId) {
        Page page = null;
        try {
            page = pageService.get(pageId);
        } catch (PageException e) {
            logger.error(e.getMessage(), e);
        }
        return page;
    }

    /**
     * 
     * 封装获取一个User的逻辑。原因在于这段代码太常用。
     * 
     * @param userId
     * @return 成功，返回page对象；失败，返回null
     */
    public User getUser(int userId) {
        User host = null;
        try {
            host = WUserAdapter.getInstance().get(userId);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
        return host;
    }

    /**
     * 
     * 权限验证。原因在于这段代码太常用。
     * 
     * @param userId
     * @param pageId
     * @return 成功，返回true；失败，false
     */
    public boolean authUserAndPage(int userId, int pageId) {
        try {
            if (!(authService.isPageAdministrator(userId, pageId))) {
                return false;
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return false;
        }

        return true;
    }
}
