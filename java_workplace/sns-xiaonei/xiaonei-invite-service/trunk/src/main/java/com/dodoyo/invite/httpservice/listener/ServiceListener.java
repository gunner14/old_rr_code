package com.dodoyo.invite.httpservice.listener;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

public class ServiceListener implements ServletContextListener
{

	public void contextInitialized(ServletContextEvent sc) 
	{
//		com.dodoyo.invite.core.Constants.WEB_APP_PATH = sc.getServletContext().getRealPath("/");
//		System.out.println("In ServiceListener, WEB_APP_PATH been inited as: " + com.dodoyo.invite.core.Constants.WEB_APP_PATH);
	}

	public void contextDestroyed(ServletContextEvent arg0) 
	{
	}

}