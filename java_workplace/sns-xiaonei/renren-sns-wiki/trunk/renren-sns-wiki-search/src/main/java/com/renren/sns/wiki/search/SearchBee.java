package com.renren.sns.wiki.search;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.CorruptIndexException;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;

/**
 * @author yi.li@renren-inc.com since Aug 13, 2012
 * 
 * @param <T>
 */
public abstract class SearchBee<T> {

    private final static int MAX_RESULT = 500;

    protected abstract IndexWriter getIndexWriter();

    private Log logger = LogFactory.getLog(SearchBee.class);

    public boolean isIndexExists() {
        throw new RuntimeException("Unsupported function. ");
    }

    public final void doIndex(DataFetcher<T> df) throws CorruptIndexException, IOException {
        df.reset();
        IndexWriter idxWriter = getIndexWriter();
        int contiuousException = 0;
        try {
            while (df.hasMore()) {
                try {
                    List<T> data = df.fetchData();
                    for (T bean : data) {
                        Document doc = buildDocument(bean);
                        idxWriter.addDocument(doc);
                    }

                    idxWriter.commit();
                    contiuousException = 0;
                } catch (Exception e) {
                    contiuousException++;
                    logger.error("build index error", e);
                    if (contiuousException > 100) {
                        logger.error("build index exceed max continuous exception count(100), exit build.");
                        break;
                    }
                }
            }
        } finally {
            if (null != idxWriter) {
                idxWriter.close();
            }
        }

    }

    protected abstract Document buildDocument(T bean);

    /**
     * 根据关键字查询所有结果集
     * 
     * @param keyWords key words
     * @return List<Document>
     * @throws IOException
     */
    public final List<Document> query(String keyWords, QueryBuilder queryBuilder)
            throws IOException {
        IndexSearcher idxSearcher = getIndexSearcher();
        Query query = queryBuilder.buildQuery(keyWords);
        TopDocs topDocs = idxSearcher.search(query, MAX_RESULT);
        ScoreDoc[] sDocs = topDocs.scoreDocs;

        List<Document> resultDocs = new ArrayList<Document>();
        if (null == sDocs || sDocs.length == 0) {
            return resultDocs;
        }

        for (int i = 0; i < sDocs.length; i++) {
            resultDocs.add(idxSearcher.doc(sDocs[i].doc));
        }

        return resultDocs;
    }

    /**
     * 分页搜索结果
     * 
     * @param keyWords key words
     * @param page 当前页
     * @param pageSize 页大小
     * @return {@link PaginationResult} 返回分页结果
     * @see PaginationResult
     * @throws IOException
     */
    public final PaginationResult doPaginationQuery(String keyWords, int page, int pageSize,
            QueryBuilder queryBuilder) throws IOException {
        PaginationResult result = new PaginationResult();
        result.setCurrentPage(page);
        result.setPageSize(pageSize);

        IndexSearcher idxSearch = getIndexSearcher();
        Query query = queryBuilder.buildQuery(keyWords);
        TopDocs topDocs = idxSearch.search(query, MAX_RESULT);

        int totalHits = topDocs.totalHits;
        int start = page * pageSize;
        int end = start + pageSize;

        List<Document> docs = new ArrayList<Document>();
        ScoreDoc[] scoreDocs = topDocs.scoreDocs;
        result.setDocs(docs);
        if (null == scoreDocs || scoreDocs.length == 0) {
            return result;
        }

        for (int i = start; i < totalHits; i++) {
            if (i > (end - 1)) {
                break;
            }
            docs.add(idxSearch.doc(scoreDocs[i].doc));
        }

        return result;
    }

    protected abstract IndexSearcher getIndexSearcher();

    /**
     * 用来构建查询，对用一个索引可以有不同的查询方式
     * 
     * @author yi.li@renren-inc.com since Aug 16, 2012
     * 
     */
    public static interface QueryBuilder {

        public Query buildQuery(String keyWords);
    }

}
