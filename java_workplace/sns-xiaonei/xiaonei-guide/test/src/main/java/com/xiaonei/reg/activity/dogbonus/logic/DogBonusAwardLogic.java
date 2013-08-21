package com.xiaonei.reg.activity.dogbonus.logic;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.dogbonus.dao.DogBonusAwardDao;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAwardModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class DogBonusAwardLogic {
	private static DogBonusAwardLogic instance = new DogBonusAwardLogic();

    public static DogBonusAwardLogic getInstance() {
        return instance;
    }
    
    public List<DogBonusAwardModel> select10(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusAwardModel> list = null;
    	try {
			list = DogBonusAwardDao.getInstance().selectLatest20();
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		return list;
		
    }
    
    public List<DogBonusAwardModel> selectById(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusAwardModel> list = null;
    	try {
			list = DogBonusAwardDao.getInstance().selectByUid(host.getId());
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		return list;
		
    }

	public int insert(User host, int bonusId, String bonusName) {
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
		try {
			ret = DogBonusAwardDao.getInstance().insertRecord(host.getId(), bonusId, bonusName);
		} 
		catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		
		if(ret == 0){
			GuideLogger.printLog(" host:"+host.getId()+" ret 0");
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" ");
		}
		
		return ret;
		
	}
    
    
}
