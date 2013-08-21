package com.renren.sns.guide.controllers.window;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.guide.utils.UserCountComparator;
import com.xiaonei.commons.interceptors.throughput.MethodThroughput;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.sns.platform.core.rose.interceptors.MergeWindowModel;
import com.xiaonei.sns.platform.core.rose.interceptors.ServiceCutter;

/**
 * HomeExUserCount类的逻辑太复杂，单独从HomeWindowsController里单独出来
 * 
 * @author wei.xiang (wei.xiang@opi-corp.com)
 * 
 *         1、将“通知”和“招呼”的计数从其他请求中去掉
 *         2、有“通知”和“招呼”的时候，最多显示1个请求和1个其他请求，后面显示“通知”和“招呼”
 *         3、仅有“通知”或者“招呼”的时候，最多显示2个请求和1个其他请求，后面显示“通知”或“招呼”
 *         4、没有“通知”和“招呼”的时候，最多显示3个请求和1个其他请求
 */
@ServiceCutter
public class GuideUserCountController {

public static Properties p = new Properties();
    
    public static Properties pex = new Properties();
    
    public static Properties mantissa = new Properties();
    
    public static Integer intMantissa = 0;
    
    private Map<Integer, String> wordMap = null;

    private Map<Integer, String> styleMap = null;

    //顺序的Map
    private Map<Integer, String> orderMap = null;

    private void initStyle(CacheManager cm) {
        styleMap = loadMap("style-",cm);        
    }

    private void initWordMap(CacheManager cm) {
        wordMap = loadMap("word-",cm);       
    }

    private void initOrderMap(CacheManager cm) {
        orderMap = loadMap("order-",cm);        
    }

    private Map<Integer, String> loadMap(String startKey,CacheManager cm) {
        Map<Integer, String> retMap = new HashMap<Integer, String>();
        try {     
            User host = UserProvider.getInstance().get(cm);
            if(mantissa.isEmpty()){
                mantissa.load(this.getClass().getClassLoader().getResourceAsStream("address-list-request.properties"));
            }   
            for (Object k : mantissa.keySet()) {
                String key = (String) k;
                String value = mantissa.getProperty(key);
                intMantissa=Integer.parseInt(value);
            }              
            if(host.getId()%10 >= intMantissa){
                if(pex.isEmpty()){
                    pex.load(this.getClass().getClassLoader().getResourceAsStream("usercount-ex.properties"));
                }
                for (Object k : pex.keySet()) {
                    String key = (String) k;
                    if (key.startsWith(startKey)) {
                        String value = pex.getProperty(key);
                        try {
                            retMap.put(Integer.parseInt(key.substring(startKey.length())), value);
                        } catch (NumberFormatException e) {
                            e.printStackTrace();
                        }
                    }
                }    
            }
            else{                
                if(p.isEmpty()){            
                    p.load(this.getClass().getClassLoader().getResourceAsStream("usercount.properties"));
                }    
                for (Object k : p.keySet()) {
                    String key = (String) k;
                    if (key.startsWith(startKey)) {
                        String value = p.getProperty(key);
                        try {
                            retMap.put(Integer.parseInt(key.substring(startKey.length())), value);
                        } catch (NumberFormatException e) {
                            e.printStackTrace();
                        }
                    }
                }    
            }                                
                   
        } catch (IOException e) {
            e.printStackTrace();
        }
        return retMap;
    }

    private Map<Integer, String> getLinkMap(CacheManager cm) {
        Map<Integer, String> linkMap = loadMap("link-",cm);        
        for (Entry<Integer, String> entry : linkMap.entrySet()) {
            entry.setValue(String.format(entry.getValue(), OpiConstants.domain.toString()));
        }
        return linkMap;
    }    

    public static class UserCount {
    	
    	private static final long serialVersionUID = -8420967221056288894L;

        int id;

        int count;

        int order;

        String link;

        String word;

        String style;

        public UserCount(int id, int count, String link, String word, String style, int order) {
            this.id = id;
            this.count = count;
            this.link = link;
            this.word = word;
            this.style = style;
            this.order = order;
        }

        public int getId() {
            return id;
        }

        public int getCount() {
            return count;
        }

        public int getOrder() {
            return order;
        }

        public void setOrder(int order) {
            this.order = order;
        }

        public String getLink() {
            return link;
        }

        public String getWord() {
            return word;
        }

        public String getStyle() {
            return style;
        }

        @Override
        public String toString() {
            return "UserCount [count=" + count + ", id=" + id + ", link=" + link + ", style=" + style + ", word=" + word + ", order=" + order + "]";
        }
    }

