package com.xiaonei.reg.guide.flows.xfive.logics;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import javax.servlet.http.HttpServletRequest;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.root.interfaces.IInfoElementaryschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoSeniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoTechschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoUniversity;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoCurrentPlace;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoHomePlace;
import com.xiaonei.reg.guide.root.interfaces.IWorkInfoCompany;
import com.xiaonei.reg.guide.root.interfaces.IXProfilable;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.AbstractCondition;
import com.xiaonei.search2.condition.AbstractUserCondition;
import com.xiaonei.search2.condition.FilterSearchCondition;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;
import com.xiaonei.search2.condition.subcondition.ElementarySchoolCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.RegionCondition;
import com.xiaonei.search2.condition.subcondition.SubCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.UserFieldNames;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.iprecord.IPRecordAdapter;


public class GuideXFiveIDsPreviewLogic {
    static class SingletonHolder {
        static GuideXFiveIDsPreviewLogic instance = new GuideXFiveIDsPreviewLogic();
    }
    public static GuideXFiveIDsPreviewLogic getInstance() {
        return SingletonHolder.instance;
    }
    public List<Integer> getRegionResults(int count,User host){
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    	    GuideLogger.printLog(" host:"+host.getId());
    		RegionInfo info = GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);    		
    		cost.endStep();
    		if(info == null) return null;
    		GuideLogger.printLog(" host:"+host.getId());
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		builder.addBirthYear(BirthdayLogic.getInstance().getUserBasic(host).getBirthYear());
    		builder.addRegionInfo(new RegionCondition(0, info.getProvince_name(), info.getCity_name()));
    		GuideLogger.printLog(" host:"+host.getId()+" "+info.getProvince_name()+info.getCity_name());
    		
