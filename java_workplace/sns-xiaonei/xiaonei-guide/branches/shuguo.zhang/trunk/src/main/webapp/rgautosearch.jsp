
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%>
<%@page import="com.xiaonei.reg.guide.util.GuideUtil"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%>
<%@page import="com.xiaonei.search2.condition.OpenPeopleSearchCondition"%>
<%@page import="com.xiaonei.search2.condition.subcondition.RegionCondition"%>
<%@page import="com.dodoyo.datum.model.RegionInfo"%>
<%@page import="com.xiaonei.search2.condition.subcondition.UniversityCondition"%>
<%@page import="com.xiaonei.reg.guide.action.a090821.UserProfileUtil"%>
<%@page import="com.xiaonei.search2.SearchAdapter"%>
<%@page import="com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory"%>
<%@page import="com.xiaonei.search2.SearchViewFactory"%>
<%@page import="com.xiaonei.search2.model.IndexType"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.search2.SearchResult"%>
<%@page import="com.xiaonei.search2.condition.subcondition.WorkplaceCondition"%>
<%@page import="com.xiaonei.search2.condition.GuideCondition"%>
<%@page import="mop.hi.oce.adapter.AdapterFactory"%>
<%@page import="mop.hi.oce.domain.network.WorkplaceInfo"%>
<%@page import="com.xiaonei.search2.model.client.SearchView"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@page import="com.dodoyo.datum.client.DatumNetworkClient"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="java.util.List"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>
<%@page import="com.xiaonei.platform.component.feed.api.IGuideRecommend"%>
<%@page import="com.xiaonei.platform.component.feed.api.IGuideRecommend.GuideRecommendModel"%>
<%@page import="com.xiaonei.reg.guide.logic.search.GuideFriendNewsLogic"%>
<%@page import="com.xiaonei.reg.guide.action.a090729.A090729Util"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>


<%
if(!"true".equals(request.getParameter("run"))){
	return;
}

int id = 81120;
int count = 111;
List<SearchView> list = null;
User host = WUserAdapter.getInstance().get(id);
try {
	/*
}
	List<WorkplaceInfo> cos = AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(id);
	if(cos == null || cos.size() == 0) {
		out.println(id+" no work!");
	}
	else{
		for(WorkplaceInfo wf : cos){
			out.println(wf.getWorkplaceName());
		}
	}
	GuideCondition builder = new GuideCondition();
	builder.addOpUser(id);
	builder.addWorkplaceInfo(new WorkplaceCondition(cos.get(0).getWorkplaceId(), cos.get(0).getWorkplaceName()));
	//list = searchG( 100, builder);
	*/
	/*
	RegionInfo info = UserProfileUtil.loadRegion(host);
	if(info == null) return;
	GuideCondition builder = new GuideCondition();
	builder.addOpUser(host.getId());
	out.println("id:"+info.getId()+info.getCity_name()+info.getProvince_name()+"<br/>");
	builder.addRegionInfo(new RegionCondition(0, info.getProvince_name(), info.getCity_name()));
	
	Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
	factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
	SearchResult result = null;
	result = SearchAdapter.getInstance().search(builder, factories, 0, count);
	out.println("work count:"+result.getTotalCount()+"<br />");
	}
	catch(Exception e){
		
	}
	*/
	OpenPeopleSearchCondition condition = null;
    try {
        condition = new OpenPeopleSearchCondition();
        //condition.addOpUser(0);
        condition.add("北京邮电大学");
        UniversityCondition subcondition = new UniversityCondition(0, null, null, null, 2006);
        condition.addSubCondition(subcondition);
        //UniversityCondition subcondition = new UniversityCondition(0, "北京邮电大学", null, null, 2006);
        //condition.addSubCondition(subcondition);
    } catch (Exception e) {
        GuideLogger.printLog("tellUniversity() - conds - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
        e.printStackTrace();
    }
    
    /*
    SearchBuilder sb = new PeopleSearch
    sb.addConditions(param);
    sb.addOpUser(hostId);
    sb.addQuery(query);*/
    
    Map<IndexType, SearchViewFactory> factories = null;
    try {
        factories = new HashMap<IndexType, SearchViewFactory>();
        factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
        
    } catch (Exception e) {
        GuideLogger.printLog("tellUniversity() - factory - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
        e.printStackTrace();
    }
    SearchResult ret = SnsAdapterFactory.getSearchAdapter().search(condition, factories, 0, 100);
    List<SearchView> retlist = ret.getContents();
    List<Integer> idlist = new ArrayList<Integer>();
    for(SearchView v : retlist){
    	idlist.add(v.getId());
    }
    out.println("all:"+idlist.size()+"<br />");
    Map<Integer, WUserCache> ucl = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(idlist);
    for(Map.Entry<Integer,WUserCache> user : ucl.entrySet()){
    	WUserCache userca = user.getValue();
    	out.println(userca.getId()+":"+userca.getName()+":"+userca.getUnivName()+"<br />");
    }
    
}
catch(Exception e){
	
}
%>