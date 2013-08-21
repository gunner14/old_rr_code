package com.xiaonei.reg.register.controllers.sb;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;
import net.paoding.rose.web.paramresolver.ServletRequestDataBinder;

import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.form.base.RegForm;

public class RegFormResolver implements ParamResolver {

	@Override
	public Object resolve(Invocation inv, ParamMetaData metaData)
			throws Exception {
		BaseRegSubmitController controller = (BaseRegSubmitController) inv
				.getController();
		Object bean = controller.getFormClazz().newInstance();
		ServletRequestDataBinder binder = new ServletRequestDataBinder(bean);
		binder.bind(inv.getRequest());
		return bean;
	}

	@Override
	public boolean supports(ParamMetaData metaData) {
		boolean rightController = BaseRegSubmitController.class
				.isAssignableFrom(metaData.getControllerClass());
		if (rightController) {
			return metaData.getParamType().equals(RegForm.class);
		}
		return false;
	}

}
