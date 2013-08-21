package com.xiaonei.passport.passport;

public class PassportServiceFactory {
	public static IPassportService getInstance(){
		return PassportServiceImpl.getInstance();
	}
}
