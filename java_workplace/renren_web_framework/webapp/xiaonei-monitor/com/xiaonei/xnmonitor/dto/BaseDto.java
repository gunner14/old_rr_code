package com.xiaonei.xnmonitor.dto;

import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;

public class BaseDto {

    Calendar ca = Calendar.getInstance();

    private Timestamp date;

    String pattern = "yyyy-MM-dd";

    DateFormat format = new SimpleDateFormat(pattern);

    public String getDate() {
        return format.format(date);
    }

    public void setDate(Timestamp date) {
        this.date = date;
    }

    public String getLable() {
        ca.setTime(this.date);
        int year = getYear(ca);
        int month = getMonth(ca);
        int day = getDay(ca);
        if (month == 0 && day == 1) {
            return year + "/" + (month + 1) + "/" + day;
        } else if (day == 1) {
            return month + 1 + "/" + day;
        } else {
            return day + "";
        }
    }

    public int getYear(Calendar ca) {
        return ca.get(Calendar.YEAR);
    }

    public int getMonth(Calendar ca) {
        return ca.get(Calendar.MONTH);
    }

    public int getDay(Calendar ca) {
        return ca.get(Calendar.DAY_OF_MONTH);
    }

    public int getHour(Calendar ca) {
        return ca.get(Calendar.HOUR_OF_DAY);
    }

    public int getMinute(Calendar ca) {
        return ca.get(Calendar.MINUTE);
    }

}
