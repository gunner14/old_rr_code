package com.renren.sns.wiki.model.info;

import com.renren.sns.wiki.model.info.annotation.WikiInfoFieldName;

/**
 * 游戏类型/游戏平台/主要角色/发行时间/发行商/开发商/简介 Wiki页资料收起状态默认展示：开发商、游戏类型、简介
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 * 
 */
public class Game extends WikiInfo {

    @WikiInfoFieldName( { "别名", "外文名", "英文名" })
    private String anotherName;

    @WikiInfoFieldName( { "开发商" })
    private String developer;//开发商

    @WikiInfoFieldName( { "游戏类型", "游戏类别" })
    private String genre;//游戏类型

    @WikiInfoFieldName( { "游戏平台" })
    private String platform;//游戏类型

    @WikiInfoFieldName( { "主要角色" })
    private String role;//主要角色

    @WikiInfoFieldName( { "发行时间" })
    private String releaseDate;//发行时间

    @WikiInfoFieldName( { "发行商" })
    private String publisher;//发行商

    @WikiInfoFieldName( { "简介" })
    private String summary;//简介 

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public String getPlatform() {
        return platform;
    }

    public void setPlatform(String platform) {
        this.platform = platform;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String getReleaseDate() {
        return releaseDate;
    }

    public void setReleaseDate(String releaseDate) {
        this.releaseDate = releaseDate;
    }

    public String getDeveloper() {
        return developer;
    }

    public void setDeveloper(String developer) {
        this.developer = developer;
    }

    public String getPublisher() {
        return publisher;
    }

    public void setPublisher(String publisher) {
        this.publisher = publisher;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    public String getAnotherName() {
        return anotherName;
    }

    public void setAnotherName(String anotherName) {
        this.anotherName = anotherName;
    }

}
