package com.xiaonei.xce.search;

import java.util.Arrays;
import com.xiaonei.search.keywordextract.struKwResult;

public class SearchKeywordExtractAdapterTest {
	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
		System.setProperty("isTestSearchChannel", "true");
	    
        String strInput = "鸟叔上春晚1分要10万！网友愤怒】凭《江南Style》风靡全球的韩国明星鸟叔朴载相,近日接到春晚邀请";    
		struKwResult[] kw = SearchKeywordExtractAdapter.getInstance().getKeywords(strInput);
        for(struKwResult item:kw)
        {
            System.out.println("res:" + item.strKeyword);
        }
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
