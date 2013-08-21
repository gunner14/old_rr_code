package com.xiaonei.passport.email;

import java.util.List;

import com.xiaonei.passport.model.Account;

public interface AutoLoginEmailLogic {

	public boolean isExistEmail(String email, List<Account> account);

	public void sendAutoLoginEmail(String email, String domain);

}
