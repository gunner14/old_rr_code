package com.renren.entrybase.model;

import junit.framework.Assert;

import org.junit.Test;


public class FieldEncoderTest {

    
    @Test
    public void testEncode1() {
        
        String s = "+-x/%fafsadsa=fds  afds+afdsa%2D$%1";
        String encoded = FieldEncoder.encodeField(s);
        String decoded = FieldEncoder.decodeField(encoded);
        Assert.assertEquals(s, decoded);
    }
    
    
}
