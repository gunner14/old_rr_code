package com.xiaonei.reg.activity.recallbonus.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.recallbonus.dao.RecallBonusInviteDao;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusInviteModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class RecallBonusInviteLogic {
	private static RecallBonusInviteLogic instance = new RecallBonusInviteLogic();

    public static RecallBonusInviteLogic getInstance() {
        return instance;
    }
    
    public int [] insertBatch(User host,List<RecallBonusInviteModel> list){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	if(list == null){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return null;
    	}
    	
    	int [] ret = null;
    	try {
			ret = RecallBonusInviteDao.getInstance().batchInsert(list);
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
    
    public int updateByInvitee(User host, int inviteeid){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(inviteeid == 0){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return 0;
    	}
    	
    	int  ret = 0;
    	try {
			ret = RecallBonusInviteDao.getInstance().updateByInviteeid(inviteeid);
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
    
    public int updateByInviter(User host, int inviterid){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(inviterid == 0){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return 0;
    	}
    	
    	int  ret = 0;
    	try {
			ret = RecallBonusInviteDao.getInstance().updateByInviterid(inviterid);
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
    
    public int[] acceptBatch(User host, List<Integer> inviters,int inviteeid){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	if(inviters == null){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return null;
    	}
    	if(inviteeid == 0){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return null;
    	}
    	
    	int[]  ret = null;
    	try {
			ret = RecallBonusInviteDao.getInstance().batchUpdateFlag(inviters, inviteeid);
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
    
    public int accept(User host, int inviterid,int inviteeid){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return 0;
    	}
    	if(inviterid == 0){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return 0;
    	}
    	if(inviteeid == 0){
    		GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
    		return 0;
    	}
    	
    	int  ret = 0;
    	try {
			ret = RecallBonusInviteDao.getInstance().acceptByInviteridAndInviteeid(inviterid, inviteeid);
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
    
    public List<RecallBonusInviteModel> getByInviterIdAndToday(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusInviteModel> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectByInviterIdAndToday(host.getId());
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
    
    public List<RecallBonusInviteModel> getByInviterId(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusInviteModel> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectByInviterId(host.getId());
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
    public String getInviteeLink(int inviteeid, String urlguide){
    	List<RecallBonusInviteModel> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectByInviteeIdForLink(inviteeid);
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" inviteeid:"+inviteeid+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	if(list == null) list = new ArrayList<RecallBonusInviteModel>();
    	
    	String ids = " ";
		
		for (RecallBonusInviteModel m : list) {
			ids = "_"+m.getId()+ids;
		}
		
		ids = ids.substring(1).trim();
    	GuideLogger.printLog(" inviteeid:"+inviteeid+" link:"+urlguide+"/rcbonus?a=accept&ids="+ids);
    	String ret = urlguide+"/rcbonus?a=accept&ids="+ids;
    	return ret;
    }
    public List<RecallBonusInviteModel> getByInviterIdForLink(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusInviteModel> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectByInviterIdForLink(host.getId());
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
    
    public List<RecallBonusInviteModel> getByInviterIdForLink(int host){
    	if(host == 0){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<RecallBonusInviteModel> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectByInviterIdForLink(host);
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	return list;
    }

	public List<Integer> getAllInviteeId(int host, int limit) {
		if(host == 0){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<Integer> list = null;
    	try {
			list = RecallBonusInviteDao.getInstance().selectAllCanLinkInviteeId(host,limit);
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" host:"+host+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	return list;
	}

	public RecallBonusInviteModel getById(Integer id) {
		if(id == 0){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	RecallBonusInviteModel ret = null;
    	try {
			ret = RecallBonusInviteDao.getInstance().selectById(id);
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		//GuideLogger.printLog(" host:"+id+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	return ret;
		
	}
    
    
}
