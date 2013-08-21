package com.xiaonei.platform.taglib;

import java.util.Date;
import java.util.Timer;

import com.xiaonei.platform.taglib.task.FooterTagTask;
import com.xiaonei.platform.taglib.task.HeadTagTask;
import com.xiaonei.platform.taglib.task.NavigationTagTask;
import com.xiaonei.platform.taglib.task.SidebarTagTask;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class TaglibInit {

	public static void init() {
//		Timer timer = new Timer(false);
//		timer.schedule(new FooterTagTask(), new Date(
//				System.currentTimeMillis() + 1000), 1000 * 60 * 30);
//		timer.schedule(new HeaderTagTask(), new Date(
//				System.currentTimeMillis() + 1000), 1000 * 60 * 30);
//		timer.schedule(new SidebarTagTask(), new Date(System
//				.currentTimeMillis() + 1000), 1000 * 60 * 1);
//		timer.schedule(new NavigationTagTask(), new Date(System
//				.currentTimeMillis() + 1000), 1000 * 60 * 30);
//		timer.schedule(new HeadTagTask(), new Date(System
//				.currentTimeMillis() + 1000), 1000 * 60 * 30);		
	}

	public static void main(String[] arg){
		init();
	}
}