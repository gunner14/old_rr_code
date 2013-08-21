package com.xiaonei.reg.activity.recallbonus.logic;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.recallbonus.dao.RecallBonusAwardDao;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAwardModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class RecallBonusAwardLogic {
	private static RecallBonusAwardLogic instance = new RecallBonusAwardLogic();

    public static RecallBonusAwardLogic getInstance() {
        return instance;
    }
    
    public List<RecallBonusAwardModel> select10(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusAwardModel> list = null;
    	try {
			list = RecallBonusAwardDao.getInstance().selectLatest20();
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
    
    public List<RecallBonusAwardModel> selectById(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusAwardModel> list = null;
    	try {
			list = RecallBonusAwardDao.getInstance().selectByUid(host.getId());
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
			ret = RecallBonusAwardDao.getInstance().insertRecord(host.getId(), bonusId, bonusName);
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
	
	 public List<RecallBonusAwardModel> selectYesterdayAwards(User host, int yesterjiday){
	    	if(host == null){
	    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
	    		return null;
	    	}
	    	List<RecallBonusAwardModel> list = null;
	    	try {
				list = RecallBonusAwardDao.getInstance().selectYesterdayRewards(yesterjiday);
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
	 
	 public List<RecallBonusAwardModel> selectYesterdayAwardsWillSend(User host, int yesterjiday){
	    	if(host == null){
	    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
	    		return null;
	    	}
	    	List<RecallBonusAwardModel> list = null;
	    	try {
				list = RecallBonusAwardDao.getInstance().selectYesterdayRewardsWillSend(yesterjiday);
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
	 
	 public List<RecallBonusAwardModel> selectYesterdayAwardsWillSendWithLimit(User host, int yesterjiday,int limit){
	    	if(host == null){
	    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
	    		return null;
	    	}
	    	List<RecallBonusAwardModel> list = null;
	    	try {
				list = RecallBonusAwardDao.getInstance().selectYesterdayRewardsWillSendWithLimit(yesterjiday, limit);
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
	 
	 public int sendAward(User host,int id){
		 if(host == null){
	    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
	    		return 0;
	    	}
	    	
	    	int ret = 0;
			try {
				ret = RecallBonusAwardDao.getInstance().sendAward(id);
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
