package com.xiaonei.reg.guide.model;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Map;

import xce.searchcache.SearchCacheResult;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchCacheInterface;
import com.xiaonei.search2.model.client.UserSearchView;

public class NetBarUserView implements UserSearchView, SearchCacheInterface {

    public NetBarUserView() {

    }

    private int id;

    private int age;

    private int birthyear;

    private String name;

    private String gender;

    private String tinyurl;

    private String tinyFullurl;

    private String homecity;

    private int networkId;

    private String networkName;

    public int getNetworkId() {
        return networkId;
    }

    public void setNetworkId(int networkId) {
        this.networkId = networkId;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    GregorianCalendar g = new GregorianCalendar();

    public String getHomecity() {
        return homecity;
    }

    public void setHomecity(String homecity) {
        this.homecity = homecity;
    }

    private java.util.List<String> _networks = new ArrayList<String>();

    public void buildSearchResultInfo(SearchCacheResult sc) {
        try {
            id = Integer.parseInt(sc.generels.get(UserFieldNames.ID));
        } catch (NumberFormatException e) {
            id = 0;
        }
        try {
            age = g.get(Calendar.YEAR)  - Integer.parseInt(sc.generels.get(UserFieldNames.BIRTH_YEAR));
        } catch (NumberFormatException e) {
            age = 0;
        }
        try {
            networkId = Integer.parseInt(sc.generels.get(UserFieldNames.NETWORK_ID));
        } catch (NumberFormatException e) {
            networkId = 0;
        }
        name = sc.generels.get(UserFieldNames.NAME);
        tinyurl = sc.generels.get(UserFieldNames.TINYURL);
        tinyFullurl = HeadUrlUtil.getHeadFullUrl(sc.generels.get(UserFieldNames.TINYURL));
        try {
            birthyear = Integer.parseInt(sc.generels.get(UserFieldNames.BIRTH_YEAR));
        } catch (NumberFormatException e) {
            birthyear = 0;
        }
        gender = sc.generels.get(UserFieldNames.GENDER);
        homecity = sc.generels.get(UserFieldNames.HOME_CITY);
        networkName = sc.generels.get(UserFieldNames.NETWORK_NAME);
        for (Map<String, String> entry : sc.infos.get(UserFieldNames.NETWORK)) {
            _networks.add(entry.get(UserFieldNames.NETWORK_NAME));
        }
    }

    public void buildGeneral(ResultSet rs) throws SQLException {
        id = rs.getInt(UserFieldNames.ID);
        age = g.get(Calendar.YEAR) - rs.getInt(UserFieldNames.BIRTH_YEAR);
        birthyear = rs.getInt(UserFieldNames.BIRTH_YEAR);
        name = rs.getString(UserFieldNames.NAME);
        tinyurl = rs.getString(UserFieldNames.TINYURL);
        tinyFullurl = HeadUrlUtil.getHeadFullUrl(rs.getString(UserFieldNames.TINYURL));
        gender = rs.getString(UserFieldNames.GENDER);
        homecity = rs.getString(UserFieldNames.HOME_CITY);
        networkId=rs.getInt(UserFieldNames.NETWORK_ID);
        networkName = rs.getString(UserFieldNames.NETWORK_NAME);
    }

    public void buildNetwork(ResultSet rs) throws SQLException {
        String network = rs.getString(UserFieldNames.NETWORK_NAME);
        _networks.add(network);
    }

    public void buildHighSchoolInfo(ResultSet rs) throws SQLException {

    }

    public void buildUniversityInfo(ResultSet rs) throws SQLException {

    }

    public void buildWorkplaceInfo(ResultSet rs) throws SQLException {

    }

    public void buildRegionInfo(ResultSet rs) throws SQLException {

    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getTinyurl() {
        return tinyurl;
    }

    public int getBirthyear() {
        return birthyear;
    }

    public String getGender() {
        return gender;
    }

    public void buildMatch(String match) {

    }

    public void buildConfig(ResultSet rs) throws SQLException {
        birthyear = rs.getInt(UserFieldNames.BIRTH_YEAR);
    }

    public void setTinyFullurl(String tinyFullurl) {
        this.tinyFullurl = tinyFullurl;
    }

    public String getTinyFullurl() {
        return tinyFullurl;
    }

    public void setNetworkName(String networkName) {
        this.networkName = networkName;
    }

    public String getNetworkName() {
        return networkName;
    }
}
