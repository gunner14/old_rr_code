package com.renren.sns.wiki.search.demo;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.wiki.search.DataFetcher;

/**
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
public class TxtDataFetcher implements DataFetcher<String> {

    private boolean hasMore = true;

    private int recordNum = 0;

    private String[] testWords = new String[] { "仙剑奇侠传三", "剑雨", "IvanLee's 猜想", "首尔" };

    int i = 0;

    @Override
    public boolean hasMore() {
        return hasMore;
    }

    @Override
    public List<String> fetchData() {
        recordNum += 3;
        if (recordNum > 20) {
            hasMore = false;
        }

        List<String> result = new ArrayList<String>();
        result.add(testWords[i++ % testWords.length] + recordNum);
        return result;
    }

    @Override
    public void reset() {
        recordNum = 0;
        hasMore = true;
    }

}
