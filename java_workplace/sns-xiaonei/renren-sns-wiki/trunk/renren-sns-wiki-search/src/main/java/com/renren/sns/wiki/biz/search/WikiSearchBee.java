package com.renren.sns.wiki.biz.search;

import java.io.File;
import java.io.IOException;

import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.cn.smart.SmartChineseAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.Field.Index;
import org.apache.lucene.document.Field.Store;
import org.apache.lucene.document.NumericField;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.FieldInfo.IndexOptions;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.index.Term;
import org.apache.lucene.queryParser.ParseException;
import org.apache.lucene.queryParser.QueryParser;
import org.apache.lucene.queryParser.QueryParser.Operator;
import org.apache.lucene.search.BooleanClause.Occur;
import org.apache.lucene.search.BooleanQuery;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.NumericRangeQuery;
import org.apache.lucene.search.PrefixQuery;
import org.apache.lucene.search.Query;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.NIOFSDirectory;
import org.apache.lucene.util.Version;

import com.renren.sns.wiki.biz.event.IndexChangeEvent;
import com.renren.sns.wiki.enums.WikiStateType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.search.SearchBee;
import com.renren.sns.wiki.search.utils.PinyinUtils;
import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * Wiki搜索内部实现
 * 
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
public class WikiSearchBee extends SearchBee<Wiki> implements EventHandler<IndexChangeEvent> {

    private Log logger = LogFactory.getLog(WikiSearchBee.class);

    private final static Version matchVersion = Version.LUCENE_36;

    private final static Analyzer analyzer = new SmartChineseAnalyzer(matchVersion);

    private String idxPath = "/data/web/wiki_idx";

    private volatile IndexSearcher indexSearcher;

    public WikiSearchBee() {
        EventUtil.getEventManager().subscribeEvent(this, IndexChangeEvent.class);
    }

    @Override
    public boolean isIndexExists() {
        File idxFile = new File(idxPath);
        return idxFile.exists();
    }

    @Override
    protected IndexWriter getIndexWriter() {
        Directory dir;
        try {
            dir = NIOFSDirectory.open(new File(idxPath));
            IndexWriterConfig idxCfg = new IndexWriterConfig(matchVersion, analyzer);
            idxCfg.setOpenMode(OpenMode.CREATE);
            IndexWriter idxWriter = new IndexWriter(dir, idxCfg);

            return idxWriter;
        } catch (IOException e) {
            throw new RuntimeException("Create index writer error. ", e);
        }
    }

    @Override
    protected Document buildDocument(Wiki bean) {
        Document doc = new Document();

        NumericField wikiId = new NumericField("wikiId", Store.YES, true);
        wikiId.setIntValue(bean.getId());
        doc.add(wikiId);

        NumericField state = new NumericField("state");
        state.setIntValue(bean.getState());
        doc.add(state);

        Field wikiName = new Field("wikiName", bean.getName(), Store.NO, Index.ANALYZED);
        wikiName.setIndexOptions(IndexOptions.DOCS_ONLY);
        doc.add(wikiName);

        try {
            Field pinyinName;
            pinyinName = new Field("pinyinName", PinyinUtils.getPinyin(bean.getName(), false),
                    Store.NO, Index.NOT_ANALYZED);
            pinyinName.setIndexOptions(IndexOptions.DOCS_ONLY);
            doc.add(pinyinName);

            Field shortPinyinName = new Field("shortPinyinName", PinyinUtils.getPinyin(
                    bean.getName(), true), Store.NO, Index.NOT_ANALYZED);
            shortPinyinName.setIndexOptions(IndexOptions.DOCS_ONLY);
            doc.add(shortPinyinName);
        } catch (BadHanyuPinyinOutputFormatCombination e) {
            logger.error("Build index error, wikiName: " + bean.getName(), e);
        }

        return doc;
    }

    public static class ConstriantQueryBuilder implements QueryBuilder {

        public Query buildQuery(String keyWords) {
            BooleanQuery rootQuery = new BooleanQuery();
            BooleanQuery query = new BooleanQuery();

            try {
                QueryParser parser = new QueryParser(matchVersion, "wikiName", analyzer);
                parser.setDefaultOperator(Operator.AND);
                Query nameQuery = parser.parse(keyWords);
                query.add(nameQuery, Occur.SHOULD);
            } catch (ParseException e) {
                throw new RuntimeException("Parser query error, keyWords: " + keyWords, e);
            }

            PrefixQuery fullPinyin = new PrefixQuery(new Term("pinyinName", keyWords));
            query.add(fullPinyin, Occur.SHOULD);

            PrefixQuery shortPinyin = new PrefixQuery(new Term("shortPinyinName", keyWords));
            query.add(shortPinyin, Occur.SHOULD);

            rootQuery.add(query, Occur.MUST);
            NumericRangeQuery<Integer> stateQuery = NumericRangeQuery.newIntRange("state", null,
                    WikiStateType.CLOSE.getState(), false, false);
            rootQuery.add(stateQuery, Occur.MUST);

            return rootQuery;
        }

    }

    private IndexSearcher createIndexSearch() {
        try {
            IndexReader reader = IndexReader.open(NIOFSDirectory.open(new File(idxPath)));
            IndexSearcher searcher = new IndexSearcher(reader);

            return searcher;
        } catch (Exception e) {
            throw new RuntimeException("Get index searcher error", e);
        }
    }

    @Override
    protected IndexSearcher getIndexSearcher() {
        if (null == indexSearcher) {
            synchronized (this) {
                if (null == indexSearcher) {
                    indexSearcher = createIndexSearch();
                }
            }
        }

        return indexSearcher;
    }

    @Override
    public void handleEvent(IndexChangeEvent event) {
        try {
            indexSearcher = createIndexSearch();
        } catch (Exception e) {
            logger.error("Index changed, but recreate index searcher error. ", e);
        }
    }

}
