package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;
import java.util.Map;

import com.xiaonei.search2.CompoundQuery;
import com.xiaonei.search2.GuideResult;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.AbstractCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter;

/**
 * the wrapper for search client
 * 
 * @author xylz(xylz@live.cn)
 * @since 20091113
 */
public class WSearchAdapter implements ISearchAdapter {
	private SearchAdapter adpater = null;

	protected WSearchAdapter() {
		adpater = SearchAdapter.getInstance();
	}

	/**
	 * @deprecated
	 * @see SnsAdapterFactory#getSearchAdapter()
	 */
	public static ISearchAdapter getInstance() {
		return SnsAdapterFactory.getSearchAdapter();
	}

	@Override
	public GuideResult guide(AbstractCondition condition, int begin, int limit) {
		return adpater.guide(condition, begin, limit);
	}

	@Override
	public Map<String, GuideResult> guide(
			Map<String, CompoundQuery> conditionMap) {
		return adpater.guide(conditionMap);
	}

	@Override
	public List<Integer> search(AbstractCondition arg0, int arg1) {
		return adpater.search(arg0, arg1);
	}

	@Override
	public SearchResult search(AbstractCondition arg0,
			Map<IndexType, SearchViewFactory> arg1, int arg2, int arg3) {
		return adpater.search(arg0, arg1, arg2, arg3);
	}

	@Override
	public int search(AbstractCondition condition) {
		return adpater.search(condition);
	}

	@Override
	public Map<String, SearchResult> search(
			Map<String, CompoundQuery> conditionMap) {
		return adpater.search(conditionMap);
	}

}
