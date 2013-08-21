package com.xiaonei.platform.core.opt.permission;

/**
 * 资源实体类，被检查的实体属性
 * 
 * @author yunlong.bai@opi-corp.com
 * 
 */
public abstract class AbsSource implements Source {

    private long id;

    private int control;

    private int userId;

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

//    public AbsSource(int id, int userId, int control) {
//        this.id = id;
//        this.userId = userId;
//        this.control = control;
//    }
    
    public AbsSource(long id, int userId, int control) {
        this.id = id;
        this.userId = userId;
        this.control = control;
    }

    @Override
    public int getControl() {
        return control;
    }

    public void setControl(int control) {
        this.control = control;
    }

    @Override
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

}
