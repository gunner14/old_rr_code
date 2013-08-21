package com.renren.sns.guide.logic;


/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午11:16:31
 */
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.dodoyo.friends.utils.LogTimeUtils;
import com.renren.sns.guide.model.GuideSearchRelation;
import com.renren.sns.guide.model.GuideSearchRelationFriend;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchView;

public class GuideRelationSearch {

    private final Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();

    private final static GuideRelationSearch instance = new GuideRelationSearch();

    private GuideRelationSearch() {
        factories.put(IndexType.User, new SearchViewFactory() {
            public SearchView createSearchView() throws SQLException {
                return new GuideSearchRelationFriend();
            }

            public String[] getRequiredFields() {
                return new String[] { UserFieldNames.ID, UserFieldNames.NAME,UserFieldNames.TINYURL, UserFieldNames.HEADURL };
            }

        });
    }

    public static GuideRelationSearch getInstance() {
        return GuideRelationSearch.instance;
    }

    public GuideSearchRelation search(int OpUser, int offset, int limit,
            GuideCondition builder) {
        SearchResult result = null;
        GuideSearchRelation relation = null;
        builder.addOpUser(OpUser);
        // 搜索
        long beginSearchResult=LogTimeUtils.logBegin();
        try {
			result = SearchAdapter.getInstance().search(builder, factories, offset,limit);
		} 
        catch (Ice.TimeoutException e){
        	System.err.println(" host:"+OpUser+" e:"+e.toString());
        }
        catch (Exception e) {
        	System.err.println(" host:"+OpUser+" e:"+e.toString());
			e.printStackTrace();
		}
        LogTimeUtils.logEnd(beginSearchResult, "EndSearchResult");
        long beginFriendsContents=LogTimeUtils.logBegin();
        if(result != null){
        	// 结果
        	List<SearchView> friends = result.getContents();
        	LogTimeUtils.logEnd(beginFriendsContents, "EndFriendsContents");
        	// 规整搜索结果
        	long beginTotalCount=LogTimeUtils.logBegin();
        	relation = new GuideSearchRelation();
        	for (SearchView searchView : friends) {
        		relation.add((GuideSearchRelationFriend) searchView);
        	}
        	relation.setCount(result.getTotalCount());
        	LogTimeUtils.logEnd(beginTotalCount, "EndTotalCount");
        }
        return relation;
    }
}

 