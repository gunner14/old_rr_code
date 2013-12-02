package com.renren.ad.sns.services.avenue.filter;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.AvenueFilterKeywords;
import com.renren.ad.sns.data.Goods;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:beans/ranking-service.xml")
public class KeywordGoodsCandidateFilterTest extends AbstractJUnit4SpringContextTests{
	@Autowired
    private Datastore datastore;

    
    @Test
    public void testFilter() {
    
    		AvenueFilterKeywords filterKeywords=new AvenueFilterKeywords();
    		filterKeywords.setKeyword("我蛮喜欢这个哦");
    		filterKeywords.setType(AvenueFilterKeywords.SPAM_KEYWORD_TYPE_FOR_DESC);
    		datastore.save(filterKeywords);
    	
    }

}
