package com.renren.sns.guide.action;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;
import com.xiaonei.reg.guide.factory.NetBarUserViewFactory;
import com.xiaonei.reg.guide.model.NetBarUserView;
import com.xiaonei.search2.CompoundQuery;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.NetbarSearchCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;


public class AjaxGuideHotStarAction extends BaseAction {
    public ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException{ 
        User host = BaseThreadUtil.currentHost(request);
        int one=1,two=2,pageIndex=0;
        if(request.getParameter("pageIndex")==null){
            pageIndex=0;
        }
        else{
            pageIndex=Integer.parseInt(request.getParameter("pageIndex"));
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
            request.setAttribute("guideKaixinHotStarList", list);  
        }
        catch(Exception e){
            e.printStackTrace();
        }
        return mapping.findForward("succ");
    }
}
        
