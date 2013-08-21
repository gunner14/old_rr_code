package com.xiaonei.reg.register.form.base.userganerate;

public interface IBaseInfo {
    // // 来源URL 统计用
    // protected String fromUrl;
    // // 链接类型
    // protected String code;
    // protected String from;
    // // 市场渠道活动
    // protected String channel;
    // protected String iu;
    // protected String ic;
    // // 新版邀请，统一成一个参数
    // protected String uuid;
    public String getFromUrl();

    public void setFromUrl(String fromUrl);

    public String getCode();

    public void setCode(String code);

    public String getFrom();

    public void setFrom(String from);

    public String getChannel();

    public void setChannel(String channel);

    public String getIu();

    public void setIu(String iu);

    public String getIc();

    public void setIc(String ic);

    public String getUuid();

    public void setUuid(String uuid);

}
