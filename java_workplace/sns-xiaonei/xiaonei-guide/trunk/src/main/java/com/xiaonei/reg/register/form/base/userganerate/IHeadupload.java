package com.xiaonei.reg.register.form.base.userganerate;

import org.apache.struts.upload.FormFile;

public interface IHeadupload {
    public FormFile getCover();

    public void setCover(FormFile cover);

    public String getTinyUrl();

    public void setTinyUrl(String tinyUrl);

    public String getMainUrl();

    public void setMainUrl(String mainUrl);

    public String getLargeUrl();

    public void setLargeUrl(String largeUrl);

    public String getHeadUrl();

    public void setHeadUrl(String headUrl);

    public String getPorid();

    public void setPorid(String porid);
}
