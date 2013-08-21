package com.xiaonei.login.daqin.controllers;

import java.lang.reflect.Method;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ParamValidator;
import net.paoding.rose.web.paramresolver.ParamMetaData;

import org.springframework.validation.Errors;

public class TsigninParamValidator implements ParamValidator {

    @Override
    public boolean supports(final ParamMetaData metaData) {
        Method supportMethod = null;
        try {
            supportMethod = TransferSigninController.class.getMethod("post", Invocation.class,
                    String.class, String.class, String.class, String.class);
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return (metaData.getControllerClass() == TransferSigninController.class)
                && (metaData.getMethod() == supportMethod);
    }

    @Override
    public Object validate(final ParamMetaData metaData, final Invocation inv, final Object target,
            final Errors errors) {
        return null;
    }

}
