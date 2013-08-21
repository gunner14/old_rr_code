package com.renren.sns.wiki.model.info;

import com.renren.sns.wiki.model.info.annotation.WikiInfoFieldName;

/**
 * 作者/出版社/出版年/页数/定价/装帧/简介 Wiki页资料收起状态默认展示：作者、出版年、简介
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 * 
 */
public class Book extends WikiInfo {

    @WikiInfoFieldName( { "副标题" })
    private String subtitle; //作者

    @WikiInfoFieldName( { "作者" })
    private String author; //作者

    @WikiInfoFieldName( { "出版社" })
    private String publisher; //出版社

    @WikiInfoFieldName( { "出版年", "出版年份" })
    private String publishDate; //出版年

    @WikiInfoFieldName( { "页数" })
    private String pageCount;//页数

    @WikiInfoFieldName( { "定价", "价格" })
    private String price;//定价

    @WikiInfoFieldName( { "装帧" })
    private String look;//装帧

    @WikiInfoFieldName( { "ISBN", "isbn" })
    private String isbn;//装帧

    @WikiInfoFieldName( { "简介" })
    private String summary;//简介

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getPublisher() {
        return publisher;
    }

    public void setPublisher(String publisher) {
        this.publisher = publisher;
    }

    public String getPublishDate() {
        return publishDate;
    }

    public void setPublishDate(String publishDate) {
        this.publishDate = publishDate;
    }

    public String getPageCount() {
        return pageCount;
    }

    public void setPageCount(String pageCount) {
        this.pageCount = pageCount;
    }

    public String getPrice() {
        return price;
    }

    public void setPrice(String price) {
        this.price = price;
    }

    public String getLook() {
        return look;
    }

    public void setLook(String look) {
        this.look = look;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    public String getSubtitle() {
        return subtitle;
    }

    public void setSubtitle(String subtitle) {
        this.subtitle = subtitle;
    }

    public String getIsbn() {
        return isbn;
    }

    public void setIsbn(String isbn) {
        this.isbn = isbn;
    }

    @Override
    public String toString() {
        return "Book [author=" + author + ", look=" + look + ", pageCount=" + pageCount
                + ", price=" + price + ", publishDate=" + publishDate + ", publisher=" + publisher
                + ", subtitle=" + subtitle + ", summary=" + summary + "]";
    }

}
