package com.xiaonei.reg.register.logic.pretreatment;

import org.apache.commons.lang.StringUtils;

import javax.servlet.http.HttpServletRequest;

public class SearchEngineeCheckLogic {

    /**
     * 搜索引擎黑名单
     */
    private static final String[] SEARCHER_BLACK_LIST = {
            "http://www.baidu.com", "http://www.google.com", "http://www.google.cn",
            "http://www.soso.com", "http://www.yahoo.cn", "http://search.cn.yahoo.com",
            "http://search.yahoo.com", "http://www.sogou.com",
            "http://www.iask.com", "http://www.yodao.com", "http://www.live.com"};


    /**
     * 是否显示好友列表<br>
     * 根据来源ref
     *
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-30 - 下午10:27:12
     */
    public static boolean isShowFriendList(HttpServletRequest request) {
        String ref = StringUtils.trimToEmpty(StringUtils.lowerCase(request.getHeader("Referer")));
        for (int i = 0; i < SEARCHER_BLACK_LIST.length; i++) {
            if (StringUtils.contains(ref, SEARCHER_BLACK_LIST[i]))
                return false;
		}
		return true;
	}
}
