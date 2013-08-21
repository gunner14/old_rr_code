package com.xiaonei.commons.interceptors.userinterface;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.IfParamExists;
import net.paoding.rose.web.annotation.Interceptor;
import net.paoding.rose.web.var.Model;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.MarkAsFromWap;
import com.xiaonei.commons.interceptors.unifiedentrance.CheckUnifiedEntrance;
import com.xiaonei.platform.component.application.menu.home.UserMenuGuideHome;
import com.xiaonei.platform.component.application.menu.model.UserMenuGuide;
import com.xiaonei.platform.component.application.menu.utils.MenuUtils;
import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFetherImpl;
import com.xiaonei.sns.platform.core.rose.vsix.V6UserIdsManager;

/**
 * {@link MenuInterceptor} 拦截对Web控制器的调用，取与导航相关的数据
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class MenuInterceptor extends ControllerInterceptorAdapter {
	private static Logger logger = LoggerFactory.getLogger(MenuInterceptor.class);
	/**
	 * ip提取工具
	 */
	private final RemoteAddrFether remoteAddrFether = new RemoteAddrFetherImpl();

	@Autowired
	private HostHolder hostHolder;

	@Autowired
	private V6UserIdsManager v6UserIdsManager;

	@Override
	protected Class<? extends Annotation> getDenyAnnotationClass() {
		return Ajax.class;
	}

	@Override
	protected boolean isForAction(Method actionMethod, Class<?> controllerClazz) {
		// async的访问没有navigation
		if (actionMethod.isAnnotationPresent(IfParamExists.class)) {
			IfParamExists ifParamExists = actionMethod.getAnnotation(IfParamExists.class);
			if (ifParamExists.value().indexOf(CheckUnifiedEntrance.async) != -1
					|| ifParamExists.value().indexOf(CheckUnifiedEntrance.asyncReload) != -1) {
				return false;
			}
		}
		// wap的访问没有navigation
		MarkAsFromWap markAsFromWap = actionMethod.getAnnotation(MarkAsFromWap.class);
		if (markAsFromWap == null) {
			markAsFromWap = controllerClazz.getAnnotation(MarkAsFromWap.class);
		}
		if (markAsFromWap != null) {
			return !markAsFromWap.value();
		}
		//
		return true;
	}

	@Override
	public Object before(Invocation inv) throws Exception {
		User host = hostHolder.getUser();
		if (host == null) {
			return true;
		}
		Model model = inv.getModel();
		
		boolean isMenuVisited = UserMenuGuideHome.getInstance().isMenuVisited(host.getId());
		inv.addModel("isMenuVisited", isMenuVisited);
		
		if (v6UserIdsManager.isV6User(host)) {
			String ip = remoteAddrFether.getRemoteAddr(inv.getRequest());
			boolean isGuide = isGuideUser(host);
			model.add("userMenu",UserMenuManager.getInstance().getNewMenuList(host.getId(),ip,isGuide));
			model.add("isOldVersion",MenuUtils.isOldVersion(host.getId(),isGuide));
		} else {
			model.add("userMenu", UserMenuManager.getInstance().get(host.getId()));
		}
		return true;
	}
	
	
	private boolean isGuideUser(User host){
		UserState state = host.getUserState();
		if(state == null){
			logger.error("user state not found ,uid:"+host.getId());
			return false;
		}
		if( state.isGuideDone()){
			return false;
		}else{
			return true;
		}
	}
}
