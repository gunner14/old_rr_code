/**
 * 
 */
package com.xiaonei.click.module;

import java.io.Serializable;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class Click implements Serializable {

    private static final long serialVersionUID = -6309314239658847043L;

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public int x = 0;

    public int y = 0;

    public String url = null;

    public int userId = 0;

    public long time = 0;

    @Override
    public String toString() {
        return "Click [time=" + time + ", url=" + url + ", userId=" + userId + ", x=" + x + ", y="
                + y + "]";
    }

}
