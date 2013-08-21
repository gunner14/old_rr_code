package com.dodoyo.opt;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import com.xiaonei.reg.common.constants.Globals;

/**
 * Initiate.java Created by
 * 
 * @author freebat (freebat@gmail.com) at 2006-2-17 19:38:14
 */
public class Initiate implements ServletContextListener {

	public void contextInitialized(ServletContextEvent arg0) {
		DodoyoResource.init();
		DodoyoResource.init("pageids");
		DodoyoResource.init("check-rules");
		setGlobalsContextRoot(arg0);
	}

	public void contextDestroyed(ServletContextEvent arg0) {
	}
	
	public static void setGlobalsContextRoot(ServletContextEvent arg0){
		Globals.contextRoot = arg0.getServletContext().getRealPath("/");
		com.kaixin.Globals.contextRoot = arg0.getServletContext().getRealPath("/");
    }
}
