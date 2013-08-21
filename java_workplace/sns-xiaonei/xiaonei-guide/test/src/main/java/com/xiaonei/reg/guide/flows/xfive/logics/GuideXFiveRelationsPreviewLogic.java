package com.xiaonei.reg.guide.flows.xfive.logics;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import com.renren.sns.guide.factory.GuideRelationModelFactory;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.guide.model.GuideIRelationModel;
import com.renren.sns.guide.utils.GuideFriendsUtil;
import com.xiaonei.platform.core.model.User;


public class GuideXFiveRelationsPreviewLogic {
    static class SingletonHolder {
        static GuideXFiveRelationsPreviewLogic instance = new GuideXFiveRelationsPreviewLogic();
    }
    public static GuideXFiveRelationsPreviewLogic getInstance() {
        return SingletonHolder.instance;
    }
    public List<GuideIRelation> getProfileResults(int count, User user,int flag_unuse){
    	List<GuideIRelation> mayFriends = new ArrayList<GuideIRelation>();
        try {
            int amount = count;
            List<GuideIRelationModel> list = new ArrayList<GuideIRelationModel>();
            //获取各个阶段的Model
            GuideIRelationModel workerModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.COWORKER_RELATION_MODEL);
            GuideIRelationModel universityModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.UNIVERSITY_RELATION_MODEL);
            GuideIRelationModel collegeModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.COLLEGE_RELATION_MODEL);
            GuideIRelationModel highModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.HIGH_RELATION_MODEL);
            GuideIRelationModel juniorModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.JUNIOR_RELATION_MODEL);
            GuideIRelationModel elementaryModel = GuideRelationModelFactory.create(user, amount,
                    GuideRelationModelFactory.ELEMENTARY_RELATION_MODEL);
            GuideIRelationModel locationModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.LOCATION_RELATION_MODEL);
            list.add(workerModel);
            list.add(universityModel);
            list.add(collegeModel);
            list.add(highModel);
            list.add(juniorModel);
            list.add(elementaryModel);
            list.add(locationModel);
            mayFriends = GuideFriendsUtil.findPeople(user, list);
            
            
        } catch (Exception e) {
            System.err.println("guideX5MayKnown:"+e.toString());
        }
		return mayFriends;
	}
    
    private static final int COUNT_LIMIT = 24;
    public String getJSONForFriendSelector(List<GuideIRelation> mayFriends) {
    	try {
			JSONArray retCandidate = new JSONArray();
			JSONObject jo = new JSONObject();
			try {
				int safeflag = 500; 
				
				int listscount = mayFriends.size();
				GuideIRelation [] relationlist = mayFriends.toArray(new GuideIRelation[mayFriends.size()]);
				int [] listindex = new int [listscount]; 
				int [] listsize = new int [listscount];
				int sum = 0;
				for (int i = 0; i < relationlist.length; i++) {
					List<GuideIRelationFriend> list = relationlist[i].getList();
					listindex[i] = 0;//list.size();
					listsize[i] = list.size();
					sum += list.size();
				}
				
				int count = 0;
				List<Integer> resultlist = new ArrayList<Integer>();
				while(sum > 0 && safeflag -- > 0){
					for (int i = 0; i < relationlist.length; i++) {
						List<GuideIRelationFriend> list = relationlist[i].getList();
						if(listindex[i] < listsize[i]){//说明该List还有东西
							GuideIRelationFriend f = list.get(listindex[i]);
							listindex[i]++;
							//去重
							if(resultlist.contains(f.getId())){
								
							}
							else{
								count++;
								if(count>COUNT_LIMIT){
									break;
								}
								
								JSONObject joinner = new JSONObject();
								resultlist.add(f.getId());
								joinner.put("id", f.getId());
								joinner.put("head", f.getTiny_Url());
								joinner.put("name", f.getName());
								joinner.put("net", relationlist[i].getTypeName());
								retCandidate.put(joinner);
							}
							sum -- ;
						}
					}
				}
				
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return "";
	}	
	
    public List<Integer> getIdsFromGuideIRelationList(List<GuideIRelation> mayFriends) {
    	List<Integer> ret = new ArrayList<Integer>();
    	if(mayFriends == null)return ret;
    	try {
			//JSONArray retCandidate = new JSONArray();
			//JSONObject jo = new JSONObject();
			try {
				int safeflag = 500; 
				
				int listscount = mayFriends.size();
				GuideIRelation [] relationlist = mayFriends.toArray(new GuideIRelation[mayFriends.size()]);
				int [] listindex = new int [listscount]; 
				int [] listsize = new int [listscount];
				int sum = 0;
				for (int i = 0; i < relationlist.length; i++) {
					List<GuideIRelationFriend> list = relationlist[i].getList();
					listindex[i] = 0;//list.size();
					listsize[i] = list.size();
					sum += list.size();
				}
				
				int count = 0;
				//List<Integer> resultlist = new ArrayList<Integer>();
				while(sum > 0 && safeflag -- > 0){
					for (int i = 0; i < relationlist.length; i++) {
						List<GuideIRelationFriend> list = relationlist[i].getList();
						if(listindex[i] < listsize[i]){//说明该List还有东西
							GuideIRelationFriend f = list.get(listindex[i]);
							listindex[i]++;
							//去重
							if(ret.contains(f.getId())){
								
							}
							else{
								count++;
								if(count>COUNT_LIMIT){
									break;
								}
								ret.add(f.getId());
							}
							sum -- ;
						}
					}
				}
				
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			//jo.put("candidate", retCandidate);
			return ret;
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return ret;
	}	

    @SuppressWarnings("unchecked")
	public static void main(String[] args) {
    	List<Integer> ret = new ArrayList<Integer>();
    	int safeflag = 500; 
    	int listscount = 2;
		//GuideIRelation [] relationlist = mayFriends.toArray(new GuideIRelation[mayFriends.size()]);
    	List<Integer> [] relationlist = new ArrayList [2];
    	relationlist[0] = new ArrayList<Integer>();
    	relationlist[0].add(5);
    	relationlist[0].add(7);
    	relationlist[0].add(8);
    	
    	relationlist[1] = new ArrayList<Integer>();
    	
    	//String [] ss = new String [3];
		int [] listindex = new int [listscount]; 
		int [] listsize = new int [listscount];
		int sum = 0;
		for (int i = 0; i < relationlist.length; i++) {
			List<Integer> list = relationlist[i];
			listindex[i] = 0;//list.size();
			listsize[i] = list.size();
			sum += list.size();
		}
		
		int count = 0;
		while(sum > 0 && safeflag -- > 0){
			for (int i = 0; i < relationlist.length; i++) {
				List<Integer> list = relationlist[i];
				if(listindex[i] < listsize[i]){//说明该List还有东西
					Integer f = list.get(listindex[i]);
					System.out.println("cur:"+listindex[i]);
					listindex[i]++;
					//去重
					if(ret.contains(f)){
						System.out.println(" "+count+":"+f);
						
					}
					else{
						count++;
						if(count>COUNT_LIMIT){
							break;
						}
						System.out.println(" "+count+":"+f);
						ret.add(f);
					}
					sum -- ;
				}
			}
		}
		System.out.println("sum:"+count);
	}
}

