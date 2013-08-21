package com.renren.sns.wiki.service.search;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.search.WikiSearchBiz;
import com.renren.sns.wiki.enums.WikiStateType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.BaseService;
import com.renren.sns.wiki.utils.PaginationList;

/**
 * Wiki搜索服务。主要用来提供搜索wiki的服务
 * 
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
@Service
public class WikiSearchService extends BaseService {

    @Autowired
    private WikiSearchBiz wikiSearchBiz;

    public PaginationList<Wiki> queryWiki(String keyWords) {
        try {
            PaginationList<Wiki> result = wikiSearchBiz.searchWiki(keyWords, 0, 8);
            if (logger.isDebugEnabled()) {
                logger.debug("query result, size: " + result.getPageList().size() + " content: "
                        + result.getPageList());
            }

            // 可能由于索引更新不及时需要在这里对wiki状态再进行一次判断避免关闭的wiki流出
            List<Wiki> wikiLs = result.getPageList();
            List<Wiki> filteredLs = new ArrayList<Wiki>();
            for (Wiki wiki : wikiLs) {
                if (wiki.getState() < WikiStateType.CLOSE.getState()) {
                    filteredLs.add(wiki);
                }
            }
            result.setPageList(filteredLs);

            return result;
        } catch (Exception e) {
            logger.error("query wiki error. keyWords: " + keyWords, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION);
        }
    }
}
