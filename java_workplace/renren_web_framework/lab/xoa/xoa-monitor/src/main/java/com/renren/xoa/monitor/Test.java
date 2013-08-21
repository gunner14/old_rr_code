package com.renren.xoa.monitor;

import java.util.HashMap;
import java.util.Map;

import org.codehaus.jackson.map.ObjectMapper;

import com.renren.xoa.jackson.ObjectMapperFactory;


public class Test {

    public static void main(String[] args) throws Exception{
        
        
        ObjectMapper mapper = ObjectMapperFactory.getInstance().getObjectMapper();
        
        Map<String, String> map = new HashMap<String, String>();
        map.put("1", "aaaa");
        map.put("2", "bbbbb");
        
        String s = mapper.writeValueAsString(map);
        System.out.println(s);
        
        Map ret = mapper.readValue(s, Map.class);
        System.out.println(ret);
        
        
        MapWrapper mw = new MapWrapper();
        
        mw.map.put("aaa", 11111);
        mw.map.put("bbb", 22222);
        
        s = mapper.writeValueAsString(mw);
        
        mw = mapper.readValue(s, MapWrapper.class);
        
        System.out.println(mw.map.get("aaa").getClass());
    }
    
    public static class MapWrapper {
        
        private Map<String, Integer> map = new HashMap<String, Integer>();

        
        public Map<String, Integer> getMap() {
            return map;
        }
        
        public void setMap(Map<String, Integer> map) {
            this.map = map;
        }
    }
    
}
