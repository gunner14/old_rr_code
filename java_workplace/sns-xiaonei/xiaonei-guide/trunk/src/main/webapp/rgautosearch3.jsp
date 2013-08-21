<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="java.util.Date"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@page import="net.spy.memcached.MemcachedClient"%>
<%@page import="com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.search2.model.client.SearchView"%>
<%@page import="com.xiaonei.search2.condition.AbstractUserCondition"%>
<%@page import="java.util.concurrent.Future"%>
<%@page import="java.util.concurrent.Executors"%>
<%@page import="java.util.concurrent.ExecutorService"%>
<%@page import="java.util.concurrent.TimeUnit"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%>
<%@page import="java.util.concurrent.ExecutionException"%>
<%@page import="java.util.concurrent.TimeoutException"%>
<%@page import="java.util.concurrent.Callable"%>
<%@page import="com.xiaonei.search2.model.IndexType"%>
<%@page import="com.xiaonei.search2.SearchViewFactory"%>
<%@page import="com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory"%>
<%@page import="com.xiaonei.search2.SearchResult"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic"%>
<%@page import="com.xiaonei.search2.condition.GuideCondition"%>
<%@page import="com.xiaonei.search2.model.UserFieldNames"%>
<%@page import="com.xiaonei.search2.condition.subcondition.UniversityCondition"%><html>

<%!static long COST = 0; %>
<%
List<SearchView> list = null;
User host = SnsAdapterFactory.getUserAdapter().get(81120);
try {
    UniversityInfo univInfo = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
    if(univInfo == null)return;
    if ("".equals(univInfo.getUniversityName())) {
        throw new Exception();
    }
    GuideCondition builder = new GuideCondition();
    builder.addOpUser(host.getId());
    String yearmust = univInfo.getEnrollYear() == 0? "" : UserFieldNames.UNIVERSITY_INFO_YEAR_FLAG_MUST;
    builder.addUniversityInfo(new UniversityCondition(univInfo
            .getUniversityId(), univInfo.getUniversityName(),
            univInfo.getDepartment(), null, univInfo
                    .getEnrollYear(),yearmust));
    //list = searchG(count, builder);
    list = timedSearch(host, builder, 50, "getUniversityResults");
    out.println("ret:"+list);
    if(list!=null){
        out.println("size:"+list.size());
    }
    out.println("<br />cost:"+COST);
} catch (Exception e) {
    e.printStackTrace();
}
%>
<%!private List<SearchView> timedSearch(User host, AbstractUserCondition builder,  int count, String sconditions) {
    ExecutorService es = Executors.newCachedThreadPool();
    Future<List<SearchView>> resultset = es.submit(new DoSearchResultsCallable(host, builder, count));
    List<SearchView> list = null;
    long cost = -1;
    try {
        long begin = System.currentTimeMillis();
        list = resultset.get(500, TimeUnit.MILLISECONDS);
        long end = System.currentTimeMillis();
        cost = end - begin;
    } catch (InterruptedException e) {
        GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        e.printStackTrace();
    } catch (ExecutionException e) {
        GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        e.printStackTrace();
    } catch (TimeoutException e) {
        GuideLogger.printLog(""+" host:"+host.getId()+" conditions:"+sconditions+" e:"+e.toString(),GuideLogger.ERROR);
        //e.printStackTrace();
        System.err.println(" host:"+host.getId()+" conditions:"+sconditions+" e:"+e.toString());
    }
    if(list == null){
        GuideLogger.printLog(""+" host:"+host.getId()+" conditions:"+sconditions+" getnull:0"+" cost:"+cost);
    }
    else{
        GuideLogger.printLog(""+" host:"+host.getId()+" conditions:"+sconditions+" get:"+list.size()+" cost:"+cost);
    }
    COST = cost;
    return list;
}
%>
<%!public class DoSearchResultsCallable implements Callable<List<SearchView>> {
    private User host;
    private AbstractUserCondition builder;
    private int count;

    public DoSearchResultsCallable(User host,AbstractUserCondition builder, int count) {
        this.host = host;
        this.builder = builder;
        this.count = count;
    }

    public List<SearchView> call() {
        Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
        factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
        SearchResult result = null;
        ISearchAdapter adapter = SnsAdapterFactory.getSearchAdapter();
        try {
            result = adapter.search(builder, factories, 0, count);
        } catch (Exception e) {
            GuideLogger.printLog(""+" - err -:"+e.toString()+"["+(adapter==null)+"]",GuideLogger.ERROR);
            e.printStackTrace(System.err);
        }
        List<SearchView> contents = null;
        if (null != result) {
            
            contents = result.getContents();
        }
        else{
            contents = null;
        }
        
        if(contents == null){
            GuideLogger.printLog(" host:"+host.getId()+ " search:"+0+" size" );
        }
        else{
            GuideLogger.printLog(" host:"+host.getId()+ " search:"+contents.size());
        }
        
        return contents;
    }
}
%>