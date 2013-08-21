package com.xiaonei.reg.guide.logic.msn;

import com.xiaonei.reg.guide.view.WebUserGuideView;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchView;

import java.sql.SQLException;

public class UserGuideWebViewFactory implements SearchViewFactory {

    private static UserGuideWebViewFactory _instance = new UserGuideWebViewFactory();

    public static UserGuideWebViewFactory getInstance() {
        return _instance;
    }

    public SearchView createSearchView() throws SQLException {
        return new WebUserGuideView();
    }

    public String[] getRequiredFields() {
        return new String[]{UserFieldNames.ID, UserFieldNames.NAME,
                UserFieldNames.TINYURL, UserFieldNames.STAR,
                UserFieldNames.SELECTED, UserFieldNames.STATUS,
                UserFieldNames.NETWORK_ID, UserFieldNames.NETWORK_NAME,
                UserFieldNames.NETWORK_STAGE, UserFieldNames.STATUS};
    }
}
