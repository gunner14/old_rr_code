/**
 *
 */
package com.xiaonei.reg.register.pojo;

import java.io.Serializable;


/**
 * AuditBasePojo.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-10 下午03:59:45
 */
public class AuditBasePojo implements Serializable {
    private static final long serialVersionUID = -7412740112563388153L;

    public AuditBasePojo(Upload up) {
        super();
        this.largeUrl = up.getLarge();
        this.mainUrl = up.getMain();
        this.tinyUrl = up.getTiny();
        this.headUrl = up.getHead();
    }

    //头像路径
    private String tinyUrl;
    private String mainUrl;
    private String largeUrl;
    private String headUrl;

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }


}
