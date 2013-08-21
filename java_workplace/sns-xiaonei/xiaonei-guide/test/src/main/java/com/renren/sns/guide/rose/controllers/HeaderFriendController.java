package com.renren.sns.guide.rose.controllers;

import java.io.IOException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

@LoginRequired
@Path("headerFriend")
public class HeaderFriendController {

	 @Get
	 @Post
	 public String index(Invocation inv, Portal portal, CacheManager cm) {
		 User user=UserProvider.getInstance().get(cm);
		 Integer friendCount=FriendsHome.getInstance().getFriendCount(user);
		 if(!user.getUserState().isGuideDone() && friendCount<10){
			 try {
				inv.getResponse().sendRedirect("http://friend.renren.com/myfriendlistx.do#item_1");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		 }
		 else{
			 try {
				inv.getResponse().sendRedirect("http://friend.renren.com/myfriendlistx.do");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		 }
		 return "@";
	 }
}
