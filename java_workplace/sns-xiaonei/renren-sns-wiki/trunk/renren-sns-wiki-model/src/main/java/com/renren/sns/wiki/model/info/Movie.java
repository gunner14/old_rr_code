package com.renren.sns.wiki.model.info;

import com.renren.sns.wiki.model.info.annotation.WikiInfoFieldName;

/**
 * 电影 导演/编剧/主演/类型/制片国家（地区）/语言/上映日期/片长/又名/IMDb链接/官方主页/官方小站/出品公司/开机地/简介
 * Wiki页资料收起状态默认展示：导演、主演、简介
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 */
public class Movie extends WikiInfo {

    /**
     * 电影 导演/编剧/主演/类型/制片国家（地区）/语言/上映日期/片长/又名/IMDb链接/官方主页/官方小站/出品公司/开机地/简介
     * Wiki页资料收起状态默认展示：导演、主演、简介
     * 
     * @author weiwei.wang
     */
    @WikiInfoFieldName( { "导演" })
    private String director;// 导演

    @WikiInfoFieldName( { "编剧" })
    private String writer;// 编剧

    @WikiInfoFieldName( { "主演" })
    private String starring;// 主演

    @WikiInfoFieldName( { "类型" })
    private String type;// 类型

    @WikiInfoFieldName( { "制片国家（地区）", "制片国家/地区", "制片国家" })
    private String country;// 制片国家（地区）

    @WikiInfoFieldName( { "语言" })
    private String language;// 语言

    @WikiInfoFieldName( { "上映日期" })
    private String releaseDate;// 上映日期

    @WikiInfoFieldName( { "片长" })
    private String runningTime;// 片长

    @WikiInfoFieldName( { "又名" })
    private String alias;// 又名

    @WikiInfoFieldName( { "IMDb链接", "imdb", "IMDb" })
    private String IMDb;// IMDb链接

    @WikiInfoFieldName( { "官方主页" })
    private String homePage;// 官方主页

    @WikiInfoFieldName( { "官方小站" })
    private String site;// 官方小站

    @WikiInfoFieldName( { "出品公司" })
    private String company;// 出品公司

    @WikiInfoFieldName( { "开机地" })
    private String movieBoot;// 开机地

    @WikiInfoFieldName( { "简介" })
    private String summary;// 简介

    public String getDirector() {
        return director;
    }

    public void setDirector(String director) {
        this.director = director;
    }

    public String getWriter() {
        return writer;
    }

    public void setWriter(String writer) {
        this.writer = writer;
    }

    public String getStarring() {
        return starring;
    }

    public void setStarring(String starring) {
        this.starring = starring;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public String getLanguage() {
        return language;
    }

    public void setLanguage(String language) {
        this.language = language;
    }

    public String getReleaseDate() {
        return releaseDate;
    }

    public void setReleaseDate(String releaseDate) {
        this.releaseDate = releaseDate;
    }

    public String getRunningTime() {
        return runningTime;
    }

    public void setRunningTime(String runningTime) {
        this.runningTime = runningTime;
    }

    public String getAlias() {
        return alias;
    }

    public void setAlias(String alias) {
        this.alias = alias;
    }

    public String getIMDb() {
        return IMDb;
    }

    public void setIMDb(String iMDb) {
        IMDb = iMDb;
    }

    public String getHomePage() {
        return homePage;
    }

    public void setHomePage(String homePage) {
        this.homePage = homePage;
    }

    public String getSite() {
        return site;
    }

    public void setSite(String site) {
        this.site = site;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getMovieBoot() {
        return movieBoot;
    }

    public void setMovieBoot(String movieBoot) {
        this.movieBoot = movieBoot;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    @Override
    public String toString() {
        return "Movie [IMDb=" + IMDb + ", alias=" + alias + ", company=" + company + ", country="
                + country + ", director=" + director + ", homePage=" + homePage + ", language="
                + language + ", movieBoot=" + movieBoot + ", releaseDate=" + releaseDate
                + ", runningTime=" + runningTime + ", site=" + site + ", starring=" + starring
                + ", summary=" + summary + ", type=" + type + ", writer=" + writer
                + ", getPropertyList()=" + getPropertyList() + "]";
    }

}
