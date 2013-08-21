package com.xiaonei.reg.guide.model;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import com.xiaonei.platform.core.head.HeadUrlUtil;

/**
 * FootprintView for SNS<br>
 * all fields are same with <b>
 * <code>mop.hi.oce.domain.model.FootprintView</code></b>
 * 
 * @author wei.xiang (wei.xiang@opi-corp.com)
 * @since 20090917
 * @author tai.wang@opi-corp.com Mar 29, 2010 - 9:54:36 AM
 */
public class SnsFootprintView implements Serializable {

    /** one day in millseconds */
    private static final long ONE_DAY = 1000L * 60 * 60 * 24;

    private static final long serialVersionUID = 655102490000623428L;

    public static void main(String[] args) {
        SnsFootprintView v = new SnsFootprintView();
        v.setTime(System.currentTimeMillis());
        System.out.println(v.getDateString());
        System.out.println(v.getDaysBeforeToday());
        v.setTime(System.currentTimeMillis() - 2 * ONE_DAY);
        System.out.println(v.getDaysBeforeToday());
        v.setTime(System.currentTimeMillis() - 1 * ONE_DAY);
        System.out.println(v.getDaysBeforeToday());
        System.out.println(v.getDateString());
    }

    /** the guest id */
    private int guest;

    /** the guest name */
    private String guestName;

    /** the guest head url (relative path) */
    private String guestUrl;

    /** the host id */
    private int host;

    /** china west-south charity */
    private boolean isCharity;
    
    private boolean isKeepUse;
    
    private boolean isOnline;
    
    
    public boolean isWapOnline() {
        return isWapOnline;
    }

    
    public void setWapOnline(boolean isWapOnline) {
        this.isWapOnline = isWapOnline;
    }

    private boolean isWapOnline;
    
    private boolean isVip;

    /** the visit time */
    private Date time = null;

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null) return false;
        if (getClass() != obj.getClass()) return false;
        SnsFootprintView other = (SnsFootprintView) obj;
        if (guest != other.guest) return false;
        if (guestName == null) {
            if (other.guestName != null) return false;
        } else if (!guestName.equals(other.guestName)) return false;
        if (guestUrl == null) {
            if (other.guestUrl != null) return false;
        } else if (!guestUrl.equals(other.guestUrl)) return false;
        if (host != other.host) return false;
        return time == other.time;
    }

    /**
     * get the time in month and day format. For example: 2月3日,12月3日
     * 
     * @return the String format of time
     * @since 20090927
     */
    public String getDateString() {
        if (time == null) return "";
        try {
            //SimpleDateFormat is not thread-safety
            return new SimpleDateFormat("M月d日").format(time);
        } catch (Exception e) {
            return "";
        }
    }

    /**
     * get the date time in full format<br>
     * For example: 2009-09-27 17:11
     * 
     * @return the full format date
     * @since 20090927
     */
    public String getDateTimeString() {
        if (time == null) return "";
        try {
            //SimpleDateFormat is not thread-safety
            return new SimpleDateFormat("yyyy-MM-dd HH:mm").format(time);
        } catch (Exception e) {
            return "";
        }
    }

    /**
     * get the visit time in some format<br>
     * 
     * <pre>
     * &lt;ol&gt;&lt;li&gt;today: HH:mm, for example, 20:05&lt;/li&gt;
     * &lt;li&gt;yesterday: 昨天&lt;/li&gt;
     * &lt;li&gt;the day before yesterday: 前天&lt;/li&gt;
     * &lt;li&gt;three days ago: MM:dd, for example, 2月4日&lt;/li&gt;
     * &lt;li&gt;no time: &quot;&quot;(empty)&lt;/li&gt;
     * &lt;/ol&gt;
     * </pre>
     * 
     * @return the format of visit time
     * @since 20090927
     */
    public String getDateTipString() {
        switch (getDaysBeforeToday()) {
            case 0:
                return getTimeString();
            case 1:
                return "昨天";
            case 2:
                return "前天";
            case -1:
                return "";
            default:
                return getDateString();
        }
    }

    /**
     * get the days before today
     * 
     * @return -1~NaN
     * 
     *         <pre>
     * The result:&lt;ol&gt;
     * &lt;li&gt;-1: if no time fetched&lt;/li&gt;
     * &lt;li&gt;0: today&lt;/li&gt;
     * &lt;li&gt;1: yesterday&lt;/li&gt;
     * &lt;li&gt;2: the day before yesterday&lt;/li&gt;
     * &lt;li&gt;n: ...&lt;/li&gt;
     * &lt;/ol&gt;
     * </pre>
     * @since 20090927
     */
    public int getDaysBeforeToday() {
        if (time == null) return -1;
        long millseconds = time.getTime();
        if (millseconds <= 0) return -1;
        Calendar c = Calendar.getInstance();
        c.set(Calendar.HOUR_OF_DAY, 0);
        c.set(Calendar.MINUTE, 0);
        c.set(Calendar.SECOND, 0);
        c.set(Calendar.MILLISECOND, 0);
        //
        long t = (c.getTimeInMillis() - millseconds);
        return t <= 0 ? 0 : (int) (1 + t / ONE_DAY);
    }

    public int getGuest() {
        return guest;
    }

    /**
     * get the full url for guest's head picture
     * 
     * @return the full url
     */
    public String getGuestFullUrl() {
        return HeadUrlUtil.getHeadFullUrl(getGuestUrl());
    }

    public String getGuestName() {
        return guestName;
    }

    public String getGuestUrl() {
        return guestUrl;
    }

    public int getHost() {
        return host;
    }

    public Date getTime() {
        return time;
    }

    /**
     * get the time in hour and minute format. For example:
     * 00:12,12:30,23:59
     * 
     * @return the String format of time
     * @since 20090927
     */
    public String getTimeString() {
        if (time == null) return "";
        try {
            //SimpleDateFormat不是线程安全的，所以需要每次new一个出来
            return new SimpleDateFormat("HH:mm").format(time);
        } catch (Exception e) {
            return "";
        }
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + guest;
        result = prime * result + ((guestName == null) ? 0 : guestName.hashCode());
        result = prime * result + ((guestUrl == null) ? 0 : guestUrl.hashCode());
        result = prime * result + host;
        //result = prime * result + time;
        return result;
    }

    public boolean isCharity() {
        return isCharity;
    }

    public boolean isKeepUse() {
        return isKeepUse;
    }

    public boolean isOnline() {
        return isOnline;
    }

    public boolean isVip() {
        return isVip;
    }

    public void setCharity(boolean isCharity) {
        this.isCharity = isCharity;
    }

    public void setGuest(int guest) {
        this.guest = guest;
    }

    public void setGuestName(String guestName) {
        this.guestName = guestName;
    }

    public void setGuestUrl(String guestUrl) {
        this.guestUrl = guestUrl;
    }

    public void setHost(int host) {
        this.host = host;
    }

    public void setKeepUse(boolean isKeepUse) {
        this.isKeepUse = isKeepUse;
        
    }

    public void setOnline(boolean isOnline) {
        this.isOnline = isOnline;
        
    }

    public void setTime(Date time) {
        this.time = time;
    }

    /**
     * set time in millseconds
     * 
     * @param millseconds
     */
    public void setTime(long millseconds) {
        this.time = new Date(millseconds);
    }

    public void setVip(boolean isVip) {
        this.isVip = isVip;
        
    }

}

