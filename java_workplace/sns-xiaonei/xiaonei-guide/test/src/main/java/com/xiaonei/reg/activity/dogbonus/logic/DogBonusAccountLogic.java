package com.xiaonei.reg.activity.dogbonus.logic;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.dogbonus.constant.IDogBonusConst;
import com.xiaonei.reg.activity.dogbonus.dao.DogBonusAccountDao;
import com.xiaonei.reg.activity.dogbonus.dao.DogBonusAccountRecordDao;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAccountModel;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAccountRecordModel;
import com.xiaonei.reg.guide.util.GuideLogger;


public class DogBonusAccountLogic {
	private static DogBonusAccountLogic instance = new DogBonusAccountLogic();

    public static DogBonusAccountLogic getInstance() {
        return instance;
    }
    
    public int getAccountCurrent(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	
    	DogBonusAccountModel model = null;
    	try {
			model = DogBonusAccountDao.getInstance().selectByUid(host.getId());
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	if(model == null){
			GuideLogger.printLog(" host:"+host.getId()+" NoCurrent",GuideLogger.WARN);
			return 0;
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" Current:"+model.getCurrent());
			return model.getCurrent();
		}
    	
    	
    }
    
    public boolean alreadyGetDailyIncreament(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return true;
    	}
    	
    	DogBonusAccountRecordModel model = null;
    	try {
			model = DogBonusAccountRecordDao.getInstance().selectDailyRecord(host.getId());
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		
		if(model == null){
			GuideLogger.printLog(" host:"+host.getId()+" NotIncToday");
			return false;
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" AlreadyInc:"+model.getAct_time()+" IncType:"+model.getAct_type());
			return true;
		}
    }
    
    public int doDailyIncreament(User host,int amount){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
    	try {
    		GuideLogger.printLog(" host:"+host.getId()+" record_daily_inc:"+amount);
			ret = DogBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IDogBonusConst.ACCOUNT_RECORD_TYPE_DAILYINCREAMENT , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_daily_inc:"+amount);
			DogBonusAccountDao.getInstance().increase(host.getId(), amount);
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
    public int doAcceptedIncreament(User host,int inviterid,int amount){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(amount <= 0){
    		GuideLogger.printLog(" host:"+host.getId()+" inc:"+amount,GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
    	try {
    		GuideLogger.printLog(" host:"+host.getId()+" record_accept_inc "+amount+" inviter:"+inviterid);
			ret = DogBonusAccountRecordDao.getInstance().insertRecord(inviterid, IDogBonusConst.ACCOUNT_RECORD_TYPE_INVITEACCEPANCEINCREAMNET , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_accept_inc "+amount+" inviter:"+inviterid);
			DogBonusAccountDao.getInstance().increase(inviterid, amount);
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
    
    public List<DogBonusAccountRecordModel> get(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	
    	List<DogBonusAccountRecordModel>  ret = null;
    	try {
			ret = DogBonusAccountRecordDao.getInstance().selecById(host.getId());
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e){
    		GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
		
		return ret;
    }
    
    public int doInviteIncreament(User host,int amount){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(amount <= 0){
    		GuideLogger.printLog(" host:"+host.getId()+" inc:"+amount,GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
    	try {
    		GuideLogger.printLog(" host:"+host.getId()+" record_invite_inc "+amount);
			ret = DogBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IDogBonusConst.ACCOUNT_RECORD_TYPE_INVITEINCREAMNET , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_invite_inc "+amount);
			DogBonusAccountDao.getInstance().increase(host.getId(), amount);
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
    
    public int doPayDecreament(User host,int amount){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(amount <= 0){
    		GuideLogger.printLog(" host:"+host.getId()+" inc:"+amount,GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
    	try {
    		GuideLogger.printLog(" host:"+host.getId()+" record_pay_dec "+amount);
			ret = DogBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IDogBonusConst.ACCOUNT_RECORD_TYPE_PAYDECREAMNET , -1*amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_pay_dec "+amount);
			DogBonusAccountDao.getInstance().increase(host.getId(), -1*amount);
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
