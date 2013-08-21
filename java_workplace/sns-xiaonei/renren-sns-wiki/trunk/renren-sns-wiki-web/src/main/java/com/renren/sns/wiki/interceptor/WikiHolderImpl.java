package com.renren.sns.wiki.interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;

import com.renren.sns.wiki.model.Wiki;

/**
 * WikiHolder的默认实现
 * 
 * @author yi.li@renren-inc.com since 2012-5-21
 * 
 */
public class WikiHolderImpl implements WikiHolder {

    private static final String WIKI_ATTRIBUTE = "wiki";

    @Autowired
    private Invocation inv;

    @Override
    public Wiki getWiki() {
        return (Wiki) inv.getRequest().getAttribute(WIKI_ATTRIBUTE);
    }

    @Override
    public void setWiki(Wiki wiki) {
        inv.getRequest().setAttribute(WIKI_ATTRIBUTE, wiki);
    }

}
