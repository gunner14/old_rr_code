package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.*;

import org.junit.Test;


public class CandidatesFilterUtilTest {

    
    @Test
    public void testGetShortLink() {
        String link = "http://item.taobao.com/item.htm?id=null";
        assertEquals("tbnull", CandidatesFilterUtil.getShortLink(link));
    }

}
