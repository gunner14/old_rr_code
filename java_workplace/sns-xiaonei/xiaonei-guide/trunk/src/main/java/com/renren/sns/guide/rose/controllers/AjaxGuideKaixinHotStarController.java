package com.renren.sns.guide.rose.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.factory.NetBarUserViewFactory;
import com.xiaonei.reg.guide.model.NetBarUserView;
import com.xiaonei.search2.CompoundQuery;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.NetbarSearchCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午04:22:17
 */
public class AjaxGuideKaixinHotStarController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User host = UserProvider.getInstance().get(cm);
        int one=1,two=2,pageIndex=0;
        if(inv.getRequest().getParameter("pageIndex")==null){
            pageIndex=0;
        }
        else{
            pageIndex=Integer.parseInt(inv.getRequest().getParameter("pageIndex"));
        }  
        try{
            String gender=host.getGender();
            NetbarSearchCondition condition = new NetbarSearchCondition();                  
            //根据男生搜女生
            condition.addGender("男生", false); 
            //根据女生搜男生
            if("女生".equals(gender)){ 
                condition.addGender("女生", false); 
            }
            Map<String, CompoundQuery> boyConditionMap = new HashMap<String, CompoundQuery>(); 
            Map<IndexType, SearchViewFactory> boyFactories = new HashMap<IndexType, SearchViewFactory>();      
            boyFactories.put(IndexType.User, NetBarUserViewFactory.getInstance());            
            boyConditionMap.put("1", new CompoundQuery(condition, boyFactories, pageIndex*two, two));
            Map<String,SearchResult> boyResults = null;
            boyResults = SearchAdapter.getInstance().search(boyConditionMap);
            //根据女生搜男生
            condition.addGender("女生", false); 
            //根据男生搜女生
            if("女生".equals(gender)){ 
                condition.addGender("男生",false);
            }
            Map<String, CompoundQuery> girlConditionMap = new HashMap<String, CompoundQuery>(); 
            Map<IndexType, SearchViewFactory> girlFactories = new HashMap<IndexType, SearchViewFactory>();      
            girlFactories.put(IndexType.User, NetBarUserViewFactory.getInstance());           
            girlConditionMap.put("1", new CompoundQuery(condition, girlFactories, pageIndex*one, one));
            Map<String,SearchResult> girlResults = null;
            girlResults = SearchAdapter.getInstance().search(girlConditionMap);
            List<NetBarUserView> list=new ArrayList<NetBarUserView>();
            if(boyResults!=null && boyResults.size()>0){
                for (SearchResult result:boyResults.values()) {
                    for (SearchView sv : result.getContents()) {
                        if (sv instanceof NetBarUserView) {
                            NetBarUserView new_name = (NetBarUserView) sv;                                  
                            list.add(new_name);                          
                        }
                    }                        
                }                    
            }
            if(girlResults!=null && girlResults.size()>0){
                for (SearchResult result:girlResults.values()) {
                    for (SearchView sv : result.getContents()) {
                        if (sv instanceof NetBarUserView) {
                            NetBarUserView new_name = (NetBarUserView) sv;    
                            list.add(new_name);                          
                        }
                    }                        
                }                    
            }
            inv.addModel("guideKaixinHotStarList", list);  
        }
        catch(Exception e){
            e.printStackTrace();
        }
        return "/views/guide/xf/common/right/guide_star_li.jsp";
    }
}
 