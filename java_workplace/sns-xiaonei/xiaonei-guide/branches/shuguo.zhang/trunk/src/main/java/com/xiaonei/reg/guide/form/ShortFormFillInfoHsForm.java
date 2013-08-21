package com.xiaonei.reg.guide.form;

import java.io.Serializable;

public class ShortFormFillInfoHsForm extends ShortFormFillInfoBaseForm implements Serializable {

    private static final long serialVersionUID = 4732822466385155439L;

    private int schoolType = -1;

    private String highSchoolName;

    private int highSchoolCode;

    private int highschoolyear;

    private String techSchoolName;

    private int techSchoolCode;

    private int techschoolyear;

    private String elementaryschoolname;

    private int elementaryschoolyear;

    private String juniorhighschoolname;

    private int juniorSchoolCode;

    private int juniorhighschoolyear;

    private String porId;

    private String tinyUrl;

    private String headUrl;

    private String mainUrl;

    private String largeUrl;

    public String getPorId() {
        return porId;
    }

    public void setPorId(String porId) {
        this.porId = porId;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public int getJuniorSchoolCode() {
        return juniorSchoolCode;
    }

    public void setJuniorSchoolCode(int juniorSchoolCode) {
        this.juniorSchoolCode = juniorSchoolCode;
    }

    public String getTechSchoolName() {
        return techSchoolName;
    }

    public void setTechSchoolName(String techSchoolName) {
        this.techSchoolName = techSchoolName;
    }

    public int getTechSchoolCode() {
        return techSchoolCode;
    }

    public void setTechSchoolCode(int techSchoolCode) {
        this.techSchoolCode = techSchoolCode;
    }

    public int getTechschoolyear() {
        return techschoolyear;
    }

    public void setTechschoolyear(int techschoolyear) {
        this.techschoolyear = techschoolyear;
    }

    public String getJuniorhighschoolname() {
        return juniorhighschoolname;
    }

    public void setJuniorhighschoolname(String juniorhighschoolname) {
        this.juniorhighschoolname = juniorhighschoolname;
    }

    public int getJuniorhighschoolyear() {
        return juniorhighschoolyear;
    }

    public void setJuniorhighschoolyear(int juniorhighschoolyear) {
        this.juniorhighschoolyear = juniorhighschoolyear;
    }

    public int getElementaryschoolyear() {
        return elementaryschoolyear;
    }

    public void setElementaryschoolyear(int elementaryschoolyear) {
        this.elementaryschoolyear = elementaryschoolyear;
    }

    public String getElementaryschoolname() {
        return elementaryschoolname;
    }

    public void setElementaryschoolname(String elementaryschoolname) {
        this.elementaryschoolname = elementaryschoolname;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public void setHighSchoolCode(int highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public int getHighschoolyear() {
        return highschoolyear;
    }

    public void setHighschoolyear(int highschoolyear) {
        this.highschoolyear = highschoolyear;
    }

    public int getSchoolType() {
        return schoolType;
    }

    public void setSchoolType(int schoolType) {
        this.schoolType = schoolType;
    }


}
