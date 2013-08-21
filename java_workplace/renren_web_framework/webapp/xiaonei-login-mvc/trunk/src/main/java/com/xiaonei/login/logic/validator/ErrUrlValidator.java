package com.xiaonei.login.logic.validator;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.Properties;
import java.util.Map.Entry;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.validation.Errors;

import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;

@Singleton
public class ErrUrlValidator implements PpParamValidator {

    @Override
    public Object validate(Invocation inv, PassportForm pForm, Errors errors) {
        if (StringUtils.isEmpty(pForm.getErrURL())) {
            return null;
        }
        if (!isURLMatch(pForm.getErrURL())) {
            pForm.setErrURL("http://www.renren.com/home");
        }
        return null;
    }

    private boolean isURLMatch(String sUrl) {
        if(StringUtils.contains(sUrl, "%0a")){
            return false;
        }
        if(StringUtils.contains(sUrl, "%0A")){
            return false;
        }
        URL url = getURL(sUrl);
        if (url == null) {
            return false;
        }
        Properties p = new Properties();
//        InputStream inStream = ErrUrlValidator.class.getClassLoader().getResourceAsStream("origurl-list.properties");
//        try {
//            p.load(inStream);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
        p.put("kaixin.com", "w");
        p.put("renren.com", "w");
        boolean isMatch = false;
        for (Entry<Object, Object> entry : p.entrySet()) {
            String key = entry.getKey().toString();
            String value = entry.getValue().toString();
            if ("w".equals(value)) {
                if (url != null && url.getHost().endsWith(key)) {
                    isMatch = true;
                    break;
                }
            }
        }
        return isMatch;
    }

    private URL getURL(String origUrl) {
        URL url;
        try {
            url = new URL(origUrl);
        } catch (MalformedURLException e) {
            return null;
        }
        return url;
    }

}
