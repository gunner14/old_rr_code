package com.renren.litetest.controllers;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-13 下午05:19:04
 */
@Path("")
public class WelcomeController {

	@Get("welcome")
	public Object showWelcome() {
		return "@Welcome";
	}
	
}
