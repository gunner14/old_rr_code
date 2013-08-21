package com.xiaonei.page.xoa.util;

import org.apache.log4j.Logger;

import com.xiaonei.page.service.PageService;


public class PageIdChecker {

    private static PageService pageService=PageService.getInstance();
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");
    
    private final static PageIdChecker instance=new PageIdChecker();
    private PageIdChecker(){
    }
    public static PageIdChecker getInstance(){
        return instance;
    }
    
    public static boolean isPageDomainIn(int curHost){
        if(!pageService.isPageDomainId(curHost)){
            logger.info(String.format("this is not a pageid %s", curHost));
            return false;
        }
        return true;
    }

}
