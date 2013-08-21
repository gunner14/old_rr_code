package com.xiaonei.recall_email.auto_login.logic;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.web.multipart.MultipartFile;

import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.recall_email.auto_login.dao.RecallKeyDAO;
import com.xiaonei.recall_email.auto_login.model.RecallEmailAutoLoginModel;

public class RecallKey {

    private static final RecallKey instance = new RecallKey();

    public static RecallKey getInstance() {
        return instance;
    }

    public void doLogin(final int userId, final HttpServletRequest request,
            final HttpServletResponse response) {
        //        LoginLogicFactory.getPassportTicketOpr().setTicket(
        //                PassportManager.getInstance().createTicket(userId, request), false, request,
        //                response);

        try {
            final User host = WUserAdapter.getInstance().get(userId);
            if (null != host) {
                BaseThreadUtil.setHost(request, host);
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        LoginLogicFactory.getPassportLogin().doTaskAfterLogin(request, userId);
    }

    public List<RecallEmailAutoLoginModel> getIdList(final MultipartFile file) {
        final List<RecallEmailAutoLoginModel> list = new ArrayList<RecallEmailAutoLoginModel>();
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(file.getInputStream()));
            String line = "";
            while ((line = br.readLine()) != null) {
                line = StringUtils.trimToEmpty(line);
                if (StringUtils.isNotEmpty(line) && StringUtils.isNumeric(line)) {
                    list.add(new RecallEmailAutoLoginModel().setUserid(Integer.valueOf(line)));
                }
            }
        } catch (final NumberFormatException e) {
            e.printStackTrace();
        } catch (final IOException e) {
            e.printStackTrace();
        } finally {
            if (null != br) {
                try {
                    br.close();
                } catch (final IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return list;
    }

    public String getRedirect(String redirect) {
        if (StringUtils.isEmpty(redirect)) {
            redirect = OpiConstants.urlMain.toString();
            //为空就直接返回
            return redirect;
        }
        //@TODO解析不出来怎么处理呢？
        try {
            final String redirect1 = URLDecoder.decode(redirect, "UTF-8");
            try {
                //为自动跳转添加白名单
                final URL redirectURL = new URL(redirect1);
                if (redirectURL.getHost().indexOf(OpiConstants.getCurrentDomain()) == -1) {
                    redirect = OpiConstants.urlMain.toString();
                }
            } catch (final MalformedURLException e) {
                return redirect;
            }
        } catch (final UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return redirect;
    }

    public boolean isValidKey(final String key, final RecallEmailAutoLoginModel model) {
        if (StringUtils.isEmpty(key)) {
            return false;
        }
        try {
            final RecallEmailAutoLoginModel model2 = RecallKeyDAO.getInstance().get(key);
            if (null == model2) {
                return false;
            }
            model.set(model2);
        } catch (final Exception e) {
            e.printStackTrace();
            return false;
        }
        if (model.isOverTime()) {
            return false;
        }
        final long days = (new Date().getTime() - model.getInsertTime().getTime())
                / (1000 * 60 * 60 * 24);
        if (days >= 7) {
            try {
                RecallKeyDAO.getInstance().updateOverTime(key);
            } catch (final SQLException e) {
                e.printStackTrace();
            }
            return false;
        }
        try {
            RecallKeyDAO.getInstance().updateOverTime(key);
        } catch (final SQLException e) {
            e.printStackTrace();
        }
        return true;
    }

    public void saveKey(final List<RecallEmailAutoLoginModel> list) {
        for (final RecallEmailAutoLoginModel model : list) {
            try {
                RecallKeyDAO.getInstance().insert(model);
            } catch (final SQLException e) {
                e.printStackTrace();
            }
        }
    }

    public void transferKey(final List<RecallEmailAutoLoginModel> list) {
        for (final RecallEmailAutoLoginModel model : list) {
            model.setLoginKey(UUID.randomUUID().toString());
        }

    }

}
