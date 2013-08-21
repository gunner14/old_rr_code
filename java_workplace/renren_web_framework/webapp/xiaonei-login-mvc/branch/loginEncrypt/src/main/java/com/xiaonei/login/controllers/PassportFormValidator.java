package com.xiaonei.login.controllers;

import java.lang.annotation.Annotation;
import java.util.Arrays;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ParamValidator;
import net.paoding.rose.web.paramresolver.ParamMetaData;

import org.apache.commons.lang.ArrayUtils;
import org.springframework.validation.Errors;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.ObjectMaker;
import com.xiaonei.login.logic.validator.PpParamValidator;
import com.xiaonei.login.logic.validator.annotation.FormValidator;
import com.xiaonei.login.logic.validator.enumeration.ValidatorEnum;

public class PassportFormValidator implements ParamValidator {

	@Override
	public boolean supports(final ParamMetaData metaData) {
		LoginLoggerDef.login.trace("in validate support ");
		if (!PassportForm.class.equals(metaData.getParamType())) {
			return false;
		}
		LoginLoggerDef.login.trace("in validate is annotation");
		Annotation anno = getAnnotation(metaData, FormValidator.class);
		if (null == anno) {
			return false;
		}
		metaData.setUserObject("annotation", anno);
		LoginLoggerDef.login.trace("in validate return true"
				+ metaData.getControllerClass().getSimpleName() + "."
				+ metaData.getMethod().getName());
		return true;
	}

	private Annotation getAnnotation(ParamMetaData metaData, Class<?> class1) {
		final Annotation[] annos = metaData.getMethod()
				.getParameterAnnotations()[metaData.getIndex()];
		for (final Annotation anno : annos) {
			if (class1.isInstance(anno)) {
				return anno;
			}
		}
		return null;
	}

	private final String loginStatus = LoginStatusCode.class.getSimpleName();

	@Override
	public Object validate(final ParamMetaData metaData, final Invocation inv,
			final Object target, final Errors errors) {
		LoginLoggerDef.login.trace("in validate");

		LoginLoggerDef.login.trace("target:" + target);
		if (null == target) {
			inv.setAttribute(loginStatus, LoginStatusCode.FAILUNKNOWN);
			return null;
		}

		final PassportForm pForm = (PassportForm) target;

		// final FormValidator formAnn =
		// pForm.getClass().getAnnotation(FormValidator.class);
		final FormValidator formAnn = (FormValidator) metaData
				.getUserObject("annotation");
		LoginLoggerDef.login.trace("formAnn:" + formAnn);
		if (null == formAnn) {
			inv.setAttribute(loginStatus, LoginStatusCode.FAILUNKNOWN);
			return null;
		}

		final ValidatorEnum[] valiArray;
		LoginLoggerDef.login.trace("formAnn.validateAllParams():"
				+ formAnn.validateAllParams());
		LoginLoggerDef.login.trace("ArrayUtils.isEmpty(formAnn.params()):"
				+ ArrayUtils.isEmpty(formAnn.params()));
		if (formAnn.validateAllParams() && ArrayUtils.isEmpty(formAnn.params())) {
			valiArray = ValidatorEnum.values();
		} else {
			valiArray = formAnn.params();
		}
		LoginLoggerDef.login.trace("valiArray:"
				+ Arrays.asList(valiArray).toString());

		for (final ValidatorEnum param : valiArray) {
			LoginLoggerDef.login.trace("param.getValidatorClazz():"
					+ param.getValidatorClazz());
			if (null == param.getValidatorClazz()) {
				continue;
			}
			final PpParamValidator va = (PpParamValidator) ObjectMaker
					.getInstance(param.getValidatorClazz());
			LoginLoggerDef.login.trace("va:" + va);
			if (null == va) {
				continue;
			}
			LoginLoggerDef.login.trace("va.validate(inv, pForm, errors):%s", va
					.validate(inv, pForm, errors));
			if (Boolean.valueOf(false).equals(va.validate(inv, pForm, errors))) {
				return null;
			}
		}

		inv.setAttribute(loginStatus, LoginStatusCode.LOGIN_OK);
		return null;
	}

}
