package com.renren.sns.wiki.interceptor;

import com.renren.sns.wiki.model.Wiki;

/**
 * 存放wiki数据
 * 
 * @author yi.li@renren-inc.com since 2012-5-21
 * 
 */
public interface WikiHolder {

    public Wiki getWiki();

    public void setWiki(Wiki wiki);
}
