package com.xiaonei.passport.web.util;

import java.net.URL;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.admin.util.net.URLEncoder;

public class OrigUrlUtil {

    
    static public String getRlUrl(String errorUrl, int errCode, String queryString) {
        StringBuffer sb =new StringBuffer(errorUrl);
        if(StringUtils.isEmpty(queryString)){
            sb.append("?");
        }else{
            sb.append("?");
            sb.append(queryString);
            sb.append("&");
        }
        sb.append("errCode=");
        sb.append(errCode);
        return sb.toString();
        
    }
	/**
	 * 拼接url
	 * @param count
	 * @param errorUrl
	 * @param origUrl
	 * @param code
	 * @param email
	 * @param loginRegistrationId
	 * @return
	 */
    static public String getUrl( int count, String errorUrl, String origUrl, int code, String email, int loginRegistrationId, String queryString){
		StringBuffer sb =new StringBuffer(errorUrl);
		if(StringUtils.isEmpty(queryString)){
			sb.append("?");
		}else{
			sb.append("?");
			sb.append(queryString);
			sb.append("&");
		}
		//sb.append("loginRegistrationId=");
		//sb.append(loginRegistrationId);
		sb.append("&");
		sb.append("origURL=");
		sb.append(LoginContextUtil.getOrigUrlEncode(origUrl));
		return appendOrigUrl(sb.toString(), email, code, count);
	}
    
    static private String appendOrigUrl(String initUrl,String email,
			int code, int catchaCount){
		StringBuilder sb=new StringBuilder(initUrl);
		boolean isFirst=true;
		if(email==null){
		    return sb.toString();
		}
		isFirst=append(sb, "catchaCount",String.valueOf(catchaCount), isFirst, false);
		isFirst=append(sb,"failCode", String.valueOf(code), isFirst, false);
		String url= sb.toString();
		return url;
	}
	
    static private boolean append(StringBuilder sb, String name, String str, boolean isFirst, boolean needEncode){
		if(!StringUtils.isEmpty(str)){
			String tmp=str;
			if(needEncode){
				tmp=URLEncoder.encodeUtf8(str);
			}
			tmp=name+"="+tmp;
			if(isFirst){
				sb.append("?"+tmp);
				isFirst=false;
			}else{
				sb.append("&"+tmp);
			}
		}
		return isFirst;
	}
    
    public static String changeOrigUrl(String origUrl, int destId) {
        try {
            java.net.URL url=new URL(origUrl);
            String queryString=url.getQuery();
            String protol=url.getProtocol();
            String condition =  getIdParaQuery(queryString);
            if (!StringUtils.isEmpty(condition)) {
                queryString =  removeKeyFromQueryString(queryString, condition);
            }
            String query = StringUtils.isEmpty(queryString) ? "" : "?"
                    + queryString;
            
            
            return protol + "://www.renren.com/" + destId + query;
        }catch(Exception e){
            return origUrl;
        } 
    }
    
    /**
     * 从现有的url中移出key=value
     * 
     * @param queryString
     * @param id
     * @return
     */
    private  static String removeKeyFromQueryString(String queryString,
            String queryCondition) {
        try{
            if(StringUtils.isEmpty(queryString)||StringUtils.isEmpty(queryCondition)){
                return queryString;
            }
            String[] queryConditions=queryString.split("&");
            StringBuffer sb=new StringBuffer();
            boolean start=true;
            for(String condition: queryConditions){
                if(!condition.equals(queryCondition)){
                    if(start){
                        start=false;
                    }else{
                        sb.append("&");
                    }
                    sb.append(condition);
                }
            }
            return sb.toString();
        }catch(Exception e){
            return queryString;
        }
        
    }
    
    private  static String getIdParaQuery(String queryString) {
        return getKeyParaQuery(queryString, "id");
    }
    
    private  static String getKeyParaQuery(String queryString, String key) {
        try{
            if(StringUtils.isEmpty(queryString)){
                return null;
            }
            String[] queryCondition=queryString.split("&");
            for(String condition:queryCondition){
                if(condition.startsWith(key+"=")){
                    return condition;
                }
            }
        }catch(Exception e){
            return null;
        }
        
        return null;
    }
    
}
