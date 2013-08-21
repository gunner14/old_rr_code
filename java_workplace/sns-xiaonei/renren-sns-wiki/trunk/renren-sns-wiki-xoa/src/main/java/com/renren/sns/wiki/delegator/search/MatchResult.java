package com.renren.sns.wiki.delegator.search;

import java.io.Serializable;

/**
 * @author yi.li@renren-inc.com since Jul 30, 2012
 * 
 */
public class MatchResult implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = -1686713649814533773L;

    private int wikiId;

    private String matchString;

    @Deprecated
    private int begin;

    @Deprecated
    private int end;

    private int state;

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    public String getMatchString() {
        return matchString;
    }

    public void setMatchString(String matchString) {
        this.matchString = matchString;
    }

    public int getBegin() {
        return begin;
    }

    public void setBegin(int begin) {
        this.begin = begin;
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }

    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
    }
}
