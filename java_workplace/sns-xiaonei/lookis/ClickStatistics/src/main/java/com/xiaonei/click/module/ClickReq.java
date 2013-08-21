/**
 * 
 */
package com.xiaonei.click.module;

import java.io.Serializable;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class ClickReq implements Serializable {

    private static final long serialVersionUID = -189675313362018261L;

    @Override
    public String toString() {
        return "ClickReq [down=" + down + ", left=" + left + ", right=" + right + ", tMax=" + tMax
                + ", tMin=" + tMin + ", top=" + top + ", url=" + url + ", userId=" + userId + "]";
    }

    public int getTop() {
        return top;
    }

    public void setTop(int top) {
        this.top = top;
    }

    public int getLeft() {
        return left;
    }

    public void setLeft(int left) {
        this.left = left;
    }

    public int getDown() {
        return down;
    }

    public void setDown(int down) {
        this.down = down;
    }

    public int getRight() {
        return right;
    }

    public void setRight(int right) {
        this.right = right;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public long gettMin() {
        return tMin;
    }

    public void settMin(long tMin) {
        this.tMin = tMin;
    }

    public long gettMax() {
        return tMax;
    }

    public void settMax(long tMax) {
        this.tMax = tMax;
    }

    private int top;

    private int left;

    private int down;

    private int right;

    private int userId;

    private String url;

    private long tMin;

    private long tMax;
}
