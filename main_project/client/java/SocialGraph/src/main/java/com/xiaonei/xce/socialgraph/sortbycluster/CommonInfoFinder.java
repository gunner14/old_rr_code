package com.xiaonei.xce.socialgraph.sortbycluster;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import scala.reflect.Print;

import xce.socialgraph.recommend.DbUtil;
import xce.socialgraph.recommend.FriendListUtil;

import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.OpenPeopleSearchCondition;
import com.xiaonei.search2.condition.subcondition.ElementarySchoolCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.search2.sample.UserPlainWebViewFactory;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.pool.old.ConnectionPoolManager;

import mop.hi.oce.adapter.impl.NetworkReaderAdapterMCImpl;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.NetworkBigData;
import mop.hi.oce.domain.network.WorkplaceInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.svc.model.WorkplaceInfoSeqHelper;

public class CommonInfoFinder {

	private static final String callerName = "SocialGraphForGuide";
	private static final int searchLimit = 1000;
	private static final int returnLimit = 30;
	private int userId;
	private static NetworkReaderAdapterMCImpl networkReaderAdapter = new NetworkReaderAdapterMCImpl();
	private static NetworkBigData nbd = null;
	private static DefaultBuddyByApplicantCacheFactory factory = new DefaultBuddyByApplicantCacheFactory();
	private static BuddyByApplicantCacheAdapter adapter = new BuddyByApplicantCacheAdapter(factory);
	private Set<Integer> blockSet = null;
	
	public CommonInfoFinder() {
		
	}
	
	public CommonInfoFinder(int userId){
		this.userId = userId;
		this.nbd = networkReaderAdapter.getNetworkBigData(this.userId);
		this.blockSet = getBlockList(this.userId);
		this.blockSet.add(userId);
	}
	