    		RegionInfo home = GuideXFiveUserProfileLogic.getInstance().loadHomeRegion(host);
    		GuideLogger.printLog("");
    		if(home != null){
    			//加入家乡 的条件
    			GuideLogger.printLog(" host:"+host.getId()+" "+home.getProvince_name()+home.getCity_name());
    			builder.addHomeProvinceMust(home.getProvince_name());
    			builder.addHomeCityShould(home.getCity_name());
    			//list = searchG(count, builder);
    			list = timedSearch(host, builder, count, "getRegionResults1");
    		}
    		else{
    			//list = searchG(count, builder);
    			list = timedSearch(host, builder, count, "getRegionResults2");
    		}
    		GuideLogger.printLog(" host:"+host.getId()+" ");
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
		cost.endStep();
    	cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getWorkResults(int count, User host) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		WorkplaceInfo cos = GuideXFiveUserProfileLogic.getInstance().loadWorkplace(host);
    		cost.endStep();
    		if(cos == null) return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		builder.addWorkplaceInfo(new WorkplaceCondition(0, cos.getWorkplaceName()));
    		//list = searchG(count, builder);
			list = timedSearch(host, builder, count, "getWorkResults");
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	cost.endStep();
    	cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getCollegeResults(int count, User host) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
		try {
			CollegeInfo cos = GuideXFiveUserProfileLogic.getInstance().loadTechschool(host);
			cost.endStep();
			if(cos == null)return null;
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addCollegeInfo(new CollegeCondition(0, cos.getCollegeName(), cos.getDepartment(), cos.getEnrollYear()));
			//list = searchG(count, builder);
			list = timedSearch(host, builder, count, "getCollegeResults");
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getJuniorResults(int count, User host) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
			JuniorHighSchoolInfo jhsInfo = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
			cost.endStep();
			if(jhsInfo == null) return null;
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(
					jhsInfo.getJunHighentarySchoolId(), null, jhsInfo
							.getJunHighentarySchoolYear()));
			//list = searchG(count, builder);
			list = timedSearch(host, builder, count, "getJuniorResults");
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
    }
    
    public List<Integer> getElementaryResults(int count, User host){
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		ElementarySchoolInfo info = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
    		cost.endStep();
			if(info == null){
				GuideLogger.printLog(" host:"+host.getId()+" elem null");
				return null;
			}
			GuideLogger.printLog(" host:"+host.getId()+" info:"+info.getElementarySchoolName()+info.getElementarySchoolYear()+"-"+info.getElementarySchoolId());
			
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addElementarySchoolInfo(new ElementarySchoolCondition(0, info.getElementarySchoolName(), info.getElementarySchoolYear()));//小学没有id，所以传入0
			//list = searchG(count, builder);
			list = timedSearch(host, builder, count, "getElementaryResults");
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
    }
    
    public List<Integer> getHighschoolResults(int count, User host) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		HighSchoolInfo highSchoolInfo = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
    		cost.endStep();
    		if(highSchoolInfo == null)return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		GuideLogger.writeLine(this,"getHighschoolResults() - "+
    				highSchoolInfo.getHighSchoolName()+
    				highSchoolInfo.getHClass1()+
    				highSchoolInfo.getHClass2()+
    				highSchoolInfo.getHClass3());
    		builder.addHighSchoolInfo(new HighSchoolCondition(highSchoolInfo.getId(), 
    				highSchoolInfo.getHighSchoolName(), 
    				//highSchoolInfo.getHClass1(), highSchoolInfo.getHClass2(), highSchoolInfo.getHClass3(),
    				null,null,null,
    				highSchoolInfo.getEnrollYear()));
			
    		//list = searchG(count, builder);
			list = timedSearch(host, builder, count, "getHighschoolResults");
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
	}
    
    public List<Integer> getUniversityResults(int count, User host) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		UniversityInfo univInfo = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
    		cost.endStep();
    		if(univInfo == null)return null;
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
			list = timedSearch(host, builder, count, "getUniversityResults");
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
    }
    
    private List<Integer> getIntegerList(List<SearchView> list){
    	GuideTimeCost cost = GuideTimeCost.begin();
    	if(list == null) return null;
    	List<Integer> ilist = new ArrayList<Integer>();
    	if(list != null){
    		for(SearchView v : list){
    			if(ilist.contains(v.getId())){
    			}
    			else{
    				ilist.add(v.getId());
    			}
    		}
    	}
    	cost.endFinally();
    	return ilist;
    }
    
    public List<Integer> getIntegerList(int []arr){
        GuideTimeCost cost = GuideTimeCost.begin();
        if(arr == null || arr.length == 0) return null;
        List<Integer> iarr = new ArrayList<Integer>();
        if(arr != null){
            for(int v : arr){
                if(iarr.contains(v)){                    
                }
                else{
                    iarr.add(v);
                }
            }
        }
        cost.endFinally();
        return iarr;
    }
    
	@SuppressWarnings("unused")
	private List<SearchView> searchG(int count,	AbstractCondition builder) {
		GuideTimeCost cost = GuideTimeCost.begin();
		Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
		factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
		SearchResult result = null;
		//SearchAdapter adapter = SearchAdapter.getInstance();
		//2009-11-13 12:12:24 将SearchAdapter换成SNS自己的封装，为了避堵
		ISearchAdapter adapter = SnsAdapterFactory.getSearchAdapter();
		try {
			result = adapter.search(builder, factories, 0, count);
			cost.endStep("after search");
		} catch (Exception e) {
			GuideLogger.printLog(""+" - err -:"+e.toString()+"["+(adapter==null)+"]",GuideLogger.ERROR);
			e.printStackTrace(System.err);
		}
		if (null != result) {
			return result.getContents();
		}
		cost.endFinally();
		return null;
	}
	
	
	
	/**
	 * 相同 ip 推人
	 * @param count
	 * @param host
	 * @return
	 */
	public List<Integer> getSameIpResults(int count, User host, HttpServletRequest req) {
		GuideTimeCost cost = GuideTimeCost.begin();
		cost.endStep();
	    String ip = BizFilterUtil.getClientIP(req);
	    cost.endStep();
	    int[] co = IPRecordAdapter.getInstance().getUsers(ip, count);
	    cost.endStep();
	    GuideLogger.printLog( "[" + host.getId() + "] ip is :"+ ip +",user friends:"+ (co==null?-1:co.length));
	    cost.endFinally();
	    return getIntegerList(co);
	}
	
	@SuppressWarnings("unused")
	private String printArr(int []arr){
	    if(arr == null || arr.length ==0) return "";
	    StringBuilder sb = new StringBuilder("");
	    for(int i : arr){
	        sb.append(i).append(",");
	    }
	    return sb.toString();
	}
	
	
	public static final int MC_UNIV = 0;
	public static final int MC_SENIOR = 1;
	public static final int MC_JUNIOR = 2;
	public static final int MC_TECH = 3;
	public static final int MC_ELEM = 4;
	public static final int MC_COMPANY = 5;
	public static final int MC_REGION = 6;
	
	private static ExecutorService es = Executors.newCachedThreadPool();
	 

	public List<Integer> getSearchfriendResults(int count, User host, String name, Map<String,IXProfilable> param) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	if(param == null) {
    		GuideLogger.printLog(" host:"+host.getId()+" param null",GuideLogger.WARN );
    		return null;
    	}
    	try {
    		IInfoUniversity info_dx = (IInfoUniversity)param.get("info_dx");
    		IInfoSeniorhighschool info_gz = (IInfoSeniorhighschool)param.get("info_gz");
    		IInfoJuniorhighschool info_cz = (IInfoJuniorhighschool)param.get("info_cz");
    		IInfoTechschool info_jx = (IInfoTechschool)param.get("info_jx");
    		IInfoElementaryschool info_xx = (IInfoElementaryschool)param.get("info_xx");
    		IWorkInfoCompany info_gs = (IWorkInfoCompany)param.get("info_gs");
    		IRegionInfoCurrentPlace info_dd = (IRegionInfoCurrentPlace)param.get("info_dd");
    		IRegionInfoHomePlace info_gx = (IRegionInfoHomePlace)param.get("info_gx");
    		String year_must = info_dx != null && info_dx.getUniversityyear() == 0? "" : UserFieldNames.UNIVERSITY_INFO_YEAR_FLAG_MUST;
    		
    		int flag = -1;
    		if(info_dx != null){
    			flag = MC_UNIV;
    		}
    		else if(info_gz != null){
    			flag = MC_SENIOR;
    		}
    		else if(info_cz != null){
    			flag = MC_JUNIOR;
    		}
    		else if(info_jx != null){
    			flag = MC_TECH;
    		}
    		else if(info_xx != null){
    			//flag = MC_ELEM; //小学作为必要条件不太靠谱……
    		}
    		else if(info_gs != null){
    			flag = MC_COMPANY;
    		}
    		else if(info_dd != null){
    			flag = MC_REGION;
    		}
    		
    		StringBuilder sconditions = new StringBuilder();
    		sconditions.append("getSearchfriendResults:["+host.getStage()+"]");
    		if(info_dx != null){
    			sconditions.append("dx,");
    		}
    		if(info_gz != null){
    			sconditions.append("gz,");
    		}
    		if(info_cz != null){
    			sconditions.append("cz,");
    		}
    		if(info_jx != null){
    			sconditions.append("jx,");
    		}
    		if(info_xx != null){
    			sconditions.append("xx,");
    		}
    		if(info_gs != null){
    			sconditions.append("gs,");
    		}
    		if(info_dd != null){
    			sconditions.append("dd,");
    		}
    		if(info_gx != null){
    			//sconditions.append("gx,");
    			GuideLogger.printLog(" host:"+host.getId()+" gx:"+info_gx.getHomeprovince()+","+info_gx.getHomecityname() );
    		}
    		
    		GuideLogger.printLog(" host:"+host.getId()+" flag:"+flag );
    		
    		
    		SubCondition [] conditions = new SubCondition [7];
    		conditions[0] = info_dx==null?null:new UniversityCondition(info_dx.getUniversitycode(),info_dx.getUniversityname(),info_dx.getUniversitydepartment(),null,info_dx.getUniversityyear(),year_must);
    		conditions[1] = info_gz==null?null:new HighSchoolCondition(info_gz.getHighschoolcode(),info_gz.getHighschoolname(),info_gz.getHighschoolclass1(),info_gz.getHighschoolclass2(),info_gz.getHighschoolclass3(),info_gz.getHighschoolyear());
    		conditions[2] = info_cz==null?null:new JuniorHighSchoolCondition(info_cz.getJuniorhighschoolcode(), null, info_cz.getJuniorhighschoolyear());
    		conditions[3] = info_jx==null?null:new CollegeCondition(0, info_jx.getTechschoolname(), null, info_jx.getTechschoolyear());
    		conditions[4] = info_xx==null?null:new ElementarySchoolCondition(0, info_xx.getElementaryschoolname(), info_xx.getElementaryschoolyear());
    		conditions[5] = info_gs==null?null:new WorkplaceCondition(0, info_gs.getCompanyname());
    		conditions[6] = info_dd==null?null:new RegionCondition(0, info_dd.getCurrentprovince(), info_dd.getCurrentcityname());
    		
    		cost.endStep();
    		FilterSearchCondition builder = new FilterSearchCondition();
    		for (int i = 0; i < conditions.length; i++) {
    			if(conditions[i] == null) {
    				//GuideLogger.printLog(" host:"+host.getId()+" <"+i+"> null");
    				continue;
    			}
    			GuideLogger.printLog(" host:"+host.getId()+" <"+i+"> exists");
				SubCondition sc = conditions[i];
				builder.addSubCondition(sc,i != flag);
				if(info_gx != null && info_gs == null && info_dx == null && info_gz == null && info_cz == null && info_jx == null){
					sconditions.append("gx,");
					builder.addHomeProvince(info_gx.getHomeprovince());
					builder.addHomeCity(info_gx.getHomecityname());
				}
			}
    		if(!MyStringUtils.isNullOrEmpty(name)){
    			builder.addName(name);
    		}
    		builder.addOpUser(host.getId());
    		list = timedSearch(host,builder,count,sconditions.toString());
    		
		} 
    	catch (Exception e) {
			GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
    }
	

	public List<Integer> getProfileResults(int count, User host,int flag_unuse) {
    	GuideTimeCost cost = GuideTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		
    		//addSubCondition(SubCondition condition, boolean should)
    		// 第二个参数如果为true则是排序条件，否则是过滤条件
    		/*UniversityInfo info_university = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
    		HighSchoolInfo info_senior = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
    		JuniorHighSchoolInfo info_junior = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
    		CollegeInfo info_tech = GuideXFiveUserProfileLogic.getInstance().loadTechschool(host);
    		ElementarySchoolInfo info_elem = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
    		WorkplaceInfo info_work = GuideXFiveUserProfileLogic.getInstance().loadWorkplace(host);
    		RegionInfo info_region = GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);*/
    		
    		IInfoUniversity info_dx = GuideXFiveUserInfoLogic.getInstance().getInfoUniversity(host);
    		IInfoSeniorhighschool info_gz = GuideXFiveUserInfoLogic.getInstance().getInfoSeniorhighschool(host);
    		IInfoJuniorhighschool info_cz = GuideXFiveUserInfoLogic.getInstance().getInfoJuniorhighschool(host);
    		IInfoTechschool info_jx = GuideXFiveUserInfoLogic.getInstance().getInfoTechschool(host);
    		IInfoElementaryschool info_xx = GuideXFiveUserInfoLogic.getInstance().getInfoElementaryschool(host);
    		IWorkInfoCompany info_gs = GuideXFiveUserInfoLogic.getInstance().getWorkInfoCompany(host);
    		IRegionInfoCurrentPlace info_dd = GuideXFiveUserInfoLogic.getInstance().getRegionInfoCurrentPlace(host);
    		IRegionInfoHomePlace info_gx = GuideXFiveUserInfoLogic.getInstance().getRegionInfoHomePlace(host);
    		String year_must = info_dx != null && info_dx.getUniversityyear() == 0? "" : UserFieldNames.UNIVERSITY_INFO_YEAR_FLAG_MUST;
    		
    		int flag = -1;
    		if(info_gs != null){
    			flag = MC_COMPANY;
    		}
    		else if(info_dx != null){
    			flag = MC_UNIV;
    		}
    		else if(info_gz != null){
    			flag = MC_SENIOR;
    		}
    		else if(info_cz != null){
    			flag = MC_JUNIOR;
    		}
    		else if(info_jx != null){
    			flag = MC_TECH;
    		}
    		else if(info_xx != null){
    			//flag = MC_ELEM; //小学作为必要条件不太靠谱……
    		}
    		else if(info_dd != null){
    			flag = MC_REGION;
    		}
    		
    		StringBuilder sconditions = new StringBuilder();
    		sconditions.append("getProfileResults:");
    		if(info_gs != null){
    			sconditions.append("gs,");
    		}
    		if(info_dx != null){
    			sconditions.append("dx,");
    		}
    		if(info_gz != null){
    			sconditions.append("gz,");
    		}
    		if(info_cz != null){
    			sconditions.append("cz,");
    		}
    		if(info_jx != null){
    			sconditions.append("jx,");
    		}
    		if(info_xx != null){
    			sconditions.append("xx,");
    		}
    		if(info_dd != null){
    			sconditions.append("dd,");
    		}
    		if(info_gx != null){
    			//sconditions += "gx,";
    			GuideLogger.printLog(" host:"+host.getId()+" gx:"+info_gx.getHomeprovince()+","+info_gx.getHomecityname() );
    		}
    		
    		GuideLogger.printLog(" host:"+host.getId()+" flag:"+flag );
    		
    		
    		SubCondition [] conditions = new SubCondition [7];
    		conditions[0] = info_dx==null?null:new UniversityCondition(info_dx.getUniversitycode(),info_dx.getUniversityname(),info_dx.getUniversitydepartment(),null,info_dx.getUniversityyear(),year_must);
    		conditions[1] = info_gz==null?null:new HighSchoolCondition(info_gz.getHighschoolcode(),info_gz.getHighschoolname(),null,null,null,info_gz.getHighschoolyear());
    		conditions[2] = info_cz==null?null:new JuniorHighSchoolCondition(info_cz.getJuniorhighschoolcode(), null, info_cz.getJuniorhighschoolyear());
    		conditions[3] = info_jx==null?null:new CollegeCondition(0, info_jx.getTechschoolname(), null, info_jx.getTechschoolyear());
    		conditions[4] = info_xx==null?null:new ElementarySchoolCondition(0, info_xx.getElementaryschoolname(), info_xx.getElementaryschoolyear());
    		conditions[5] = info_gs==null?null:new WorkplaceCondition(0, info_gs.getCompanyname());
    		conditions[6] = info_dd==null?null:new RegionCondition(0, info_dd.getCurrentprovince(), info_dd.getCurrentcityname());
    		
    		cost.endStep();
    		GuideCondition builder = new GuideCondition();
    		for (int i = 0; i < conditions.length; i++) {
    			if(conditions[i] == null) {
    				//GuideLogger.printLog(" host:"+host.getId()+" <"+i+"> null");
    				continue;
    			}
    			GuideLogger.printLog(" host:"+host.getId()+" <"+i+"> exists");
				SubCondition sc = conditions[i];
				builder.addSubCondition(sc,i != flag);
				if(info_gx != null && host.getStage() == 90 && !hasGoodEducationInfo(conditions)){
					sconditions.append(90+"("+i+"),");
					builder.addHomeProvinceMust(info_gx.getHomeprovince());
					builder.addHomeCityMust(info_gx.getHomecityname());
					builder.addCurrentStage(90);
				}
				if(info_gz != null){
					SubCondition info_gz_class = new HighSchoolCondition(info_gz.getHighschoolcode(),info_gz.getHighschoolname(),info_gz.getHighschoolclass1(),info_gz.getHighschoolclass2(),info_gz.getHighschoolclass3(),info_gz.getHighschoolyear());
					builder.addSubCondition(info_gz_class, true);
				}
			}
    		builder.addOpUser(host.getId());
    		//list = searchG(count, builder);
    		//Future<List<SearchView>> resultset = es.submit(new DoSearchResultsCallable(host, builder, count));
    		//list = resultset.get(555, TimeUnit.MILLISECONDS);
    		list = timedSearch(host,builder,count,sconditions.toString());
    		
		} 
    	catch (Exception e) {
			GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endStep();
		cost.endFinally();
		return getIntegerList(list);
    }
	
	private boolean hasGoodEducationInfo(SubCondition[] conditions) {
		for (int i = 0; i < conditions.length; i++) {
			SubCondition sub = conditions[i];
			if(i <= 3){
				if(sub != null){
					return true;
				}
			}
			else{
				break;
			}
			
		}
		return false;
	}
	private List<SearchView> timedSearch(User host, AbstractUserCondition builder,	int count, String sconditions) {
		Future<List<SearchView>> resultset = es.submit(new DoSearchResultsCallable(host, builder, count));
		List<SearchView> list = null;
		long cost = -1;
		try {
			long begin = System.currentTimeMillis();
			list = resultset.get(200, TimeUnit.MILLISECONDS);
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
		return list;
	}

	public static class DoSearchResultsCallable implements Callable<List<SearchView>> {
		private User host;
		private AbstractUserCondition builder;
		private int count;

		public DoSearchResultsCallable(User host,AbstractUserCondition builder, int count) {
			this.host = host;
			this.builder = builder;
			this.count = count;
		}

		public List<SearchView> call() {
			GuideTimeCost cost = GuideTimeCost.begin();
			Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
			factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
			SearchResult result = null;
			ISearchAdapter adapter = SnsAdapterFactory.getSearchAdapter();
			cost.endStep();
			try {
				result = adapter.search(builder, factories, 0, count);
				cost.endStep("after search");
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
			
			cost.endFinally();
			return contents;
		}
	}
    public static void main(String[] args) throws UnsupportedEncodingException {
    	
		System.out.println(native2ascii1("中4中@#$#$^%^中sdfsd\u4e2d"));
		System.out.println(native2ascii("中4中@#$#$^%^中sdfsd\u4e2d"));
	}
    
    private static String native2ascii1(String src) {
    	if(src == null) return src;
    	String des = "";
    	/*int length = src.length();
    	for (int i = 0; i < length; i++) {
    		char cvalue = (char) src.charAt(i);
    		int ivalue = (int) src.charAt(i);
    		if(ivalue >= 19968 && ivalue <= 171941)
    			des += "\\u" + Integer.toString(cvalue, 16);
    		else
    			des += cvalue;
    	}*/
    	return des;
    }
    
    private static String native2ascii(String src) {
		String des = "";
		/*char[] chars = new char[src.length()];
		for (int i = 0; i < chars.length; i++) {
			chars[i] = (char) src.charAt(i);
			int ivalue = (int) chars[i];
			if(ivalue >= 19968 && ivalue <= 171941)
				des += "\\u" + Integer.toString(chars[i], 16);
			else
				des += chars[i];
		}*/
		return des;
	}
    
    public static String chineseToAscii(String s) {
		try {
			byte[] src = s.getBytes("UTF8");
			char[] des = new char[src.length];
			for (int i = 0; i < src.length; i++) {
				des[i] = (char) (src[i] & 0xFF);
			}
			return new String(des);
		} catch (UnsupportedEncodingException ex) {
			return s;
		}
	}
}

