package com.renren.sns.wiki.utils;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.platform.core.model.User;

/**
 * 
 * 返回用户信息格式的转化
 * 
 * @author weiwei.wang@renren-inc.com since 2012-6-19
 * 
 */
public class UserUtils {

    /**
     * 包含user的id name 和 头像信息,可使用工具转换为json使用
     * 
     * @param user
     * @return
     */
    public static Map<String, Object> convertToSimpleMap(User user) {

        Map<String, Object> map = new HashMap<String, Object>();

        Map<String, Object> userNameInfoMap = new HashMap<String, Object>();
        userNameInfoMap.put("id", user.getUserNameInfo().getId());
        userNameInfoMap.put("name", user.getUserNameInfo().getName());

        Map<String, Object> userUrlMap = new HashMap<String, Object>();
        userUrlMap.put("tinyUrl", user.getUserUrl().getTinyUrl());
        userUrlMap.put("headUrl", user.getUserUrl().getHeadUrl());
        userUrlMap.put("mainUrl", user.getUserUrl().getMainUrl());
        userUrlMap.put("largeUrl", user.getUserUrl().getLargeUrl());

        map.put("userName", userNameInfoMap);
        map.put("userUrl", userUrlMap);

        return map;
    }

}
