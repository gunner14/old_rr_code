package xiaonei.commons.jade.example.dao;

import java.util.Date;

/**
 * Xiaonei-Commons-Jade 演示: ContactInfo - Bean 对象
 * 
 * @author han.liao
 */
public class ContactInfo {

    protected int id, userId;

    protected String name, phoneCode;

    protected Date createTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPhoneCode() {
        return phoneCode;
    }

    public void setPhoneCode(String phoneCode) {
        this.phoneCode = phoneCode;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    @Override
    public String toString() {
        return "ContactInfo [id=" + id + ", userId=" + userId // NL
                + ", name=" + name + ", phoneCode=" + phoneCode // NL
                + ", createTime=" + createTime + "]";
    }
}
