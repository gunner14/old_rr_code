package com.xiaonei.login.controllers.ajax;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.security.login.toolkit.encrypt.EncryptAdapter;

@Path(value = { "getEncryptKey" })
@Singleton
@Ajax
public class EncryptKeyController {
	
	private EncryptAdapter ea = new EncryptAdapter();

	@Get
	public String getEncryptKey() {
		return "@" + ea.generateJSONRSAComponent();
	}
}
