package com.xiaonei.reg.activity.recallbonus.logic;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.recallbonus.constant.IRecallBonusConst;
import com.xiaonei.reg.activity.recallbonus.dao.RecallBonusAccountDao;
import com.xiaonei.reg.activity.recallbonus.dao.RecallBonusAccountRecordDao;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountModel;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel;
import com.xiaonei.reg.guide.util.GuideLogger;


public class RecallBonusAccountLogic {
	private static RecallBonusAccountLogic instance = new RecallBonusAccountLogic();

    public static RecallBonusAccountLogic getInstance() {
        return instance;
    }
    
    public int getAccountCurrent(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	
    	RecallBonusAccountModel model = null;
    	try {
			model = RecallBonusAccountDao.getInstance().selectByUid(host.getId());
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
    
    public boolean alreadyDoDailyRecordByType(User host,int type){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return true;
    	}
    	
    	RecallBonusAccountRecordModel model = null;
    	try {
			model = RecallBonusAccountRecordDao.getInstance().selectDailyRecordByType(host.getId(),type);
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
			GuideLogger.printLog(" host:"+host.getId()+" NotDoToday"+" type:"+type);
			return false;
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" AlreadyInc:"+model.getAct_time()+" type:"+type);
			return true;
		}
    }
    
    public boolean alreadyGetDailyIncreament(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return true;
    	}
    	
    	RecallBonusAccountRecordModel model = null;
    	try {
			model = RecallBonusAccountRecordDao.getInstance().selectDailyRecord(host.getId());
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
    		GuideLogger.printLog(" host:"+host.getId()+" record_daily_inc "+amount);
			ret = RecallBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IRecallBonusConst.ACCOUNT_RECORD_TYPE_DAILYINCREAMENT , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_daily_inc "+amount);
			RecallBonusAccountDao.getInstance().increase(host.getId(), amount);
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
			ret = RecallBonusAccountRecordDao.getInstance().insertRecord(inviterid, IRecallBonusConst.ACCOUNT_RECORD_TYPE_INVITEACCEPANCEINCREAMNET , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_accept_inc "+amount+" inviter:"+inviterid);
			RecallBonusAccountDao.getInstance().increase(inviterid, amount);
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
    
    public List<RecallBonusAccountRecordModel> get(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	
    	List<RecallBonusAccountRecordModel>  ret = null;
    	try {
			ret = RecallBonusAccountRecordDao.getInstance().selecById(host.getId());
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
			ret = RecallBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IRecallBonusConst.ACCOUNT_RECORD_TYPE_INVITEINCREAMNET , amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_invite_inc "+amount);
			RecallBonusAccountDao.getInstance().increase(host.getId(), amount);
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
			ret = RecallBonusAccountRecordDao.getInstance().insertRecord(host.getId(), IRecallBonusConst.ACCOUNT_RECORD_TYPE_PAYDECREAMNET , -1*amount);
			GuideLogger.printLog(" host:"+host.getId()+" account_pay_dec "+amount);
			RecallBonusAccountDao.getInstance().increase(host.getId(), -1*amount);
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

	public int doDailyRecordByType(User host, int type,int amount) {
		if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	
    	int ret = 0;
    	try {
    		GuideLogger.printLog(" host:"+host.getId()+" record_daily_type "+amount);
			ret = RecallBonusAccountRecordDao.getInstance().insertRecord(host.getId(), type , amount);
			/*GuideLogger.printLog(" host:"+host.getId()+" account_daily_inc "+amount);
			RecallBonusAccountDao.getInstance().increase(host.getId(), amount);*/
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
