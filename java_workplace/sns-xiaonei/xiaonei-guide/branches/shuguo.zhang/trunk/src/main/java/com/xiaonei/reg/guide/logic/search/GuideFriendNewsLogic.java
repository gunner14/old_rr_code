package com.xiaonei.reg.guide.logic.search;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.xiaonei.platform.component.feed.api.IGuideRecommend;
import com.xiaonei.platform.component.feed.api.IGuideRecommend.GuideRecommendModel;
import com.xiaonei.platform.component.feed.api.impl.GuideRecommend;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.WebUserGuideView;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.FilterSearchCondition;
import com.xiaonei.search2.condition.OpenPeopleSearchCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;


public class GuideFriendNewsLogic {
    static class SingletonHolder {
        static GuideFriendNewsLogic instance = new GuideFriendNewsLogic();
    }
    public static GuideFriendNewsLogic getInstance() {
        return SingletonHolder.instance;
    }
    private static final int MAX_N = 10;
    
    //2009-11-13 12:12:24 将SearchAdapter换成SNS自己的封装，为了避堵
    private ISearchAdapter getSearchAdapter(){
    	//SearchAdapter adapter = SearchAdapter.getInstance();
		ISearchAdapter adapter = null;
		try {
			adapter = SnsAdapterFactory.getSearchAdapter();
		} catch (Exception e) {
			GuideLogger.printLog("get adapter error! e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return adapter;
    }
    public void testAll(final User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	GuideLogger.writeLine(this, "testAll() - start - host:" + host.getId());
    	int work = tellWork(host);
    	GuideLogger.writeLine(this, "testAll() - work:"+work+" - host:" + host.getId());
		int university = tellUniversity(host);
		GuideLogger.writeLine(this, "testAll() - university:"+university+" - host:" + host.getId());
		int highschool = tellHighschool(host);
		GuideLogger.writeLine(this, "testAll() - highschool:"+highschool+" - host:" + host.getId());
		int junior = tellJunior(host);
		GuideLogger.writeLine(this, "testAll() - junior:"+junior+" - host:" + host.getId());
		cost.endFinally();
		GuideLogger.writeLine(this,"testAll() - end - host:"+host.getId());
		cost.endFinally();
    }
    
    public void tellAll(final User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	GuideLogger.writeLine(this, host.getId()+": tellAll() - start - host:" + host.getId());
    	int sum = 0;
    	int work = tellWork(host);
		if ( work == -1) {
			GuideLogger.writeLine(this, host.getId()+": tellAll() - no work - host:" + host.getId());
			int university = tellUniversity(host);
			if ( university == -1) {
				GuideLogger.writeLine(this, host.getId()+": tellAll() - no university - host:" + host.getId());
				int highschool = tellHighschool(host);
				if ( highschool == -1) {
					GuideLogger.writeLine(this, host.getId()+": tellAll() - no highschool - host:" + host.getId());
					int junior = tellJunior(host);
					if ( junior == -1) {
						GuideLogger.writeLine(this, host.getId()+": tellAll() - no junior - host:" + host.getId());
					} else {
						sum += junior;
						GuideLogger.writeLine(this, host.getId()+": tellAll() - junior:"+junior+" - host:" + host.getId());
					}
				} else {
					sum += highschool;
					GuideLogger.writeLine(this, host.getId()+": tellAll() - highschool:"+highschool+" - host:" + host.getId());
				}
			} else {
				sum += university;
				GuideLogger.writeLine(this, host.getId()+": tellAll() - university:"+university+" - host:" + host.getId());
			}
		}
		else{
			sum += work; 
			GuideLogger.writeLine(this, host.getId()+": tellAll() - university:"+work+" - host:" + host.getId());
		}
		cost.endFinally();
		GuideLogger.writeLine(this,host.getId()+": tellAll() - end - host:"+host.getId()+" [inshion]for_stat:"+sum);
    }
    
    private void tell(User host,SearchResult result,String comment,String network,String url){
    	MyTimeCost cost = MyTimeCost.begin();
    	IGuideRecommend publish = GuideRecommend.getInstance();
		int c = 0;
		List<Integer> idlist = new ArrayList<Integer>();
		for (SearchView sv : result.getContents()) {
			if(sv instanceof WebUserGuideView){
				WebUserGuideView ugv = (WebUserGuideView) sv;
				if(ugv.getId() == host.getId()) continue;
				c++;
				if(c<=2){
					GuideLogger.writeLine(this,c+"|"+host.getId()+" tell():SendNewsFeed -> to:{"+ugv.getId()+"["+ugv.getName()+"]}"+" - {"+host.getName()+"("+network+")注册了"+OpiConstants.domain+"["+comment+"]}");
				}
				if(c<=MAX_N){
					//MyLogger.writeLine(this,host.getId()+": will tell():"+c+":"+ugv.getId());
					//publish.dispatch(host.getId(), host.getName(), ugv.getId(), model);// toID：要发给谁。
					//2009-9-21 10:28:38改成发数组的接口
					idlist.add(ugv.getId());
				}
				else{
					break;
				}
			}
			else{
				GuideLogger.writeLine(this,host.getId()+": tell():"+sv.getId()+"not WebUserGuideView");
			}
		}
		
		
		//如果idlist里没人，就不发。否则才发
		if(idlist.size()<=0){
			
		}
		else{
			//2009-9-21 14:21:46 改成批量发新鲜事了
			GuideRecommendModel model = new GuideRecommendModel();
			model.setComment(comment);
			model.setNetwork(network);
			model.setUrl(url);
			int [] intarray = new int [idlist.size()];
			for (int i = 0; i < intarray.length; i++) {
				intarray[i] = idlist.get(i);
			}
			
			publish.dispatch(host.getId(), host.getName(), intarray, model);
		}
		
		GuideLogger.writeLine(this,host.getId()+": tell("+network+") count:"+c);
		cost.endFinally();
		
    	
//		model.setComment("是漂亮师妹呢，快去看看吧");// 是漂亮师妹呢，快去看看吧
//		model.setNetwork("北京大学信息管理系07级 ");// 北京大学信息管理系07级
//		model.setUrl("url");// 点我认识TA要去的链接
    }
    
    private void seeResult(SearchResult result) {
//    	int c = 0;
//		for(SearchView sv : result.getContents()){
//			c++;
//			if(c>=6) break;
//			User user = UserAdapter.getUser(sv.getId());
//			GuideLogger.writeLine(this,"Result "+c+" :"+user.getId()+user.getName()+user.getAccount());
//		}
	}

	private String getFormatYear(int year) {
    	if(year <= 0){
    		return "";
    	}
    	else if(year % 1000 == 0){
			return year+"级";
		}
		else if(year % 100 <= 9){
			return "0"+year%100+"级";
		}
		else{
			return year%100+"级";
		}
	}

	private int tellUniversity(final User host){
		MyTimeCost cost = MyTimeCost.begin();
		int ret = -1;
		if(host == null){
			GuideLogger.printLog("host null",GuideLogger.ERROR);
		}
		else{
			UniversityInfo info = UserProfileUtil.loadUniversity(host);	
			if(info == null){
				GuideLogger.writeLine(this,host.getId()+"tellUniversity() - univ null");
			}
			else{
				if(null == info.getDepartment() || "".equals(info.getDepartment())){
					return ret;
				}
				if(info.getEnrollYear() == 0){
					return ret;
				}
				
				GuideLogger.writeLine(this,"tellUniversity() - host:"+host.getId()+" "+info.getUniversityName());
				OpenPeopleSearchCondition condition = null;
				try {
					condition = new OpenPeopleSearchCondition();
					UniversityCondition subcondition = new UniversityCondition(info.getUniversityId(), info.getUniversityName(), info.getDepartment(), null, info.getEnrollYear());
					condition.addOpUser(host.getId());
					condition.addSubCondition(subcondition);
				} catch (Exception e) {
					GuideLogger.printLog("tellUniversity() - conds - "+"host:"+host.getId()+e.toString(),1);
					e.printStackTrace();
				}
				
				Map<IndexType, SearchViewFactory> factories = null;
				try {
					factories = new HashMap<IndexType, SearchViewFactory>();
					factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
					
				} catch (Exception e) {
					GuideLogger.printLog("tellUniversity() - factory - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
					e.printStackTrace();
				}
				
				SearchResult result = null;
				ISearchAdapter adapter = null;
				try {
					adapter = getSearchAdapter();
					if(adapter == null){
						GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +"adapter null!", GuideLogger.ERROR);
					}
					else{
						result = adapter.search(condition, factories, 0, MAX_N);
						// Telling!!!
						if(result != null){
							String uname = info.getUniversityName() == null ? "":info.getUniversityName();
							String dept = info .getDepartment()==null? "" : info.getDepartment();
							String year = getFormatYear(info.getEnrollYear());
							
							String network = uname+dept+year;
							String comment = "大学同学来了，看看你能不能成为TA的第一个好友吧！";
							String url = "javascript:void(0);";
							tell(host,result,comment, network, url);
							ret = result.getTotalCount();
							seeResult(result);
						}
					}
				} catch (Exception e) {
					GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +e.toString()+"|"+(adapter==null)+","+(result==null), GuideLogger.ERROR);
					e.printStackTrace();
				}
				
			}
			GuideLogger.writeLine(this, host.getId()+"tellUniversity() - tells:"+ret);
			cost.endFinally();
		}
		return ret;
	}

	private int tellWork(final User host){
		MyTimeCost cost = MyTimeCost.begin();
		if (host == null) {
			GuideLogger.writeLine(this, "tellWork() - host null");
			return -1;
		}
		int ret = -1;
		WorkplaceInfo info = UserProfileUtil.loadWork(host);
		if (info == null) {
			GuideLogger.writeLine(this, host.getId()+"tellWork() - work null");
			return -1;
		}
		else{
			FilterSearchCondition condition = null;
			try {
				condition = new FilterSearchCondition();
				WorkplaceCondition subcondition = new WorkplaceCondition(info.getWorkplaceId(), info.getWorkplaceName());
				condition.addOpUser(host.getId());
				condition.addWorkplaceInfo(subcondition);
			} catch (Exception e) {
				GuideLogger.printLog("tellWork() - conds - " + "host:"+ host.getId() + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			Map<IndexType, SearchViewFactory> factories = null;
			try {
				factories = new HashMap<IndexType, SearchViewFactory>();
				factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
				
			} catch (Exception e) {
				GuideLogger.printLog("tellWork() - factory - " + "host:"	+ host.getId() + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			SearchResult result = null;
			ISearchAdapter adapter = null;
			try {
				adapter = getSearchAdapter();
				if(adapter == null){
					GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +"adapter null!", GuideLogger.ERROR);
				}
				else{
					result = adapter.search(condition, factories, 0, MAX_N);
					// Telling!!!
					if(result != null){
						String network = info.getWorkplaceName();
						String comment = "是你的同事呢，快去认识TA吧！";
						String url = "javascript:void(0);";
						tell(host,result,comment, network, url);
						
						ret = result.getTotalCount();
						
						seeResult(result);
					}
				}
				
			} catch (Exception e) {
				GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +e.toString()+"|"+(adapter==null)+","+(result==null), GuideLogger.ERROR);
//				e.printStackTrace();
			}
			
		}
		GuideLogger.writeLine(this, host.getId()+"tellWork() - tells:"+ret);
		cost.endFinally();
		return ret;
	}

	private int tellHighschool(final User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	int ret = -1;
    	if(host == null){
    		GuideLogger.writeLine(this,"tellHighschool() - host null");
    		return ret;
    	}
    	HighSchoolInfo info = UserProfileUtil.loadHighschool(host);	
		if(info == null){
			GuideLogger.writeLine(this,host.getId()+"tellHighschool() - info null");
		}
		else{
			GuideLogger.writeLine(this,"tellHighschool() - host:"+host.getId()+" "+info.getHighSchoolName());
			OpenPeopleSearchCondition condition = null;
			try {
				condition = new OpenPeopleSearchCondition();
				HighSchoolCondition subcondition = new HighSchoolCondition(info.getHighSchoolId(), info.getHighSchoolName(), null, null, null, info.getEnrollYear());
				condition.addOpUser(host.getId());
				condition.addSubCondition(subcondition);
			} catch (Exception e) {
				GuideLogger.printLog("tellHighschool() - conds - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			Map<IndexType, SearchViewFactory> factories = null;
			try {
				factories = new HashMap<IndexType, SearchViewFactory>();
				factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
				
			} catch (Exception e) {
				GuideLogger.printLog("tellHighschool() - factory - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			SearchResult result = null;
			ISearchAdapter adapter = null;
			try {
				adapter = getSearchAdapter();
				if(adapter == null){
					GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +"adapter null!", GuideLogger.ERROR);
				}
				else{
					result = adapter.search(condition, factories, 0, MAX_N);
					// Telling!!!
					if(result != null){
						String year = getFormatYear(info.getEnrollYear());
						String network = info.getHighSchoolName()+year;
						String comment = "高中同学呢，看看你认识吗？";
						String url = "javascript:void(0);";
						tell(host,result,comment, network, url);
						ret = result.getTotalCount();
						seeResult(result);
					}
				}
			} catch (Exception e) {
				GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +e.toString()+"|"+(adapter==null)+","+(result==null), GuideLogger.ERROR);
//				e.printStackTrace();
			}
			
		}
		GuideLogger.writeLine(this, host.getId()+"tellHighschool() - tells:"+ret);
		cost.endFinally();
		return ret;
    }
    
	private int tellJunior(final User host){
    	MyTimeCost cost = MyTimeCost.begin();
    	int ret = -1;
    	if(host == null){
    		GuideLogger.printLog("host null tellJunior()",GuideLogger.ERROR);
    	}
    	else{
    		JuniorHighSchoolInfo info = UserProfileUtil.loadJuniorhighschool(host);	
    		if(info == null){
    			GuideLogger.writeLine(this,host.getId()+"tellJunior() - univ null");
    		}
    		else{
    			GuideLogger.writeLine(this,"tellJunior() - host:"+host.getId()+" "+info.getJunHighentarySchoolName());
    			OpenPeopleSearchCondition condition = null;
    			try {
    				condition = new OpenPeopleSearchCondition();
    				JuniorHighSchoolCondition subcondition = new JuniorHighSchoolCondition(info.getJunHighentarySchoolId(), info.getJunHighentarySchoolName(), info.getJunHighentarySchoolYear());
    				condition.addOpUser(host.getId());
    				condition.addSubCondition(subcondition);
    			} catch (Exception e) {
    				GuideLogger.printLog("tellJunior() - conds - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
    				e.printStackTrace();
    			}
    			
    			Map<IndexType, SearchViewFactory> factories = null;
    			try {
    				factories = new HashMap<IndexType, SearchViewFactory>();
    				factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
    				
    			} catch (Exception e) {
    				GuideLogger.printLog("tellJunior() - factory - "+"host:"+host.getId()+e.toString(),GuideLogger.ERROR);
    				e.printStackTrace();
    			}
    			
    			SearchResult result = null;
    			ISearchAdapter adapter = null;
    			try {
    				adapter = getSearchAdapter();
    				if(adapter == null){
    					GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +"adapter null!", GuideLogger.ERROR);
    				}
    				else{
    					result = adapter.search(condition, factories, 0, MAX_N);
    					// Telling!!!
    					if(result != null){
    						String year = getFormatYear(info.getJunHighentarySchoolYear());
    						String network = info.getJunHighentarySchoolName()+year;
    						String comment = "初中同学呢，看看你认识吗？";
    						String url = "javascript:void(0);";
    						tell(host,result,comment, network, url);
    						ret = result.getTotalCount();
    						ret = result.getTotalCount();
    						seeResult(result);
    						
    					}
    				}
    			} catch (Exception e) {
    				GuideLogger.printLog("" + "host:"	+ host.getId() + "|" +e.toString()+"|"+(adapter==null)+","+(result==null), GuideLogger.ERROR);
//					e.printStackTrace();
    			}
    			
    		}
    		GuideLogger.writeLine(this, host.getId()+"tellJunior() - tells:"+ret);
    		cost.endFinally();
    	}
		return ret;
    }
    
}
