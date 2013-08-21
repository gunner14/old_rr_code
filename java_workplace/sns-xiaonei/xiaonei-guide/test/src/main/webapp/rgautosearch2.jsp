<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">



<%@page import="mop.hi.oce.adapter.AdapterFactory"%>

<%@page import="com.xiaonei.search2.SearchResult"%>
<%@page import="com.dodoyo.friends.relationfriend.logic.SearchRelation"%>
<%@page import="com.xiaonei.search2.condition.GuideCondition"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.search2.model.IndexType"%>
<%@page import="com.xiaonei.search2.SearchViewFactory"%>
<%@page import="java.util.HashMap"%>
<%@page import="com.xiaonei.search2.model.client.SearchView"%>
<%@page import="com.dodoyo.friends.relationfriend.logic.SearchRelationFriend"%>
<%@page import="com.xiaonei.search2.model.UserFieldNames"%>
<%@page import="com.xiaonei.search2.SearchAdapter"%>
<%@page import="com.dodoyo.datum.model.RegionInfo"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.search2.condition.subcondition.RegionCondition"%>
<%@page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@page import="com.dodoyo.friends.relationfriend.logic.RelationSearch"%>
<%@page import="com.dodoyo.friends.relationfriend.logic.IRelationFriend"%>
<%@page import="java.util.List"%>
<%@page import="com.renren.sns.guide.model.GuideIRelation"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic"%>
<%@page import="com.renren.sns.guide.model.GuideSearchRelation"%>
<%@page import="com.renren.sns.guide.logic.GuideRelationSearch"%>
<%@page import="com.xiaonei.search2.condition.subcondition.HighSchoolCondition"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic"%>
<%@page import="mop.hi.oce.domain.network.HighSchoolInfo"%><form action="http://browse.renren.com/searchgd?s=0&ajax=1&q=chenzhou">
s:<input name="s" value="0" />
ajax:<input name="ajax" value="1" />
q:<input name="q" value="陈周" />
<input  type=submit />

</form>
<%
int offset = 0;
int limit = 52;
GuideCondition builder = new GuideCondition();
builder.addBirthYear(1985);
builder.addCurrentStage(20);

Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
factories.put(IndexType.User, new SearchViewFactory() {
    public SearchView createSearchView()  {
        return new SearchRelationFriend();
    }

    public String[] getRequiredFields() {
        return new String[] { UserFieldNames.ID, UserFieldNames.NAME,UserFieldNames.TINYURL, UserFieldNames.HEADURL };
    }

});



// 规整搜索结果
SearchRelation relation;

SearchResult result;
builder.addOpUser(81120);
// 搜索
result = SearchAdapter.getInstance().search(builder, factories, 0,51);

out.println("size:"+result.getContents().size());


builder = new GuideCondition();
User host = SnsAdapterFactory.getUserAdapter().get(81120);
RegionInfo regionInfo = DatumInfoClient.getInstance().getRegionInfo(host);
if (regionInfo != null && regionInfo.getProvince_name() != null
        && regionInfo.getCity_name() != null) {
    builder.addRegionInfo(new RegionCondition(0, regionInfo.getProvince_name(),
            regionInfo.getCity_name()));
} else {
    out.println("info:null"+"<br />");
}
relation = RelationSearch.getInstance().search(host.getId(), offset, limit, builder);
result = SearchAdapter.getInstance().search(builder, factories, offset,limit);
out.println("size:"+result.getContents().size()+"<br />");

int cnt = 0;
int innercnt = 0;
List<IRelationFriend> ifd = relation.getList();

out.println("r size:"+ifd.size()+" relation:"+relation.getCount()+"<br />");

List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(46, host, -1);
out.println("List<GuideIRelation> size:"+list.size()+"<br />");
for(GuideIRelation ir : list){
    out.println("ir:"+ir.getList().size()+"<br />");
}

builder = new GuideCondition();
HighSchoolInfo hsInfo = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
HighSchoolCondition condition = new HighSchoolCondition(hsInfo.getHighSchoolId(), hsInfo.getHighSchoolName().trim(), "", "", "", hsInfo.getEnrollYear());
builder.addHighSchoolInfo(condition);   

GuideSearchRelation guideSearchRelation = GuideRelationSearch.getInstance().search(81120, offset, limit, builder); 
out.println("guideSearchRelation:"+guideSearchRelation.getList().size()+"<br />");
%>