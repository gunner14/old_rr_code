package com.xiaonei.reg.guide.logic.preview;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileCacheLogic;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.AbstractCondition;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;
import com.xiaonei.search2.condition.subcondition.ElementarySchoolCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.RegionCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.iprecord.IPRecordAdapter;


public class IDsPreviewLogic {
    static class SingletonHolder {
        static IDsPreviewLogic instance = new IDsPreviewLogic();
    }
    public static IDsPreviewLogic getInstance() {
        return SingletonHolder.instance;
    }
//    private static final ProfileLogic profileLogic = new ProfileLogic();
    
    public List<Integer> getRegionResults(int count,User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		RegionInfo info = GuideUserProfileCacheLogic.getInstance().getUserRegion(host);
    		if(info == null){
    			GuideLogger.printLog("Mem regionnull host:"+host.getId()+" stage:"+host.getStage(),GuideLogger.WARN);
    			info = UserProfileUtil.loadRegion(host);
    		}
    		if(info == null) return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		builder.addRegionInfo(new RegionCondition(0, info.getProvince_name(), info.getCity_name()));
    		list = searchG( count, builder);
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getRegionResults(int count,User host, RegionInfo info){
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
    		if(info == null) return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		builder.addRegionInfo(new RegionCondition(0, info.getProvince_name(), info.getCity_name()));
    		list = searchG( count, builder);
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getWorkResults(int count, User host) {
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
//    		List<WorkplaceInfo> cos = AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(host.getId());
    		WorkplaceInfo cos = UserProfileUtil.loadWork(host);
    		if(cos == null) return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		builder.addWorkplaceInfo(new WorkplaceCondition(0, cos.getWorkplaceName()));
    		list = searchG( count, builder);
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	cost.endFinally();
    	return getIntegerList(list);
    }
    public List<Integer> getCollegeResults(int count, User host) {
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
		try {
//			List<CollegeInfo> cos = AdapterFactory.getNetworkAdapter().getCollegeInfoList(host.getId());
//			if(cos == null || cos.size() == 0) return null;
			CollegeInfo cos = UserProfileUtil.loadCollege(host);
			if(cos == null)return null;
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addCollegeInfo(new CollegeCondition(0, cos.getCollegeName(), cos.getDepartment(), cos.getEnrollYear()));
			list = searchG( count, builder);
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
    	return getIntegerList(list);
    }
    
    public List<Integer> getJuniorResults(int count, User host) {
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
//			JuniorHighSchoolInfo jhsInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId());
			JuniorHighSchoolInfo jhsInfo = UserProfileUtil.loadJuniorhighschool(host);
			if(jhsInfo == null) return null;
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(
					jhsInfo.getJunHighentarySchoolId(), null, jhsInfo
							.getJunHighentarySchoolYear()));
			list = searchG( count, builder);
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
		return getIntegerList(list);
    }
    
    public List<Integer> getElementaryResults(int count, User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
//    		ElementarySchoolInfo info = AdapterFactory.getNetworkAdapter().getElementarySchoolInfo(host.getId());
    		ElementarySchoolInfo info = UserProfileUtil.loadElementaryschool(host);
			if(info == null) return null;
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addElementarySchoolInfo(new ElementarySchoolCondition(info.getElementarySchoolId(), info.getElementarySchoolName(), info.getElementarySchoolYear()));
			list = searchG( count, builder);
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
		return getIntegerList(list);
    }
    
    public List<Integer> getHighschoolResults(int count, User host) {
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
//    		List<HighSchoolInfo> highSchoolList = profileLogic.getHighschoolInfoList(host.getId());
//    		if(highSchoolList==null || highSchoolList.size() == 0 ) return null;
//    		HighSchoolInfo highSchoolInfo = highSchoolList.get(0);
    		HighSchoolInfo highSchoolInfo = UserProfileUtil.loadHighschool(host);
    		if(highSchoolInfo == null)return null;
    		GuideCondition builder = new GuideCondition();
    		builder.addOpUser(host.getId());
    		GuideLogger.writeLine(this,"getHighschoolResults() - "+
    				highSchoolInfo.getHighSchoolName()+
    				highSchoolInfo.getHClass1()+
    				highSchoolInfo.getHClass2()+
    				highSchoolInfo.getHClass3());
    		builder.addHighSchoolInfo(new HighSchoolCondition(0, 
    				highSchoolInfo.getHighSchoolName(), 
    				null, null, null,
    				//2010-2-23 16:01:08 谢锟让去掉的
    				//highSchoolInfo.getHClass1(), highSchoolInfo.getHClass2(), highSchoolInfo.getHClass3(), 
    				highSchoolInfo.getEnrollYear()));
			
    		list = searchG(count, builder);
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
		return getIntegerList(list);
	}
    
    public List<Integer> getUniversityResults(int count, User host) {
    	MyTimeCost cost = MyTimeCost.begin();
    	List<SearchView> list = null;
    	try {
//    		List<UniversityInfo> ulist = profileLogic.getUniversityInfoList(host.getId());
//    		if(ulist==null || ulist.size()==0)return null;
//			UniversityInfo univInfo = ulist.get(0);
    		UniversityInfo univInfo = UserProfileUtil.loadUniversity(host);
    		if(univInfo == null)return null;
			if ("".equals(univInfo.getUniversityName())) {
				throw new Exception();
			}
			GuideCondition builder = new GuideCondition();
			builder.addOpUser(host.getId());
			builder.addUniversityInfo(new UniversityCondition(univInfo
					.getUniversityId(), univInfo.getUniversityName(),
					univInfo.getDepartment(), null, univInfo
							.getEnrollYear()));
			list = searchG(count, builder);
		} catch (Exception e) {
			e.printStackTrace();
		}
		cost.endFinally();
		return getIntegerList(list);
    }
    
    private List<Integer> getIntegerList(List<SearchView> list){
    	MyTimeCost cost = MyTimeCost.begin();
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
    
    private List<Integer> getIntegerList(int []arr){
        MyTimeCost cost = MyTimeCost.begin();
        if(arr == null) return null;
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
    
	private List<SearchView> searchG(int count,	AbstractCondition builder) {
		MyTimeCost cost = MyTimeCost.begin();
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
	 * 大学推人
	 * 同时搜索大学&高中
	 * @param count
	 * @param host
	 * @return
	 */
	@SuppressWarnings("unused")
	private List<Integer> getUniversityHighschoolResults(int count, User host) {
		/*
        MyTimeCost cost = MyTimeCost.begin();
        List<SearchView> list = null;
        try {
            HighSchoolInfo highSchoolInfo = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
            UniversityInfo univInfo = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
            if(univInfo == null && highSchoolInfo == null) return null;
            
            GuideCondition builder = new GuideCondition();
            builder.addOpUser(host.getId());
            builder.addUniversityInfo(new UniversityCondition(univInfo
                    .getUniversityId(), univInfo.getUniversityName(),
                    univInfo.getDepartment(), null, univInfo
                            .getEnrollYear()));
            builder.addHighSchoolInfo(new HighSchoolCondition(0, 
                    highSchoolInfo.getHighSchoolName(), 
                    highSchoolInfo.getHClass1(), highSchoolInfo.getHClass2(), highSchoolInfo.getHClass3(), 
                    highSchoolInfo.getEnrollYear()));
            
            list = searchG(count, builder);
        } catch (Exception e) {
            e.printStackTrace();
        }
        cost.endFinally();
        return getIntegerList(list);
        */
		return null;
    }
	
	/**
	 * 高中推人
	 * 同时搜索高中&初中
	 * @param count
	 * @param host
	 * @return
	 */
	@SuppressWarnings("unused")
	private List<Integer> getHighAndJuniorHightschoolResults(int count, User host) {
		/*
        MyTimeCost cost = MyTimeCost.begin();
        List<SearchView> list = null;
        try {
            HighSchoolInfo highSchoolInfo = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
            JuniorHighSchoolInfo jhsInfo = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
            
            if(highSchoolInfo == null && jhsInfo == null) return null;
            GuideCondition builder = new GuideCondition();
            builder.addOpUser(host.getId());
            GuideLogger.writeLine(this,"getHighAndJuniorHightschoolResults() - "+
                    highSchoolInfo.getHighSchoolName()+
                    highSchoolInfo.getHClass1()+
                    highSchoolInfo.getHClass2()+
                    highSchoolInfo.getHClass3());
            builder.addHighSchoolInfo(new HighSchoolCondition(0, 
                    highSchoolInfo.getHighSchoolName(), 
                    highSchoolInfo.getHClass1(), highSchoolInfo.getHClass2(), highSchoolInfo.getHClass3(), 
                    highSchoolInfo.getEnrollYear()));
            builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(
                    jhsInfo.getJunHighentarySchoolId(), null, jhsInfo
                            .getJunHighentarySchoolYear()));
            list = searchG(count, builder);
        } catch (Exception e) {
            e.printStackTrace();
        }
        cost.endFinally();
        return getIntegerList(list);
        */
		return null;
    }
	
	/**
	 * 初中推人
	 * 同时搜索初中&小学
	 * @param count
	 * @param host
	 * @return
	 */
	@SuppressWarnings("unused")
	private List<Integer> getHightschoolAndElementaryResults(int count, User host) {
		/*
        MyTimeCost cost = MyTimeCost.begin();
        List<SearchView> list = null;
        try {
            JuniorHighSchoolInfo jhsInfo = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
            ElementarySchoolInfo einfo = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
            
            if(einfo == null && jhsInfo == null) return null;
            GuideCondition builder = new GuideCondition();
            builder.addOpUser(host.getId());
            
            builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(
                    jhsInfo.getJunHighentarySchoolId(), null, jhsInfo
                            .getJunHighentarySchoolYear()));
            builder.addElementarySchoolInfo(new ElementarySchoolCondition(einfo.getElementarySchoolId(), einfo.getElementarySchoolName(), einfo.getElementarySchoolYear()));
            list = searchG(count, builder);
        } catch (Exception e) {
            e.printStackTrace();
        }
        cost.endFinally();
        return getIntegerList(list);
        */
		return null;
    }
	
	/**
	 * 相同 ip 推人
	 * @param count
	 * @param host
	 * @return
	 */
	public List<Integer> getSameIpResults(int count, User host, HttpServletRequest req) {
	    String ip = BizFilterUtil.getClientIP(req);
	    int[] co = IPRecordAdapter.getInstance().getUsers(ip, count);
	    GuideLogger.writeLine(this, "ip is :"+ ip +",user ids ::"+ this.printArr(co));
	    return getIntegerList(co);
	}
	
	private String printArr(int []arr){
	    if(arr == null || arr.length ==0) return "";
	    StringBuilder sb = new StringBuilder("");
	    for(int i : arr){
	        sb.append(i).append(",");
	    }
	    return sb.toString();
	}
}
