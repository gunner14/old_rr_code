package com.xiaonei.login.logic.validator.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import com.xiaonei.login.logic.validator.enumeration.ValidatorEnum;

/**
 * FormValidator <br>
 * 
 * @author tai.wang@opi-corp.com Jan 13, 2010 - 6:58:24 PM
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.PARAMETER)
public @interface FormValidator {

	boolean validateAllParams() default true;

	ValidatorEnum[] params() default {};

}
