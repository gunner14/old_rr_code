package com.xiaonei.passport.dbwrite;

public class AccountServiceFactory {

	public static AccountService getInstance(){
		return AccountServiceImpl.getInstance();
	}
}
