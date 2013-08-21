package com.xiaonei.sns.platform.core.rose.interceptors;

import java.lang.annotation.Annotation;

import com.xiaonei.platform.core.control.TimeCostUtil;

import net.paoding.rose.web.ControllerInterceptor;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.PortalUtils;
import net.paoding.rose.web.portal.Window;

/**
 * a {@link ControllerInterceptor} which merges all the model of window to the
 * portal
 * 
 * @author xylz(xylz@live.cn)
 * @since 20091106
 * @see {@link Window}
 * @see {@link ControllerInterceptor}
 * @see {@link Portal}
 * 
 */
public class MergeWindowModelInterceptor extends ControllerInterceptorAdapter {
	public MergeWindowModelInterceptor() {
		setPriority(MergeWindowModel.PERFORMANCE_PRIV);
	}

	@Override
	protected Class<? extends Annotation> getRequiredAnnotationClass() {
		return MergeWindowModel.class;
	}

	@SuppressWarnings("deprecation")
	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		Window window = PortalUtils.getWindow(inv);
		if (window != null && instruction == null) {
			synchronized (inv.getModel()) {
				if(TimeCostUtil.logger.isDebugEnabled())
					TimeCostUtil.logger.debug("merge portal data:"+window.getName()+", path:"+window.getPath());
				window.getPortal().getInvocation().getModel().merge(
						inv.getModel().getAttributes());
			}
			return "";
		}
		return instruction;

	}
}
