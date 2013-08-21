package com.renren.sns.wiki.search.demo;

import java.io.IOException;
import java.util.List;

import org.apache.lucene.document.Document;

import com.renren.sns.wiki.search.PaginationResult;

/**
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
public class SearchDemo {

    /**
     * @param args
     */
    public static void main(String[] args) {
        TxtSearchBee searchBee = new TxtSearchBee();
        TxtDataFetcher df = new TxtDataFetcher();

        try {
            searchBee.doIndex(df);
            //            List<Document> docs = searchBee.query("格瓦拉");
            PaginationResult result = searchBee.doPaginationQuery("仙剑", 0, 5,
                    new TxtSearchBee.TxtQueryBuilder());
            List<Document> docs = result.getDocs();

            for (Document doc : docs) {
                System.out.println("==> " + doc.get("txt"));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
