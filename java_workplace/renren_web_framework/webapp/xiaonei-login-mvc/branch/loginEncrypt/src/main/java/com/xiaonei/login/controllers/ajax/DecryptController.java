package com.xiaonei.login.controllers.ajax;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.security.login.toolkit.encrypt.EncryptAdapter;

@Path(value = { "decrypt" })
@Singleton
@Ajax
public class DecryptController {
	
	private EncryptAdapter ea = new EncryptAdapter();

	@Get
	public String decrypt(final Invocation inv, @Param("password") String password, @Param("rkey") String rkey) {
		return "@" + ea.decryptForWeb(password, rkey);
	}
}
