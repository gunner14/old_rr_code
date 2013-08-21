package com.xiaonei.xnmonitor.dto;

import java.util.Calendar;
import java.util.Date;

public class TimeParam {

    Calendar ca = Calendar.getInstance();

    private int byear;

    private int bmonth;

    private int bday;

    private int bhour;

    private int bminute;

    private int bsecond;

    private int eyear;

    private int emonth;

    private int eday;

    private int ehour;

    private int eminute;

    private int esecond;

    public TimeParam() {
        ca.setTime(new Date());
        byear = ca.get(Calendar.YEAR);
        bmonth = ca.get(Calendar.MONTH) + 1;
        bday = ca.get(Calendar.DAY_OF_MONTH);
        bsecond = 0;
        bhour = 0;
        bminute = 0;

        ca.add(Calendar.DAY_OF_MONTH, 1);
        eyear = ca.get(Calendar.YEAR);
        emonth = ca.get(Calendar.MONTH) + 1;
        eday = ca.get(Calendar.DAY_OF_MONTH);
        ehour = 0;
        eminute = 0;
        esecond = 0;
    }

    public TimeParam(int reduceDay) {
        ca.setTime(new Date());
        eyear = ca.get(Calendar.YEAR);
        emonth = ca.get(Calendar.MONTH) + 1;
        eday = ca.get(Calendar.DAY_OF_MONTH);
        ehour = 0;
        eminute = 0;
        esecond = 0;

        ca.add(Calendar.DAY_OF_MONTH, -1);
        byear = ca.get(Calendar.YEAR);
        bmonth = ca.get(Calendar.MONTH) + 1;
        bday = ca.get(Calendar.DAY_OF_MONTH);
        bsecond = 0;
        bhour = 0;
        bminute = 0;

    }

    public int getByear() {
        return byear;
    }

    public void setByear(int byear) {
        this.byear = byear;
    }

    public int getBmonth() {
        return bmonth;
    }

    public void setBmonth(int bmonth) {
        this.bmonth = bmonth;
    }

    public int getBday() {
        return bday;
    }

    public void setBday(int bday) {
        this.bday = bday;
    }

    public int getBhour() {
        return bhour;
    }

    public void setBhour(int bhour) {
        this.bhour = bhour;
    }

    public int getBminute() {
        return bminute;
    }

    public void setBminute(int bminute) {
        this.bminute = bminute;
    }

    public int getBsecond() {
        return bsecond;
    }

    public void setBsecond(int bsecond) {
        this.bsecond = bsecond;
    }

    public int getEyear() {
        return eyear;
    }

    public void setEyear(int eyear) {
        this.eyear = eyear;
    }

    public int getEmonth() {
        return emonth;
    }

    public void setEmonth(int emonth) {
        this.emonth = emonth;
    }

    public int getEday() {
        return eday;
    }

    public void setEday(int eday) {
        this.eday = eday;
    }

    public int getEhour() {
        return ehour;
    }

    public void setEhour(int ehour) {
        this.ehour = ehour;
    }

    public int getEminute() {
        return eminute;
    }

    public void setEminute(int eminute) {
        this.eminute = eminute;
    }

    public int getEsecond() {
        return esecond;
    }

    public void setEsecond(int esecond) {
        this.esecond = esecond;
    }

}
