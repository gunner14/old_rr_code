package com.xiaonei.reg.guide.view;

import java.io.Serializable;
import java.util.Date;

public class PopShare implements Serializable {

    private static final long serialVersionUID = 4472729050246363949L;
    //主键
    private int id;
    private int owner;
    private int auth;
    private int fromNo;
    private String fromName;
    private String fromUniv;
    private String link;
    private String pic;
    private int type;
    private String body;
    private Date createTime;
    private String title;
    private String ownerName;
    private String ownerTinyUrl;
    private String summary;
    private int albumId;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getOwner() {
        return owner;
    }

    public void setOwner(int owner) {
        this.owner = owner;
    }

    public int getAuth() {
        return auth;
    }

    public void setAuth(int auth) {
        this.auth = auth;
    }

    public int getFromNo() {
        return fromNo;
    }

    public void setFromNo(int fromNo) {
        this.fromNo = fromNo;
    }

    public String getFromName() {
        return fromName;
    }

    public void setFromName(String fromName) {
        this.fromName = fromName;
    }

    public String getFromUniv() {
        return fromUniv;
    }

    public void setFromUniv(String fromUniv) {
        this.fromUniv = fromUniv;
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }

    public String getPic() {
        return pic;
    }

    public void setPic(String pic) {
        this.pic = pic;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getOwnerName() {
        return ownerName;
    }

    public void setOwnerName(String ownerName) {
        this.ownerName = ownerName;
    }

    public String getOwnerTinyUrl() {
        return ownerTinyUrl;
    }

    public void setOwnerTinyUrl(String ownerTinyUrl) {
        this.ownerTinyUrl = ownerTinyUrl;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    public int getAlbumId() {
        return albumId;
    }

    public void setAlbumId(int albumId) {
        this.albumId = albumId;
    }

}
