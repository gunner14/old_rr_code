package com.xiaonei.login.daqin.services;

import java.util.List;

import org.springframework.stereotype.Service;

import com.xiaonei.xce.passport.Account;

@Service
public interface DaqinService {

    public String getSigninKey(final String domain);

    String getCallbackUrl(String domain);

    String getReturnJson(String domain, List<Account> accounts, String url);
}
