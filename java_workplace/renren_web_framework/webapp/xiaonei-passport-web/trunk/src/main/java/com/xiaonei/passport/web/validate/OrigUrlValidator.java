package com.xiaonei.passport.web.validate;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.adapt.LoginConfigureImpl;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.websecurity.adapter.SafeFilter;

@Service
public class OrigUrlValidator implements PpParamValidator {
	protected Log logger = LogFactory.getLog(this.getClass());
    
    @Override
    public LoginStatusCode validate(PassportForm pForm) {
    	String email=null;
    	try{
    		email=pForm.getEmail();
    		 if (StringUtils.isEmpty(pForm.getOrigURL())) {
    	            return LoginStatusCode.LOGIN_OK;
    	        }
    	        if (!isURLMatch(pForm.getOrigURL())) {
    	            pForm.setOrigURL("http://www.renren.com/home");
    	        }
    	        String origUrl=SafeFilter.getInstance().getRichContent(pForm.getOrigURL());
    	        pForm.setOrigURL(origUrl);
    	        return LoginStatusCode.LOGIN_OK;
    	}catch(Exception e){
    		logger.error("origurl valdation error , the email is "+email, e);
    		return LoginStatusCode.LOGIN_OK;
    	}
       
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
//	    List<String> validationDomains=LoginConfigureImpl.getLocalInstance().getLoginConfigure(0, LoginConfigureKey.VALIDATION_DOMAIN.getKey());
//       if(validationDomains.contains(getDomainFromUrl(url.toString()))){
//    	   return true;
//       }
//        return false;
        Properties p = new Properties();
//      InputStream inStream = OrigUrlValidator.class.getClassLoader()
//              .getResourceAsStream("origurl-list.properties");
//      try {
//          p.load(inStream);
//      } catch (Exception e) {
//          e.printStackTrace();
//      }
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
    
    /**
     * get the domain from a url string 
     * eg. http://aa.bb.cc.dd.com/ee/ff?....  the domain is dd.com
     * @param url
     * @return
     */
//    private String getDomainFromUrl(String url){
//    	Pattern pattern = Pattern.compile("^(http://)(\\w*\\.){0,}(\\w*\\.com).*");
//		Matcher matcher = pattern.matcher(url);
//		while(matcher.find()){
//			return matcher.group(3);
//		}
//		return "";
//    }

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
