package com.xiaonei.reg.register.form.base.userganerate;

public interface IInvite {
    public int getInviterId();

    public void setInviterId(int inviterId);

    public int getInviteType();

    public void setInviteType(int inviteType);

    public String getInviteeEmail();

    public void setInviteeEmail(String inviteeEmail);

    public String getGroupName();

    public void setGroupName(String groupName);

    public int getAppId();

    public void setAppId(int appId);

    public String getAppToken();

    public void setAppToken(String appToken);
    
    public String getAppName();
    
    public void setAppName(String appName);
}
