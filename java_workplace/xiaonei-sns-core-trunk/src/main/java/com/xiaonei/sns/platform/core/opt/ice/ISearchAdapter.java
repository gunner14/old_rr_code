package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;

import com.xiaonei.search2.CompoundQuery;
import com.xiaonei.search2.GuideResult;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.AbstractCondition;
import com.xiaonei.search2.model.IndexType;

public interface ISearchAdapter extends IRegisterable{

	GuideResult guide(AbstractCondition condition, int begin, int limit);

	Map<String, GuideResult> guide(Map<String, CompoundQuery> conditionMap);

	List<Integer> search(AbstractCondition arg0, int arg1);

	SearchResult search(AbstractCondition arg0,
			Map<IndexType, SearchViewFactory> arg1, int arg2, int arg3);

	int search(AbstractCondition condition);

	Map<String, SearchResult> search(Map<String, CompoundQuery> conditionMap);

}
