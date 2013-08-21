package com.xiaonei.platform.component.tools;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.component.dao.StarDAO;
import com.xiaonei.platform.component.view.UserTinyView;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.jdbc.dao.UniversityDAO;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.StringFormat;

public class UserUtil {
	
	/**
	 * Author:wei.xiang
	 * Date:2009-03-30
	 * 判断是否为用ID注册用户(高中与教育部合作) 
	 **/
	public static boolean isIdUser(User user){
		try{
			if(user.getStage()==10&&!StringFormat.isValidEmailAddr(user.getEmail())&&!StringFormat.isValidMobile(user.getEmail())){
				return true;
			}
			else{
				return false;
			}			
		}
		catch(Exception e){
			return false;
		}		
	}
	
	/**
	 * Author:wei.xiang
	 * Date:2009-04-02
	 * 取校内人气之星 
	 **/
	public static Map<String, Object> getPeopleStar(User user,int starNumber){
		Map<String, Object> map = new HashMap<String, Object>();
		List<UserTinyView> starList = null;
		try	{// 人气之星
			if (user.getStage() == Stage.STAGE_UNIV){
				starList = StarDAO.getInstance().getByNetwork(user.getUniv(),starNumber);				
				University starUniv = NetworkManager.getInstance().getUnivById(user.getUniv());
				if ((starList == null) || (starList.size() == 0)){
					starUniv = UniversityDAO.getInstance().getOnePopUniv(user.getUniv());
					if (starUniv == null){
						starList = StarDAO.getInstance().getByTop(starNumber);
					} else{
						starList = StarDAO.getInstance().getByNetwork(starUniv.getId(),starNumber);
						if ((starList == null) || (starList.size() == 0)){
							starUniv = null;
							starList = StarDAO.getInstance().getByTop(starNumber);
						}
					}
				}
				map.put("starUniv", starUniv);
			} else if (user.getStage() == Stage.STAGE_HEIGHSCHOOL){
				starList = StarDAO.getInstance().getByNetwork(user.getUniv(),starNumber);				
				if (starList.size() < 20){
					// 数量不足时的填补
					List<UserTinyView> sl = null;
					sl = StarDAO.getInstance().getByNetwork(user.getUniv() + 1,starNumber);
					if (sl != null){
						starList.addAll(sl);
					}
				}
				map.put("starNetwork", NetworkManager.getInstance()
						.getHighSchoolById(user.getUniv()));
			} else if (user.getStage() == Stage.STAGE_MAJOR){
				starList = StarDAO.getInstance().getByNetwork(user.getUniv(),starNumber);
				map.put("starNetwork", NetworkManager.getInstance().getWorkplaceById(user.getUniv()));
			}
		} catch (Exception e){
			e.printStackTrace();
		}
		if(starList != null){
			map.put("starList", starList);
		}
		else{
			List<UserTinyView> nullList=new ArrayList<UserTinyView>();
			map.put("starList", nullList);
		}
		return map;	
	}
}
