package com.renren.sns.guide.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.Window;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path("tnf")
public class TransmitNewsFeedController {
	
	@Autowired
    private HostHolder hostHolder;
	
	@Get
    public String index(Invocation inv, Portal portal, CacheManager cm) {
		User host = null;
        try {
            host = this.hostHolder.getUser();
            if (host == null) {
                throw new Exception("get host error");
            }
        } catch (Exception e) {
            e.printStackTrace();            
            System.err.println("get host error");          
            return "@-1";
        }
        portal.setTimeout(700);//超时700ms就切除
		try {
			if(host.getId() % 2 ==0 ){
				GuideLogger.printLog("Transmit Hot Share");
				addWindow(portal, "window_transmitbloglist", "transmitBlogList");
				addWindow(portal, "window_transmitphotolist", "transmitPhotoList");
				addWindow(portal, "window_transmitvideolist", "transmitVideoList");
			}
			else{
				GuideLogger.printLog("Transmit City Feeds");
				addWindow(portal, "window_transmitcityfeeds", "transmitCityFeeds");
			}
				
		} catch (Exception e) {
			e.printStackTrace();
		}
        return "@";
	}
	
	private void addWindow(Portal portal, String name, String path) {
		try {
			Window window = portal.addWindow(name, "/window/guideWindows/" + path);
			window.set(Window.FUTURE_CANCEL_ENABLE_ATTR, false);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
