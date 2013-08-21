package com.xiaonei.reg.activity.dogbonus.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.dogbonus.constant.IDogBonusConst;
import com.xiaonei.reg.activity.dogbonus.dao.DogBonusInviteDao;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusInviteModel;
import com.xiaonei.reg.guide.util.GuideLogger;



public class DogBonusInviteLogic {
	private static DogBonusInviteLogic instance = new DogBonusInviteLogic();

    public static DogBonusInviteLogic getInstance() {
        return instance;
    }
    
    public int [] insertBatch(User host,List<DogBonusInviteModel> list){
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
			ret = DogBonusInviteDao.getInstance().batchInsert(list);
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
			ret = DogBonusInviteDao.getInstance().updateByInviteeid(inviteeid);
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
			ret = DogBonusInviteDao.getInstance().updateByInviterid(inviterid);
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
			ret = DogBonusInviteDao.getInstance().batchUpdateFlag(inviters, inviteeid);
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
			ret = DogBonusInviteDao.getInstance().acceptByInviteridAndInviteeid(inviterid, inviteeid);
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
    
    public List<DogBonusInviteModel> getByInviterIdAndToday(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusInviteModel> list = null;
    	try {
			list = DogBonusInviteDao.getInstance().selectByInviterIdAndToday(host.getId());
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
    
    public List<DogBonusInviteModel> getByInviterId(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusInviteModel> list = null;
    	try {
			list = DogBonusInviteDao.getInstance().selectByInviterId(host.getId());
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
    	List<DogBonusInviteModel> list = null;
    	try {
			list = DogBonusInviteDao.getInstance().selectByInviteeIdForLink(inviteeid);
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	catch (Exception e){
    		GuideLogger.printLog(" inviteeid:"+inviteeid+" e:"+e.toString(),GuideLogger.ERROR);
    		e.printStackTrace();
    	}
    	
    	String ids = " ";
		
    	if(list == null){
    		list = new ArrayList<DogBonusInviteModel>();
    	}
		for (DogBonusInviteModel m : list) {
			ids = "_"+m.getId()+ids;
		}
		
		ids = ids.substring(1).trim();
		
		String ret = urlguide+"/"+IDogBonusConst.URL_ADDRESS+"?a=accept&ids="+ids;
    	GuideLogger.printLog(" inviteeid:"+inviteeid+" link:"+ret);
    	return ret;
    }
    public List<DogBonusInviteModel> getByInviterIdForLink(User host){
    	if(host == null){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusInviteModel> list = null;
    	try {
			list = DogBonusInviteDao.getInstance().selectByInviterIdForLink(host.getId());
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
    
    public List<DogBonusInviteModel> getByInviterIdForLink(int host){
    	if(host == 0){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	List<DogBonusInviteModel> list = null;
    	try {
			list = DogBonusInviteDao.getInstance().selectByInviterIdForLink(host);
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
			list = DogBonusInviteDao.getInstance().selectAllCanLinkInviteeId(host,limit);
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

	public DogBonusInviteModel getById(Integer id) {
		if(id == 0){
    		GuideLogger.printLog(" host null ",GuideLogger.WARN);
    		return null;
    	}
    	DogBonusInviteModel ret = null;
    	try {
			ret = DogBonusInviteDao.getInstance().selectById(id);
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
