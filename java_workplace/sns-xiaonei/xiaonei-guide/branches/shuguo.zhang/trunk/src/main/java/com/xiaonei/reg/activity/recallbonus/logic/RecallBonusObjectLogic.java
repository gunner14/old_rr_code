package com.xiaonei.reg.activity.recallbonus.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.recallbonus.dao.RecallBonusObjectDao;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class RecallBonusObjectLogic {
	private static RecallBonusObjectLogic instance = new RecallBonusObjectLogic();

    public static RecallBonusObjectLogic getInstance() {
        return instance;
    }
    
    private static Map<Integer,RecallBonusObjectModel> map = null;
    
    private void reloadMap(){
    	map = new HashMap<Integer, RecallBonusObjectModel>();
    	//callBonusModel m = 		new RecallBonusModel(id, bonusId, 		bonusName,   bonusCost, bonusRate, 	bonusCount, bonusMax);
    	RecallBonusObjectModel m1 = new RecallBonusObjectModel(1, 1, 		"礼券2个", 			15, 10000000,			 0, 1000000);
    	RecallBonusObjectModel m2 = new RecallBonusObjectModel(2, 2, 		"礼券5个",		 	 6,  2000000,			 0, 500000);
    	RecallBonusObjectModel m3 = new RecallBonusObjectModel(3, 3, 		"礼券10个", 			 9,  1000000,			 0, 100000);
    	RecallBonusObjectModel m4 = new RecallBonusObjectModel(4, 4, 		"VIP7天免费体验", 	 3,    50000,			 0, 10000);
    	RecallBonusObjectModel m5 = new RecallBonusObjectModel(5, 5, 		"VIP30天免费体验", 	 6,    10000,			 0, 1000);
    	RecallBonusObjectModel m6 = new RecallBonusObjectModel(6, 6, 		"VIP3个月免费体验", 	 9,     1000,			 0, 100);
    	RecallBonusObjectModel m7 = new RecallBonusObjectModel(7, 7, 		"免费VIP5年", 		15,        2,		 	 0, 1);
    	RecallBonusObjectModel m8 = new RecallBonusObjectModel(8, 8, 		"IPhone一个",		15,        0,		 	 0, 0);
    	RecallBonusObjectModel m9 = new RecallBonusObjectModel(9, 9, 		"IMac一台",		    30,        0,		 	 0, 0);
    	RecallBonusObjectModel m10 = new RecallBonusObjectModel(10, 10, 	"10元移动话费",		15,    	   0,		 	 0, 0);
    	RecallBonusObjectModel m11 = new RecallBonusObjectModel(11, 11, 	"30元移动话费",	 	0,     	   0,		 	 0, 0);
    	map.put(1, m1);
    	map.put(2, m2);
    	map.put(3, m3);
    	map.put(4, m4);
    	map.put(5, m5);
    	map.put(6, m6);
    	map.put(7, m7);
    	map.put(8, m8);
    	map.put(9, m9);
    	map.put(10, m10);
    	map.put(11, m11);
    }
    
    private Map<Integer,RecallBonusObjectModel> getMap(){
    	if(map == null){
    		reloadMap();
    	}
    	return map;
    }
    
    public RecallBonusObjectModel getBounusInfo_FromMem(User host,int bid){
    	if(bid == 0){
    		GuideLogger.printLog(" key=0",GuideLogger.WARN);
    	}
    	RecallBonusObjectModel value = getMap().get(bid);
    	if(value == null){
    		
    	}
    	return value;
    }
    
    public List<RecallBonusObjectModel> select(){
    	
    	List<RecallBonusObjectModel>  ret = null;
    	try {
			ret = RecallBonusObjectDao.getInstance().select();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e){
    		e.printStackTrace();
    	}
		
		if(ret == null)ret= new ArrayList<RecallBonusObjectModel> ();
		return ret;
    }
    
    public int getRateNumber_FromMem(User host,int bid){
    	if(bid == 0){
    		GuideLogger.printLog(" key=0",GuideLogger.WARN);
    	}
    	RecallBonusObjectModel value = getMap().get(bid);
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
			ret = RecallBonusObjectDao.getInstance().increase(bid);
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

	public RecallBonusObjectModel selectById(int bid) {
		RecallBonusObjectModel  ret = null;
    	try {
			ret = RecallBonusObjectDao.getInstance().selectByBonusid(bid);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e){
    		e.printStackTrace();
    	}
		
		return ret;
	}
    
}
