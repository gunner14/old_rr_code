package com.xiaonei.login.daqin.services;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.List;

import net.sf.json.JSONObject;

import org.springframework.stereotype.Service;

import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.xce.passport.Account;

@Service
public class DaqinServiceImpl implements DaqinService {

    @Override
    public String getCallbackUrl(final String domain) {
        if ("nuomi.com".equals(domain)) {
            return "http://www.nuomi.com/renren/callback";
        } else if ("fengche.com".equals(domain)) {
            return "http://www.fengche.com/renren/callback";
        } else if ("56.com".equals(domain)) {
            return "http://user.56.com/login/callback";
        }
        return null;
    }

    @Override
    public String getReturnJson(final String domain, final List<Account> accounts, final String url) {
        final int time = (int) (System.currentTimeMillis() / 1000);
        final StringBuffer str = new StringBuffer();
        str.append(time);
        for (final Account account : accounts) {
            str.append("-");
            str.append(account.getId());
            str.append("-");
            str.append(account.getDomain());
            str.append("-");
            str.append(account.getStatus());
            if ("56.com".equals(account.getDomain())) {
                str.append("-");
                str.append(account.getAccount());
            }
        }
        str.append("-");
        str.append(this.getSigninKey(domain));
        final String hashcode = Encoder.encode(str.toString());
        final StringBuffer psb = new StringBuffer();
        psb.append(time).append(hashcode);
        for (final Account account : accounts) {
            psb.append(",");
            psb.append(account.getId());
            psb.append("-");
            psb.append(account.getDomain());
            psb.append("-");
            psb.append(account.getStatus());
            if ("56.com".equals(account.getDomain())) {
                psb.append("-");
                psb.append(account.getAccount());
            }
        }
        final JSONObject jo = new JSONObject();
        jo.put("p", psb.toString());
        jo.put("u", url);
        try {
            return URLEncoder.encode(jo.toString(), "UTF-8");
        } catch (final UnsupportedEncodingException e) {
            e.printStackTrace();
            return jo.toString();
        }
    }

    @Override
    public String getSigninKey(final String domain) {
        if ("nuomi.com".equals(domain)) {
            return "305880804";
        } else if ("fengche.com".equals(domain)) {
            return "QingTing";
        } else if ("56.com".equals(domain)) {
            return "5556666";
        }
        return "@";
    }
}
