package com.renren.sns.wiki.constant;

import com.xiaonei.platform.core.opt.DyString;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * wiki web工程所需的常量
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-30
 * 
 */
public class WikiWebConstant {

    /**
     * wiki的域名
     */
    public static final DyString urlWiki = new DyString() {

        @Override
        public String toString() {
            String domain = OpiConstants.getCurrentDomain();
            if (domain == null) {
                domain = OpiConstants.defXiaoneiDomain;
            }
            return "http://wiki." + domain;
        }
    };

}
