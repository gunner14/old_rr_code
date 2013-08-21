package com.renren.sns.wiki.biz.search;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.WikiBiz;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.search.DataFetcher;

/**
 * 为搜索提供数据的数据获取器
 * 
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
@Service
public class WikiSearchDataFetcher implements DataFetcher<Wiki> {

    private boolean hasMore = true;

    private int currentPage = 0;

    private int pageSize = 100;

    @Autowired
    private WikiBiz wikiBiz;

    @Override
    public boolean hasMore() {
        return hasMore;
    }

    @Override
    public List<Wiki> fetchData() {
        try {
            List<Wiki> wikiList = wikiBiz.getWikiListByLimit(currentPage * pageSize, pageSize);
            if (wikiList.size() == 0) {
                hasMore = false;
            }

            return wikiList;
        } finally {
            currentPage++;
        }
    }

    @Override
    public void reset() {
        hasMore = true;
        currentPage = 0;
    }

}
