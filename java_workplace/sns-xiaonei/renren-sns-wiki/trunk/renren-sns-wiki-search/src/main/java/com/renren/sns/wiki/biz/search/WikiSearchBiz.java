package com.renren.sns.wiki.biz.search;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.lucene.document.Document;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.biz.WikiBiz;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.search.PaginationResult;
import com.renren.sns.wiki.search.SearchBee;
import com.renren.sns.wiki.search.SearchBee.QueryBuilder;
import com.renren.sns.wiki.utils.PaginationList;

/**
 * wiki搜索服务
 * 
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
@Service
public class WikiSearchBiz extends BaseBiz implements InitializingBean {

    private SearchBee<Wiki> searchBee;

    @Autowired
    private WikiSearchDataFetcher wikiSearchDataFetcher;

    @Autowired
    private WikiBiz wikiBiz;

    private QueryBuilder wikiContriantQueryBuilder = new WikiSearchBee.ConstriantQueryBuilder();

    /**
     * 搜索wiki
     * 
     * @param keyWords
     * @param page
     * @param pageSize
     * @return
     * @throws IOException
     */
    public PaginationList<Wiki> searchWiki(String keyWords, int page, int pageSize)
            throws IOException {
        PaginationResult queryResult = searchBee.doPaginationQuery(keyWords, page, pageSize,
                wikiContriantQueryBuilder);
        PaginationList<Wiki> result = new PaginationList<Wiki>();
        result.setCurrentPage(queryResult.getCurrentPage());
        result.setPageSize(queryResult.getPageSize());
        result.setTotalCount(queryResult.getTotalHits());

        List<Document> docs = queryResult.getDocs();
        List<Integer> wikiIds = new ArrayList<Integer>();
        for (Document doc : docs) {
            Integer wikiId = Integer.parseInt(doc.get("wikiId"));
            wikiIds.add(wikiId);
        }

        if (logger.isDebugEnabled()) {
            logger.debug("query from index, wikiIds: " + wikiIds.toString());
        }

        if (wikiIds.size() > 0) {
            List<Wiki> wikiLs = wikiBiz.getWikiListFromCache(wikiIds);
            result.setPageList(wikiLs);
        } else {
            result.setPageList(new ArrayList<Wiki>(0));
        }

        return result;
    }

    public void rebuildIndex() {
        try {
            searchBee.doIndex(wikiSearchDataFetcher);
        } catch (Exception e) {
            throw new RuntimeException("rebuild index error. ", e);
        }
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        searchBee = new WikiSearchBee();
        if (searchBee.isIndexExists()) {
            logger.info("index exists. initial will not happen. ");
            return;
        }

        logger.info("begin to initial index");
        searchBee.doIndex(wikiSearchDataFetcher);
        logger.info("finish index");
    }
}
