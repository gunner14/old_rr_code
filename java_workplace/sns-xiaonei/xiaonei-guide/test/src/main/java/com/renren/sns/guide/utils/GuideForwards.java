package com.renren.sns.guide.utils;

import java.util.HashMap;
import java.util.Map;


public class GuideForwards {

    private GuideForwards() {
    }

    private static GuideForwards instance = null;

    public static synchronized GuideForwards getInstance() {
        if (instance == null) {
            instance = new GuideForwards();
        }
        return instance;
    }
    
    private static Map<String,String> map=new HashMap<String,String>();
    
    static{
        map.put("app", "guide_app");
        map.put("bar", "guide_net_bar");
        map.put("forwardGames", "guide_forward_games");    
        map.put("kaixinApp", "guide_kaixin_app");         
        map.put("mntOfEdu", "guide_hs_mntOfEdu");
        map.put("ok", "guide_common");    
        map.put("social", "guide_social");
        map.put("xf", "guide_xf");
    }
    
    public String getForwardPath(String forwardName){
        return map.get(forwardName);
    }
}
