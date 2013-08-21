package com.xiaonei.reg.activity.dogbonus.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.dogbonus.dao.DogBonusObjectDao;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusObjectModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class DogBonusObjectLogic {
	private static DogBonusObjectLogic instance = new DogBonusObjectLogic();

    public static DogBonusObjectLogic getInstance() {
        return instance;
    }
    
    private static Map<Integer,DogBonusObjectModel> map = null;
    
    private void reloadMap(){
    	map = new HashMap<Integer, DogBonusObjectModel>();
    	//callBonusModel m = new DogBonusModel(id, bonusId, 	bonusName, bonusCost, bonusRate, 	bonusCount, bonusMax);
    	DogBonusObjectModel m1 = new DogBonusObjectModel(1, 1, 		"礼券2个", 			5, 10000000,			 0, 1000000);
    	DogBonusObjectModel m2 = new DogBonusObjectModel(2, 2, 		"礼券5个",		 	2,  2000000,			 0, 500000);
    	DogBonusObjectModel m3 = new DogBonusObjectModel(3, 3, 		"礼券10个", 			3,  1000000,			 0, 100000);
    	DogBonusObjectModel m4 = new DogBonusObjectModel(4, 4, 		"VIP7天免费体验", 	1,    50000,			 0, 10000);
    	DogBonusObjectModel m5 = new DogBonusObjectModel(5, 5, 		"VIP30天免费体验", 	2,    10000,			 0, 1000);
    	DogBonusObjectModel m6 = new DogBonusObjectModel(6, 6, 		"VIP3个月免费体验", 	3,     1000,			 0, 100);
    	DogBonusObjectModel m7 = new DogBonusObjectModel(7, 7, 		"免费VIP5年", 		5,        2,		 	 0, 1);
    	DogBonusObjectModel m8 = new DogBonusObjectModel(8, 8, 		"IPhone一个",		5,        0,		 	 0, 0);
    	DogBonusObjectModel m9 = new DogBonusObjectModel(9, 9, 		"IMac一台",		   10,        0,		 	 0, 0);
    	map.put(1, m1);
    	map.put(2, m2);
    	map.put(3, m3);
    	map.put(4, m4);
    	map.put(5, m5);
    	map.put(6, m6);
    	map.put(7, m7);
    	map.put(8, m8);
    	map.put(9, m9);
    }
    
    private Map<Integer,DogBonusObjectModel> getMap(){
    	if(map == null){
    		reloadMap();
    	}
    	return map;
    }
    
    public DogBonusObjectModel getBounusInfo_FromMem(User host,int bid){
    	if(bid == 0){
    		GuideLogger.printLog(" key=0",GuideLogger.WARN);
    	}
    	DogBonusObjectModel value = getMap().get(bid);
    	if(value == null){
    		
    	}
    	return value;
    }
    
    public List<DogBonusObjectModel> select(){
    	
    	List<DogBonusObjectModel>  ret = null;
    	try {
			ret = DogBonusObjectDao.getInstance().select();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e){
    		e.printStackTrace();
    	}
		
		if(ret == null)ret= new ArrayList<DogBonusObjectModel> ();
		return ret;
    }
    
    public int getRateNumber_FromMem(User host,int bid){
    	if(bid == 0){
    		GuideLogger.printLog(" key=0",GuideLogger.WARN);
    	}
    	DogBonusObjectModel value = getMap().get(bid);
    	if(value == null){
    		
    	}
    	return value.getBonus_rate();
    	
    }
    
    public int increaseBonus(User host,int bid){//DAO
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return -1;
    	}
    	
    	int ret = 0;
    	try {
			ret = DogBonusObjectDao.getInstance().increase(bid);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		
		if(ret == 0){
			GuideLogger.printLog(" host:"+host.getId()+" bid:"+bid+" IncFail");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" bid:"+bid+" IncSucc");
		}
		
		return ret;
    	
    	
    }
    
}
