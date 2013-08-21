package com.xiaonei.page.xoa.controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.util.Util;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
/**
 * 
 * 怎么注入service。
 * @author happyshelocks@gmail.com
 *
 */

public class CheckerUtil {
    
    private static PageService pageService=PageService.getInstance();
    
    private static final CheckerUtil instance=new CheckerUtil();
    private CheckerUtil(){
        
    }

    public static CheckerUtil getInstance(){
        return instance;
    }
    
    public PageHostPair checkPageAndHost(int pageId, int curHost) {
        if(!Util.isOriPageId(pageId)){
            return new PageHostPair(null, null, "@pageid-illegal");
        }
        Page page = pageService.get(pageId);
        if (page == null) {
            return new PageHostPair(null, null,"@page-not-exist");
        }
        User host = WUserAdapter.getInstance().get(curHost);
        if (host == null) {
            return new PageHostPair(null, null,"@host-not-exist");
        }
        return new PageHostPair(page, host, "");
    }
}
