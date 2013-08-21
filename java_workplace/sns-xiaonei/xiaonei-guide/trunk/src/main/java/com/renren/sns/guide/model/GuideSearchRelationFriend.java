package com.renren.sns.guide.model;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午11:14:09
 * 类说明
 */

import java.sql.ResultSet;
import java.sql.SQLException;

import xce.searchcache.SearchCacheResult;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchCacheInterface;
import com.xiaonei.search2.model.client.UserSearchView;

public class GuideSearchRelationFriend implements GuideIRelationFriend, UserSearchView,
        SearchCacheInterface {

    private int id = 0;

    private String name = "";

    private String gender = "";

    private String tinyUrl = "";

    private String headUrl = "";

    public String getGender() {
        return gender;
    }

    public String getTiny_Url() {
        return HeadUrlUtil.getHeadFullUrl(tinyUrl);

    }

    public String getHead_Url() {
        return HeadUrlUtil.getHeadFullUrl(headUrl);
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void buildConfig(ResultSet rs) throws SQLException {
    }

    public void buildGeneral(ResultSet rs) throws SQLException {
        id = rs.getInt(UserFieldNames.ID);
        name = rs.getString(UserFieldNames.NAME);
        tinyUrl = rs.getString(UserFieldNames.TINYURL);
        headUrl = rs.getString(UserFieldNames.HEADURL);
    }

    public void buildMatch(String match) {
    }

    public void buildNetwork(ResultSet rs) throws SQLException {
    }

    public void buildHighSchoolInfo(ResultSet rs) throws SQLException {
        // TODO Auto-generated method stub

    }

    public void buildRegionInfo(ResultSet rs) throws SQLException {
        // TODO Auto-generated method stub

    }

    public void buildUniversityInfo(ResultSet rs) throws SQLException {
        // TODO Auto-generated method stub

    }

    public void buildWorkplaceInfo(ResultSet rs) throws SQLException {
        // TODO Auto-generated method stub

    }

    @Override
    public void buildSearchResultInfo(SearchCacheResult sc) {

        if ((sc == null) || (sc.generels == null) || (sc.infos == null)) return;

        try {
            id = Integer.parseInt(sc.generels.get(UserFieldNames.ID));
            name = sc.generels.get(UserFieldNames.NAME);
            gender = sc.generels.get(UserFieldNames.GENDER);
            headUrl = sc.generels.get(UserFieldNames.HEADURL);
            tinyUrl = sc.generels.get(UserFieldNames.TINYURL);
        } catch (NumberFormatException e1) {}
    }

}
