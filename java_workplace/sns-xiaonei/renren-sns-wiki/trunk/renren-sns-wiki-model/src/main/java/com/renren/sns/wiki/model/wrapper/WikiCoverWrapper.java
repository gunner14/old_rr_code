package com.renren.sns.wiki.model.wrapper;

import java.io.Serializable;

import com.renren.sns.wiki.model.WikiCover;


/**
 * WikiCover的包装类,当没有cover时可以减少保存到cache的流量
 * @author weiwei.wang@renren-inc.com since 2012-3-31
 *
 */
public class WikiCoverWrapper implements Serializable {

    private static final long serialVersionUID = -4057923834386854744L;

    private WikiCover wikiCover;
    
    private boolean haveCover;

    
    public WikiCover getWikiCover() {
        return wikiCover;
    }

    
    public void setWikiCover(WikiCover wikiCover) {
        this.wikiCover = wikiCover;
    }

    
    public boolean isHaveCover() {
        return haveCover;
    }

    
    public void setHaveCover(boolean haveCover) {
        this.haveCover = haveCover;
    }
    
}
