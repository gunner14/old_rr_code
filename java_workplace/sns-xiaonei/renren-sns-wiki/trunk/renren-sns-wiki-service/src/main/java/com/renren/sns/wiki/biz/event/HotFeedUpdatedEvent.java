package com.renren.sns.wiki.biz.event;

import com.renren.sns.wiki.utils.event.Event;

/**
 * 重新载入最热Feed的事件
 * 
 * @author yi.li@renren-inc.com since 2012-7-5
 * 
 */
public class HotFeedUpdatedEvent implements Event {

    /**
     * 
     */
    private static final long serialVersionUID = -7722828380139580610L;

    private int wikiId;

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }
}
