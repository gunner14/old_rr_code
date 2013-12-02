package com.xiaonei.xce.userurl;

import mop.hi.oce.domain.Markable;
import xce.userbase.UserUrlData;
import xce.userbase.UserUrlDataN;

import com.renren.tripod.proto.UserBaseProto.PbUserUrl;

public class UserUrlInfo extends Markable {

    protected int id;

    protected String largeUrl;

    protected String mainUrl;

    protected String headUrl;

    protected String tinyUrl;

    protected String flexUrl;

    public static final String UID = "ID";

    public static final String LARGEURL = "LARGEURL";

    public static final String MAINURL = "MAINURL";

    public static final String HEADURL = "HEADURL";

    public static final String TINYURL = "TINYURL";

    public static final String FLEXURL = "FLEXURL";

    public String toString() {
        String result = "UserUrlInfo:[";
        result += "id:" + id;
        result += ", largeUrl:" + largeUrl;
        result += ", mainUrl:" + mainUrl;
        result += ", headUrl:" + headUrl;
        result += ", tinyUrl:" + tinyUrl;
        result += ", flexUrl:" + flexUrl;
        result += "]";
        return result;
    }

    public UserUrlInfo parse(UserUrlData data) {
        this.id = data.id;
        this.largeUrl = data.largeUrl;
        this.mainUrl = data.mainUrl;
        this.headUrl = data.headUrl;
        this.tinyUrl = data.tinyUrl;
        return this;
    }

    public UserUrlInfo parse(UserUrlDataN data) {
        this.id = data.id;
        this.largeUrl = data.largeUrl;
        this.mainUrl = data.mainUrl;
        this.headUrl = data.headUrl;
        this.tinyUrl = data.tinyUrl;
        this.flexUrl = data.flexUrl;
        return this;
    }

    public UserUrlInfo parse(PbUserUrl data) {
        this.id = (int) data.getId();
        this.largeUrl = data.getLargeurl();
        this.mainUrl = data.getMainurl();
        this.headUrl = data.getHeadurl();
        this.tinyUrl = data.getTinyurl();
        this.flexUrl = data.getFlexurl();
        return this;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        mark(UserUrlInfo.HEADURL, headUrl);
        this.headUrl = headUrl;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        mark(UserUrlInfo.UID, String.valueOf(id));
        this.id = id;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        mark(UserUrlInfo.LARGEURL, largeUrl);
        this.largeUrl = largeUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        mark(UserUrlInfo.MAINURL, mainUrl);
        this.mainUrl = mainUrl;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        mark(UserUrlInfo.TINYURL, tinyUrl);
        this.tinyUrl = tinyUrl;
    }

    public String getFlexUrl() {
        return flexUrl;
    }

    public void setFlexUrl(String flexUrl) {
        mark(UserUrlInfo.FLEXURL, flexUrl);
        this.flexUrl = flexUrl;
    }
}