package com.xiaonei.reg.guide.factory;

import java.sql.SQLException;

import com.xiaonei.reg.guide.model.NetBarUserView;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.UserSearchView;

public class NetBarUserViewFactory implements SearchViewFactory {
	private static NetBarUserViewFactory _instance = new NetBarUserViewFactory();
	public static NetBarUserViewFactory getInstance() {
		if (_instance == null) {
			_instance = new NetBarUserViewFactory();
		}
		return _instance;
	}

	private NetBarUserViewFactory() {
	}

	public UserSearchView createSearchView() throws SQLException {
		return new NetBarUserView();
	}

	public String[] getRequiredFields() {
		return new String[] { 
			UserFieldNames.ID, 
			UserFieldNames.NAME,
			UserFieldNames.BIRTH_YEAR,			
			UserFieldNames.TINYURL,
			UserFieldNames.HOME_CITY,
		};
	}
}
