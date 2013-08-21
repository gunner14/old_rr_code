package com.renren.xoa.commons.binaryparam;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.junit.Assert;
import org.junit.Test;

import com.renren.xoa.commons.binaryparam.impl.ParameterWrapperImpl;
import com.renren.xoa.commons.binaryparam.model.MockUserFactory;


public class MultiFormatParamEncoderTest {
    
    @Test
    public void testJava() throws Exception {
        
        MultiFormatParamEncoder encoder = new MultiFormatParamEncoder();
        
        Map<String, ParameterWrapper> params = new HashMap<String, ParameterWrapper>();
        
        ParameterWrapperImpl wrapper = new ParameterWrapperImpl();
        String key = "user1";
        wrapper.setKey(key);
        wrapper.setFormat(ParamFormat.JAVA_SERIALIZATION);
        List<Object> values = new ArrayList<Object>(1);
        values.add(MockUserFactory.getUser());
        wrapper.setValues(values);
        
        
        params.put(key, wrapper);
        
        ParameterWrapperImpl wrapper2 = new ParameterWrapperImpl();
        String key2 = "user2";
        wrapper2.setKey(key2);
        wrapper2.setFormat(ParamFormat.JAVA_SERIALIZATION);
        List<Object> values2 = new ArrayList<Object>(1);
        values2.add(MockUserFactory.getUser());
        wrapper2.setValues(values2);
        
        params.put(key2, wrapper2);
        
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        encoder.encodeToStream(params, bos);
        
        System.out.println(bos.size());
        
        ByteArrayInputStream bis = new ByteArrayInputStream(bos.toByteArray());
        
        Map<String, List<Object>> ret = encoder.decodeFromStream(bis);
        Assert.assertEquals(params.size(), ret.size());
        
        for (Entry<String, List<Object>> entry : ret.entrySet()) {
            
            String k = entry.getKey();
            Object o = entry.getValue().get(0);
            
            Object expectedO = params.get(k).getValues().get(0);
            Assert.assertEquals(expectedO, o);
        }
    }
    
}