	private CommonInfo search(String searchKey, OpenPeopleSearchCondition condition) {
		Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
        factories.put(IndexType.User, UserPlainWebViewFactory.getInstance());
     
    	final int begin = 0; 
        SearchResult result = null;
        try {
			result = SearchAdapter.getInstance().searchWithRestriction(condition, factories, begin, searchLimit, callerName);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        List<Integer> searchResult = new ArrayList<Integer>();
        int count = 0;
        for (SearchView sv : result.getContents()) {
        	if (blockSet.contains(sv.getId())) {
        		continue;
        	}
        	searchResult.add(sv.getId());
        	if (++count >= returnLimit) {
        		break;
        	}
        }
        if (searchResult.size() != 0) {
        	return new CommonInfo(searchKey, searchResult, result.getContents().size());
        } else {
        	return null;
        }
	}
	
	public static Set<Integer> getBlockList(int hostId) {
		Set<Integer> sgBlockList = new HashSet<Integer>();
		List<Long> block = DbUtil.getInstance().getSgBlockList(hostId);
		for(Long blockId : block) {
			sgBlockList.add(blockId.intValue());
		}
		List<Integer> friendList = FriendListUtil.getInstance().getFriendList(hostId, -1);
		for(Integer friendId : friendList) {
			sgBlockList.add((friendId));
		}

		DefaultBuddyByApplicantCacheData data = (DefaultBuddyByApplicantCacheData) adapter.getBuddyByApplicantCacheData(hostId, 0, -1);
		List<DefaultBuddyByApplicantCacheAccepterInfo> accepters = data.getAccepters();
		for(Iterator<DefaultBuddyByApplicantCacheAccepterInfo> iterator = accepters.iterator(); iterator.hasNext();) {
			DefaultBuddyByApplicantCacheAccepterInfo accepterInfo = iterator.next();
			sgBlockList.add(accepterInfo.getAccepter());			
		}		
		return sgBlockList;
	}
	
	private int minInt(int a, int b) {
		if (a < b) {
			return a;
		} else {
			return b;
		}
	}
	
	private List<CommonInfo> getWorkRecommend(int size){
		List<WorkplaceInfo> workInfos = nbd.getWorkspaces();
		if (workInfos.size() < 1) {
			return null;
		}
		List<String> formerKeys = new ArrayList<String>();
		List<CommonInfo> result = new ArrayList<CommonInfo>();
		for (int i = 0; i != minInt(size, workInfos.size()); i++) {
			OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
			String workplace = workInfos.get(i).getWorkplaceName();
			String searchKey = workplace;
			if (formerKeys.contains(searchKey)) {
				continue;
			}
			WorkplaceCondition conditionWorkplace = new WorkplaceCondition(0, workplace);
			condition.addSubCondition(conditionWorkplace);
			CommonInfo commonInfo = search(searchKey, condition);
			if (commonInfo != null) {
				result.add(commonInfo);
			}
			formerKeys.add(searchKey);
		}
		return result;
	}
	
	private List<CommonInfo> getUnivRecommend(int size){
		List<UniversityInfo> univInfos = nbd.getUnivs();
		if (univInfos.size() < 1) {
			return null;
		}
		List<String> formerKey = new ArrayList<String>();
		List<CommonInfo> result = new ArrayList<CommonInfo>();
		for (int i = 0; i != minInt(size, univInfos.size()); i++) {
			OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
			int id = univInfos.get(i).getUniversityId();
			String name = univInfos.get(i).getUniversityName();
			String department = univInfos.get(i).getDepartment();
			String searchKey = name + department;
			int year = univInfos.get(i).getEnrollYear();
			if (year > 1000 && year < 3000) {
				searchKey = searchKey + year + "级";
			}
			if (formerKey.contains(searchKey)) {
				continue;
			}
			UniversityCondition conditionUniversity = new UniversityCondition(id, name, department, null, year);
			condition.addSubCondition(conditionUniversity);
			CommonInfo commonInfo = search(searchKey, condition);
			if (commonInfo != null) {
				result.add(commonInfo);
			}
			formerKey.add(searchKey);
		}
		return result;
	}
	
	private List<CommonInfo> getHighRecommond(int size) {
		List<HighSchoolInfo> highInfos = nbd.getHighschools();
		if (highInfos.size() < 1) {
			return null;
		}
		List<CommonInfo> result = new ArrayList<CommonInfo>();
		List<String> formerKey = new ArrayList<String>();
		for (int i = 0; i != minInt(size, highInfos.size()); i++) {
			OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
			String name = highInfos.get(i).getHighSchoolName();
			String searchKey = name;
			int year = highInfos.get(i).getEnrollYear();
			if (year > 1000 && year < 3000) { 
				searchKey = searchKey + year + "级";
			}
			if (formerKey.contains(searchKey)) {
				continue;
			}
			HighSchoolCondition conditionHighSchool = new HighSchoolCondition(0, name, null, null, null, year);
			condition.addSubCondition(conditionHighSchool);
			CommonInfo commonInfo = search(searchKey, condition);
			if (commonInfo != null) {
				result.add(commonInfo);
			}
			formerKey.add(searchKey);
		}
		return result;
	}
	
	private List<CommonInfo> getJuniorhighRecommond(int size) {
		List<JuniorHighSchoolInfo> juniorhighInfos = nbd.getJuniorhighschools();
		if (juniorhighInfos.size() < 1) {
			return null;
		}
		List<CommonInfo> result = new ArrayList<CommonInfo>();
		List<String> formerKey = new ArrayList<String>();
		for (int i = 0; i != minInt(size, juniorhighInfos.size()); i++) {
			OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
			String name = juniorhighInfos.get(i).getJunHighentarySchoolName();
			String searchKey = name;
			int year = juniorhighInfos.get(i).getJunHighentarySchoolYear();
			if (year > 1000 && year < 3000) { 
				searchKey = searchKey + year + "级";
			}
			if (formerKey.contains(searchKey)) {
				continue;
			}
			JuniorHighSchoolCondition conditionJuniorHighSchool = new JuniorHighSchoolCondition(0, name, year);
			condition.addSubCondition(conditionJuniorHighSchool);
			CommonInfo commonInfo = search(searchKey, condition);
			if (commonInfo != null) {
				result.add(commonInfo);
			}
			formerKey.contains(searchKey);
		}
		return result;
	}
	
	private List<CommonInfo> getElementaryRecommond(int size) {
		List<ElementarySchoolInfo> elemInfos = nbd.getElementaryschools();
		if (elemInfos.size() < 1) {
			return null;
		}
		List<CommonInfo> result = new ArrayList<CommonInfo>();
		List<String> formerKey = new ArrayList<String>();
		for (int i = 0; i != minInt(size, elemInfos.size()); i++) {
			OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
			String name = elemInfos.get(i).getElementarySchoolName();
			String searchKey = name;
			int year = elemInfos.get(i).getElementarySchoolYear();
			if (year > 1000 && year < 3000) { 
				searchKey = searchKey + year + "级";
			}
			if (formerKey.contains(searchKey)) {
				continue;
			}
			ElementarySchoolCondition conditonElementarySchool = new ElementarySchoolCondition(0, name, year);
			condition.addSubCondition(conditonElementarySchool);
			CommonInfo commonInfo = search(searchKey, condition);
			if (commonInfo != null) {
				result.add(commonInfo);
			}
			formerKey.add(searchKey);
		}
		return result;
	}
	
	
	public List<CommonInfo> getInfoRecommend(String type, int size) {
		if (type == CommonInfo.workType) {
			return getWorkRecommend(size);
		} else if (type == CommonInfo.univType) {
			return getUnivRecommend(size);
		} else if (type == CommonInfo.highType) {
			return getHighRecommond(size);
		} else if (type == CommonInfo.juniType) {
			return getJuniorhighRecommond(size);
		} else if (type == CommonInfo.elemType) {
			return getElementaryRecommond(size);
		} else {
			return null;
		}
	}
		
	public static void main(String[] args) {
		List<Integer> test = new ArrayList<Integer>();
		test.add(null);
		test.add(12);
//		int userId = 266084115;
		int userId = 224838055;
		long start = Calendar.getInstance().getTimeInMillis();
		
		
		CommonInfoFinder finder = new CommonInfoFinder(userId);
		
		
		long end = Calendar.getInstance().getTimeInMillis();
		System.out.println("new time: " + (end - start));
				
		List<CommonInfo> result = finder.getInfoRecommend(CommonInfo.workType, 1);
						
		end = Calendar.getInstance().getTimeInMillis();
		System.out.println("get work info time: " + (end - start));
		start = end;
		if (result != null) {
			System.out.println("work type size:" + result.get(0).toSting());
		}
		
		
		result = finder.getInfoRecommend(CommonInfo.univType, 3);
		if (result.size() != 0) {
			System.out.println("univ1 type size:" + result.get(0).toSting());
			System.out.println("univ2 type size:" + result.get(1).toSting());
		}
		
		result = finder.getInfoRecommend(CommonInfo.highType, 3);
		if (result.size() != 0) {
			System.out.println("high type size:" + result.get(0).toSting());
		}
		
		result = finder.getInfoRecommend(CommonInfo.juniType, 3);
		if (result.size() != 0) {
			System.out.println("juni type size:" + result.get(0).toSting());
		}
		
		result = finder.getInfoRecommend(CommonInfo.elemType, 3);
		if (result.size() != 0) {
			System.out.println("elem type size:" + result.get(0).toSting());
		}
		
		end = Calendar.getInstance().getTimeInMillis();
		System.out.println("get info time: " + (end - start));
		System.out.println(result);
		System.exit(0);
	}
}
