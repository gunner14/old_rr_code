package com.xiaonei.login.logic.autologin;

import java.util.List;

import com.xiaonei.xce.passport.Account;

public interface AutoLoginEmailLogic {

	public boolean isExistEmail(String email, List<Account> account);

	public void sendAutoLoginEmail(String email, String domain);

}
