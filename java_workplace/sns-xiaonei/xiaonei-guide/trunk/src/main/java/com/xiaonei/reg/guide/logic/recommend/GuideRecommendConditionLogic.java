package com.xiaonei.reg.guide.logic.recommend;

import java.util.ArrayList;
import java.util.List;


public class GuideRecommendConditionLogic {

    private static GuideRecommendConditionLogic _instance = new GuideRecommendConditionLogic();
   
    private  List<Integer> trailNumList=new ArrayList<Integer>();
    
    public GuideRecommendConditionLogic(){
    	trailNumList.add(1);
    	trailNumList.add(2);
    	trailNumList.add(3);
    	trailNumList.add(4);
    }
    public static GuideRecommendConditionLogic getInstance() {
    	return _instance;
    }
    
    public void add(String i){
    	trailNumList.add(new Integer(i));
    }
    
    public void del(String i){
    	trailNumList.remove(new Integer(i));
    }
    
    public List<Integer> getConditonList(){
    	return trailNumList;
    }
    
    public boolean contains(int num){
    	return trailNumList.contains(num);
    	
    }
}
