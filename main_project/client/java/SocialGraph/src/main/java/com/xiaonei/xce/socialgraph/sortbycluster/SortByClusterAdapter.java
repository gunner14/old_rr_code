package com.xiaonei.xce.socialgraph.sortbycluster;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;

import org.json.JSONObject;
import org.json.JSONArray;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.SortByClusterPrx;
import xce.socialgraph.SortByClusterPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.OpenPeopleSearchCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.ElementarySchoolCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.condition.subcondition.BaseCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.search2.sample.UserPlainWebViewFactory;

/**
 * Get recommondation sorted by autogroup
 * 
 * @author zhangnan.
 * 
 */
public class SortByClusterAdapter extends ReplicatedClusterAdapter {
	private static final String endpoint = "ControllerSortByCluster";
	private static int interval = 120;
	private static SortByClusterAdapter instance = null;

	private SortByClusterAdapter() {
		super(endpoint, interval, Channel.newSocialGraphChannel(endpoint));
	}

	public static SortByClusterAdapter getInstance() {
		if (null == instance) {
			synchronized (SortByClusterAdapter.class) {
				if (null == instance) {
					try {
						instance = new SortByClusterAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}
	private SortByClusterPrx getSortByClusterManager(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if (prx0 != null) {
			SortByClusterPrx prx = SortByClusterPrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_twoway());
			return prx;
		}
		return null;
	}

	// private SortByClusterPrx getSortByClusterManagerOneWay(int userId) {
	// Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	// if (prx0 != null) {
	// SortByClusterPrx prx =
	// SortByClusterPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_oneway());
	// return prx;
	// }
	// return null;
	// }


	/**
	 * Get Recommend sorted by autogroup
	 * 
	 * @param userId
	 * @param idList
	 *            < 500
	 * @param limit
	 *            < 50
	 * @return
	 */
	public List<Integer> getRecommendByCluster(int userId,
			List<Integer> idList, int limit) {
		final int size = idList.size();
		int[] list = new int[size];
		for (int i = 0; i < size; ++i) {
			list[i] = idList.get(i);
		}

		int[] tmpList = getSortByClusterManager(userId).GetRecommendByCluster(userId, list, limit);

		List<Integer> result = new ArrayList<Integer>();
		for (int i = 0; i < tmpList.length; ++i) {
			result.add(tmpList[i]);
		}
		return result;
	}
	
 //add JS rank algorithm, new sortByCluster adapter.
	public List<Offer> getFinalRecommendByCluster(Map<String, String> userInfoMap, 
			int limit) throws ParseException {
//	public List<Offer> getFinalRecommendByCluster(int limit) throws ParseException {
	
//		Map<String, String> userInfoMap = new HashMap<String, String>();
//		userInfoMap.put("p", "[{\"name\":\"清华大学\",\"id\":\"1001\", \"depart\":\"信息学院\", \"year\":\"2004\", \"t\":\"univ\"}]");
//		userInfoMap.put("uid", "458135031");
		JSONObject searchJson = new JSONArray(userInfoMap.get("p")).getJSONObject(0);
		
		int userId = Integer.parseInt(userInfoMap.get("uid"));
		
		final int searchRetLimit = 500;
		List<Integer> searchRetIds = searchUserIds(searchJson, searchRetLimit);
		
		int[] idList = new int[searchRetIds.size()];
		for(int i = 0; i != searchRetIds.size(); i++) {
			idList[i] = searchRetIds.get(i);
		}
		// type;
		String searchTypeString = searchJson.getString("t");
		
		int[] tmpList = getSortByClusterManager(userId).GetFinalRecommendByCluster(
				userId, idList, limit);  // socialgraph interface
		
		List<Offer> result = new ArrayList<Offer>();
		for(int i = 0; i != tmpList.length; i++) {
			Offer offer = new Offer();
			offer.setBase(tmpList[i], 0, searchTypeString);
			result.add(offer);	
		}
		return result;
	}
	
	private List<Integer> searchUserIds(JSONObject searchJson, int limit) {        

        OpenPeopleSearchCondition condition = new OpenPeopleSearchCondition();
		String searchTypeString = searchJson.getString("t");
		if (searchTypeString.equals("work")) {  //工作地点
			String workplace = searchJson.getString("name");
			WorkplaceCondition conditionWorkplace = new WorkplaceCondition(0, workplace);
 			condition.addSubCondition(conditionWorkplace);
		} else if (searchTypeString.equals("univ")) { // 大学
			int id = searchJson.getInt("id");
			String name = searchJson.getString("name");
			String department = searchJson.getString("depart");
			int year = searchJson.getInt("year");
			UniversityCondition conditionUniversity = new UniversityCondition(id, name, department, null, year);
			condition.addSubCondition(conditionUniversity);
		} else if (searchTypeString.equals("high")) { //中学
			String name = searchJson.getString("name");
			int year = searchJson.getInt("year");
			HighSchoolCondition conditionHighSchool = new HighSchoolCondition(0, name, null, null, null, year);
			condition.addSubCondition(conditionHighSchool);
		} else if (searchTypeString.equals("sect")) { //大专
			String name = searchJson.getString("name");
			int year = searchJson.getInt("year");
			CollegeCondition condtionCollege = new CollegeCondition(0, name, null, year);
			condition.addSubCondition(condtionCollege);
		} else if (searchTypeString.equals("juni")) { //初中
			String name = searchJson.getString("name");
			int year = searchJson.getInt("year");			
			JuniorHighSchoolCondition conditionJuniorHighSchool = new JuniorHighSchoolCondition(0, name, year);
			condition.addSubCondition(conditionJuniorHighSchool);
		} else if (searchTypeString.equals("elem")) { //小学
			String name = searchJson.getString("name");
			int year = searchJson.getInt("year");
			ElementarySchoolCondition conditonElementarySchool = new ElementarySchoolCondition(0, name, year);
			condition.addSubCondition(conditonElementarySchool);
		} else if (searchTypeString.equals("base")) { //家乡
			String province = searchJson.getString("prov");
			String city = searchJson.getString("city");
			BaseCondition conditonBase = new BaseCondition(null, province, city);
			condition.addSubCondition(conditonBase);
		} else {
			/* do nothing */
		}
		
        Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
        factories.put(IndexType.User, UserPlainWebViewFactory.getInstance());
        
        final int begin = 0; 
		final String callerName = "SocialGraphForGuide";
        SearchResult result = null;
        
        try {
			result = SearchAdapter.getInstance().searchWithRestriction(condition, factories, begin, limit, callerName);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        List<Integer> searchResult = new ArrayList<Integer>();
        for (SearchView sv : result.getContents()) {
        	searchResult.add(sv.getId());
        }
		return searchResult;
	}

	public static void main(String[] args) {
		int userId = 238111132;
		List<Integer> idList = new ArrayList<Integer>();
		idList.add(1003);
		idList.add(231626616);
		idList.add(225025281);
		idList.add(224838055);
		idList.add(221752298);
		idList.add(222890793);
		idList.add(459228307);
		idList.add(459228307);
		idList.add(336789465);
		int limit = 50;
		List<Integer> resList = new ArrayList<Integer>();
		resList = SortByClusterAdapter.getInstance().getRecommendByCluster(userId, idList, limit);
		// ///////////////////////
		List<Offer> resOffers = new ArrayList<Offer>();
//		resList.clear();
//		String univString = "[{\"name\":\"清华大学\",\"id\":\"1001\", \"depart\":\"信息学院\", \"year\":\"2004\", \"t\":\"univ\"}]";
//		String workString = "[{\"name\":\"千橡互动\", \"t\":\"work\"}]";
//		String highString = "[{\"name\":\"北京一二六中\",\"year\":\"2004\", \"t\":\"high\"}]";
//		String collegeString = "[{\"name\":\"东城天艺技术学校\",\"year\":\"2008\", \"t\":\"sect\"}]";
//		String juniString = "[{\"name\":\"北京一二六中\",\"year\":\"2004\", \"t\":\"juni\"}]";
//		String elemString = "[{\"name\":\"北京小学\",\"year\":\"1994\", \"t\":\"elem\"}]";
//		String baseString = "[{\"prov\":\"湖北\",\"city\":\"武汉市\", \"t\":\"base\"}]";
//		
		System.out.println(resOffers.size());
		
		Map<String, String> map = new HashMap<String, String>();
		//map.put("p", "[{\"name\":\"Nanyang Technological University\",\"id\":\"0\", \"depart\":\"EEE\", \"year\":\"2010\", \"t\":\"univ\"}]");
		map.put("p", "[{\"name\":\"千橡互动\", \"t\":\"work\"}]");
		map.put("uid", "458135031");

		try {
			resOffers = SortByClusterAdapter.getInstance()
						.getFinalRecommendByCluster(map, 50);
		} catch (ParseException e) {
			// TODO Auto-generated catch block 
			e.printStackTrace();
		}
		
		for(Offer one: resOffers) {
			System.out.println(one.getUserId() + "\t" + one.getWeight() + "\t" + one.getType());
		}
		System.out.println("offer size: " + resOffers.size());
		System.exit(0);
	}
}