    @MethodThroughput(maxConcurrent = 50)
    @MergeWindowModel
    public void homeUserCount(Invocation inv, Window window, CacheManager cm) {    	
        initStyle(cm);
        initWordMap(cm);
        initOrderMap(cm);
        List<Integer> otherList=new ArrayList<Integer>();         
        otherList.add(36);//热点动态
        otherList.add(39);//人气请求        
        otherList.add(43);//通讯录请求
        User host = UserProvider.getInstance().get(cm);
        Map<Integer, Integer> uc = UserCountMgr.getCountMap(host.getId());
        if (uc != null) {
            List<UserCount> list = new ArrayList<UserCount>();
            //展示的list
            List<UserCount> displayList = new ArrayList<UserCount>();
            Map<Integer, String> linkMap = getLinkMap(cm);
            for (Entry<Integer, Integer> u : uc.entrySet()) {
            	//好友类型数
            	if(u.getKey()==UserCountMgr.FRIEND_TYPE_UNREAD){
            		inv.addModel("FRIEND_TYPE_UNREAD", u.getValue());
            	}
            	//请求类型数
            	if(u.getKey()==UserCountMgr.REQUEST_TYPE_UNREAD){
            		inv.addModel("REQUEST_TYPE_UNREAD", u.getValue());
            	}
            	//通知类型数
            	if(u.getKey()==UserCountMgr.MESSAGE_TYPE_UNREAD){
            		inv.addModel("MESSAGE_TYPE_UNREAD", u.getValue());
            	}
            	//应用邀请的个数
            	if(u.getKey()==0){
            		inv.addModel("appInviteCount", u.getValue());
            	}
            	if (linkMap.get(u.getKey()) == null || wordMap.get(u.getKey()) == null
                        || styleMap.get(u.getKey()) == null || orderMap.get(u.getKey()) == null || u.getValue() == 0) 
                    continue;
                try {
                    UserCount ucount = new UserCount(u.getKey(), u.getValue(), linkMap.get(u.getKey()), wordMap.get(u.getKey()), styleMap.get(u.getKey()), 
                            Integer.parseInt(orderMap.get(u.getKey())));
                    list.add(ucount);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
            }
            if (list != null) {
                Collections.sort(list, new UserCountComparator());
                int otherCount = 0;
                /**
                 * ucType: 0:没有通知和招呼 1:有一个通知或招呼 2:有通知和招呼
                 */
                int ucType = 0, msgCount = 0, pokeCount = 0;
                String msgOrPoke = "msg";
                for (int i = 0; i < list.size(); i++) {
                    if ("个通知".equals(list.get(i).getWord())) {
                        msgCount = list.get(i).getCount();
                        inv.addModel("msgCount", msgCount);
                        msgOrPoke = "msg";
                        ucType++;
                        continue;
                    }
                    if ("个招呼".equals(list.get(i).getWord())) {
                        pokeCount = list.get(i).getCount();
                        inv.addModel("pokeCount", pokeCount);
                        msgOrPoke = "poke";
                        ucType++;
                        continue;
                    }
                }
                UserCount muc = new UserCount(73, msgCount, OpiConstants.urlMsg + "/notify/notifications.do", "个通知", "l-request", 73);
                UserCount puc = new UserCount(73, pokeCount, OpiConstants.urlMain + "/GetPokeList.do", "个招呼", "l-poke", 73);
                switch (ucType) {
                    case 0:
                        /*
                         * 计算其他请求的个数
                         * 只有 list.size() > 4才会有其他请求                        
                         */
                        if (list.size() > 4) {
                            for (int i = 0; i < list.size(); i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    continue;
                                } else if (i >= 3) {
                                    //其他请求的个数中不包括不在请求中心的数据
                                    if(!otherList.contains(list.get(i).getId())){
                                        otherCount += list.get(i).getCount();
                                    }                                  
                                }
                            }
                        }
                        //填充展示的list
                        if (otherCount > 0) {
                            //当有其他请求的时候，list.size()必大于等于3，参见上面的else if(i>=3)
                            for (int i = 0; i < 3; i++) {
                                displayList.add(list.get(i));
                            }
                            UserCount ouc = new UserCount(73, otherCount, OpiConstants.urlReq + "/request/requestList.do", "个其他请求", "iOther", 73);
                            displayList.add(ouc);
                        } else {
                            for (int i = 0; i < list.size(); i++) {
                                displayList.add(list.get(i));
                            }
                        }
                        break;
                    case 1:
                        if (list.size() > 4) {
                            /*
                             * 计算其他请求的个数
                             * 只有 list.size() > 4才会有其他请求                        
                             */
                            boolean isInSecond = false;//通知或招呼是否在前两个
                            int loopTime = list.size();
                            if (list.size() > 2) {
                                loopTime = 2;
                            }
                            for (int i = 0; i < loopTime; i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    isInSecond = true;
                                }
                            }
                            if (isInSecond) {
                                for (int i = 0; i < list.size(); i++) {
                                    if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                        continue;
                                    } else if (i >= 3) {
                                        //其他请求的个数中不包括不在请求中心的数据
                                        if(!otherList.contains(list.get(i).getId())){
                                            otherCount += list.get(i).getCount();
                                        }    
                                    }
                                }
                            } else {
                                for (int i = 0; i < list.size(); i++) {
                                    if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                        continue;
                                    } else if (i >= 2) {
                                        //仅有“通知”或者“招呼”的时候，最多显示2个请求和1个其他请求
                                        //其他请求的个数中不包括不在请求中心的数据
                                        if(!otherList.contains(list.get(i).getId())){
                                            otherCount += list.get(i).getCount();
                                        }    
                                    }
                                }
                            }
                        }
                        //填充展示的list
                        if (otherCount > 0) {
                            //froDisCount：前面不是通知不是招呼的请求的个数，最多只填充两个
                            int froDisCount = 0;
                            //当有其他请求的时候，list.size()必大于等于2，参见上面的else if(i>=2)
                            for (int i = 0; i < list.size(); i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    continue;
                                }
                                displayList.add(list.get(i));
                                froDisCount++;
                                if (froDisCount == 2) break;
                            }
                            UserCount ouc = new UserCount(73, otherCount, OpiConstants.urlReq + "/request/requestList.do", "个其他请求", "iOther", 73);
                            displayList.add(ouc);
                            if ("msg".equals(msgOrPoke)) {
                                displayList.add(muc);
                            } else if ("poke".equals(msgOrPoke)) {
                                displayList.add(puc);
                            }
                        } else {
                            //无其他请求的时候list.size()必<= 4，所以填充完displayList后,displayList不会>4
                            for (int i = 0; i < list.size(); i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    continue;
                                }
                                displayList.add(list.get(i));
                            }
                            if ("msg".equals(msgOrPoke)) {
                                //把通知加进去
                                displayList.add(muc);
                            } else if ("poke".equals(msgOrPoke)) {
                                //把招呼加进去
                                displayList.add(puc);
                            }
                        }
                        break;
                    case 2:
                        /*
                         * 计算其他请求的个数
                         * 只有 list.size() > 4才会有其他请求                        
                         */
                        if (list.size() > 4) {
                            boolean msgPokeFirst = false;//通知或者招呼是否在第一个          
                            if (list.size() >= 0) {
                                if ("个通知".equals(list.get(0).getWord()) || "个招呼".equals(list.get(0).getWord())) {
                                    msgPokeFirst = true;
                                }
                            }
                            if (msgPokeFirst) {
                                for (int i = 0; i < list.size(); i++) {
                                    if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                        continue;
                                    } else if (i >= 2) {
                                        //有“通知”和“招呼”的时候，最多显示1个请求和1个其他请求
                                        //其他请求的个数中不包括不在请求中心的数据
                                        if(!otherList.contains(list.get(i).getId())){
                                            otherCount += list.get(i).getCount();
                                        }    
                                    }
                                }
                            } else {
                                for (int i = 0; i < list.size(); i++) {
                                    if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                        continue;
                                    } else if (i >= 1) {
                                        //其他请求的个数中不包括不在请求中心的数据
                                        if(!otherList.contains(list.get(i).getId())){
                                            otherCount += list.get(i).getCount();
                                        }    
                                    }
                                }
                            }
                        }
                        //填充展示的list
                        if (otherCount > 0) {
                            //froDisCount：前面不是通知和招呼的请求的个数，最多只填充一个
                            int froDisCount = 0;
                            for (int i = 0; i < list.size(); i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    continue;
                                }
                                displayList.add(list.get(i));
                                froDisCount++;
                                if (froDisCount == 1) break;
                            }
                            UserCount ouc = new UserCount(73, otherCount, OpiConstants.urlReq + "/request/requestList.do", "个其他请求", "iOther", 73);
                            //把其他请求加进去
                            displayList.add(ouc);
                            //把通知和招呼加进去
                            displayList.add(muc);
                            displayList.add(puc);
                        } else {
                            //无其他请求的时候list.size()必<= 4，所以填充完displayList后,displayList不会>4
                            for (int i = 0; i < list.size(); i++) {
                                if ("个通知".equals(list.get(i).getWord()) || "个招呼".equals(list.get(i).getWord())) {
                                    continue;
                                }
                                displayList.add(list.get(i));
                            }
                            //把通知和招呼加进去
                            displayList.add(muc);
                            displayList.add(puc);
                        }
                        break;
                }
                if (displayList.size() > 0) {
                    inv.addModel("displayList", displayList);
                }
                inv.addModel("msgOrPoke", msgOrPoke);
                inv.addModel("ucType", ucType);
            }
        }
    }
}

