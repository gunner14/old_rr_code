package com.xiaonei.reg.guide.view;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.reg.guide.model.UserNetwork;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchCacheInterface;
import com.xiaonei.search2.model.client.UserSearchView;
import xce.searchcache.SearchCacheResult;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class WebUserGuideView implements UserSearchView, SearchCacheInterface {

    public WebUserGuideView() {
    }

    private int _id;
    private int _star;
    private int _selected;
    private int _status;

    private String _name;
    private String _tiny_url;

    private List<String> _matches = new ArrayList<String>();
    private UserNetwork _network = new UserNetwork();

    public void buildHighSchoolInfo(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
    }

    public void buildRegionInfo(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
    }

    public void buildUniversityInfo(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
    }

    public void buildWorkplaceInfo(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
    }

    public void buildMatch(String match) {
        //  Auto-generated method stub
        _matches.add(match);
    }

    public void buildGeneral(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
        _id = rs.getInt(UserFieldNames.ID);
        _name = rs.getString(UserFieldNames.NAME);
        _star = rs.getInt(UserFieldNames.STAR);
        _selected = rs.getInt(UserFieldNames.SELECTED);
        _tiny_url = rs.getString(UserFieldNames.TINYURL);
    }

    public void buildNetwork(ResultSet rs) throws SQLException {
        //  Auto-generated method stub
        int status = rs.getInt(UserFieldNames.STATUS);
        if (status == -1) {
            _network.setNetworkId(rs.getInt(UserFieldNames.NETWORK_ID));
            _network.setNetworkName(rs.getString(UserFieldNames.NETWORK_NAME));
            _network.setStage(rs.getInt(UserFieldNames.NETWORK_STAGE));
        }
    }

    public int getId() {
        //  Auto-generated method stub
        return _id;
    }

    public String getName() {
        return _name;
    }

    public int getStar() {
        return _star;
    }

    public int getSelected() {
        return _selected;
    }

    public int getStatus() {
        return _status;
    }

    public String getTinyurl() {
        return HeadUrlUtil.getHeadFullUrl(_tiny_url);
    }

    public UserNetwork getNetwork() {
        return _network;
    }

    public List<String> getMatches() {
        return _matches;
    }

    public void buildConfig(ResultSet rs) throws SQLException {

    }

    public void buildSearchResultInfo(SearchCacheResult sc) {

        if ((sc == null) || (sc.generels == null) || (sc.infos == null))
            return;

        try {
//			private int _id;
//			private int _star;
//			private int _selected;
//			private int _status;
//
//			private String _name;
//			private String _tiny_url;
//
//			private List<String> _matches = new ArrayList<String>();
//			private UserNetwork _network = new UserNetwork();

            _id = Integer.parseInt(sc.generels.get(UserFieldNames.ID));
            _star = Integer.parseInt(sc.generels.get(UserFieldNames.STAR));
            _selected = Integer.parseInt(sc.generels
                    .get(UserFieldNames.SELECTED));
            _name = sc.generels.get(UserFieldNames.NAME);
            _tiny_url = sc.generels.get(UserFieldNames.TINYURL);

        } catch (NumberFormatException e1) {
        }


//		for (Map<String, String> networks : sc.infos
//				.get(UserFieldNames.NETWORK))
//		{
//			UserNetwork network = new UserNetwork();
//			try
//			{
//				int netId = Integer.parseInt(networks
//						.get(UserFieldNames.NETWORK_ID));
//				int netStage = Integer.parseInt(networks
//						.get(UserFieldNames.NETWORK_STAGE));
//				network.setNetworkId(netId);
//				network.setNetworkName(networks
//						.get(UserFieldNames.NETWORK_NAME));
//				network.setStage(netStage);
//				
//				_network.add(network);
//			} catch (NumberFormatException e)
//			{
//			}
//		}
    }

}
