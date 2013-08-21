package com.xiaonei.reg.guide.util;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.lang.StringUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import Ice.TimeoutException;

import com.dodoyo.friends.relationfriend.logic.IRelation;
import com.dodoyo.friends.relationfriend.logic.IRelationFriend;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAwardModel;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserRegisterExtraMessageUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.model.RegExtraMsg;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.sns.platform.core.opt.ice.IUserCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class GuideUtil {
	public static boolean isRenrenUser(User host){
		if(host == null) return true;
		if("renren.com".equals(host.getDomain()) && "renren.com".equals(OpiConstants.domain.toString())){
			return true;
		}
		else{
			return false;
		}
	}

	public static boolean isKaixinUser(User host){
		if(host == null) return true;
		if("kaixin.com".equals(host.getDomain()) && "kaixin.com".equals(OpiConstants.domain.toString())){
			return true;
		}
		else{
			return false;
		}
	}
	
	public static boolean isNetbarUser(User host){
		//2009-12-24 18:27:20 为了不和面馆的冲突
		if(isXFiveAppMZMGUser(host)){
			return false;
		}
		String fromSource = RegFromSourceLogic.getFromSource(host);
		List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromSource);
		if(fromlist.contains(RegFromSourceLogic.FS_NETBAR)){
			GuideLogger.printLog("hostid:" + host.getId()+" isNetbarUser");
			return true;
		}
		return false;
	}
	
	public static boolean isSGGood(User host){
		boolean ret = false;
		GuideLogger.printLog("hostid:" + host.getId()+"");
		int dec = GuideXFiveUserDecisionLogic.getInstance().getDecision(host.getId());
		
		if(dec == 1){
			GuideLogger.printLog("hostid:" + host.getId()+" sggood");
			ret = true;
		}
		else{
			GuideLogger.printLog("hostid:" + host.getId()+" sgbad");
		}
		return ret;
	}
	
	//产品硬牵过去的用户
	public static boolean needGoToXFiveUser(User host){
		int id = 0;
		try {
			id = host.getId();
			//2009-11-25 18:20:32 加上为1的用户
			//2009-11-30 10:00:12 把尾号为1的改成尾号为6的
			//2009-12-01 14:09:51 加上尾号是7的 走其它阶段新表单  X5V4 XFive-Blue
			//2009-12-8 16:43:19 加上尾号是8的，走大推人页即 coral流程 ——诗伦
			//2009-12-8 16:43:50 加上尾号是2的，用现有尾号为7的人的引导。选择以上都不是，确定之后，进网吧引导，之后全部使用网吧流程产品。——诗伦
			//2009-12-18 14:44:30晓堂的需求，扩大尾号！
			//2009-12-28 14:25:27加上尾号是3的，OscarXu的需求
			//2010-3-29 15:34:37加上尾号是9的，诗伦陈周的需求
			if(id % 10 == 0 || id % 10 == 1
					|| id % 10 == 6 || id % 10 == 5
					|| id % 10 == 7 || id % 10 == 8
					/*|| id % 10 == 8*/ 
					|| id % 10 == 2 || id % 10 == 4
					|| id %10 == 3
					|| id %10 == 9
					){ 
				GuideLogger.printLog("hostid:" + host.getId()+" need to");
				return true;
			}
			else{
				GuideLogger.printLog("hostid:" + host.getId()+" need not");
				return false;
			}
		} catch (Exception e) {
			GuideLogger.printLog(" get id error! err:" + e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog("hostid:" + host.getId()+" need def");
		return false;
	}

	//走逻辑走出来 的用户
	public static boolean isXFiveUser(User host) {
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.ERROR);
			return false;
		}
		try {
			if(needGoToXFiveUser(host)){
				//return true;
				//要加一个逻辑 2009-11-24 15:18:37 晓棠 老的新的得分开
				if(GuideXFiveUserDecisionLogic.getInstance().getDecision(host.getId()) != GuideXFiveUserDecisionLogic.SOCIAL_GRAPH_NOTSET){
					GuideLogger.printLog("hostid:" + host.getId()+" isX5");
					return true;
				}
				
				else{
					GuideLogger.printLog("hostid:" + host.getId()+" notX5");
					return false;
				}
			}
			//2009-12-11 17:44:15 网吧的也要来凑热闹，哎……硬加上吧！——Inshion
			else if(isNetBarXFiveUser(host)){
				GuideLogger.printLog("hostid:" + host.getId()+" isNetbarX5");
				return true;
			}
			//2009-12-24 17:48:13麻子面馆的也要来……哎
			else if(isXFiveAppMZMGUser(host)){
				GuideLogger.printLog("hostid:" + host.getId()+" isMXMGX5");
				return true;
			}
			else{
				GuideLogger.printLog("hostid:" + host.getId()+" notX5");
				return false;
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId() + e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog("hostid:" + host.getId()+" def notX5");
		return false;
	}

	public static boolean isNetBarXFiveUser(User host){
		String fromSource = RegFromSourceLogic.getFromSource(host);
		List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromSource);
		if(fromlist.contains(RegFromSourceLogic.FS_NETBARXFIVE)){
			GuideLogger.printLog("hostid:" + host.getId()+" isNBX5User");
			return true;
		}
		return false;
	}

	public static boolean isXFiveAppMZMGUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		String fromsource = RegFromSourceLogic.getFromSource(host);
		List<String> list = RegFromSourceLogic.getFromSoureList(fromsource);
		if (list.contains(RegFromSourceLogic.FS_MAZIMIANGUAN)) {
			ret = true;
		}
		return ret;
	}
	
	public static boolean isXinSheng2010(User host){
	    if (host == null)
            return false;
        boolean ret = false;
        String fromsource = RegFromSourceLogic.getFromSource(host);
        List<String> list = RegFromSourceLogic.getFromSoureList(fromsource);
        if (list.contains(RegFromSourceLogic.FS_XS2010)) {
            ret = true;
        }
        return ret;
	}

	public static String getJSONForFriendSelector(List<IRelation> mayFriends) {
		try {
			JSONArray retCandidate = new JSONArray();
			JSONObject jo = new JSONObject();
			try {
				int safeflag = 500; 
				
				int listscount = mayFriends.size();
				IRelation [] relationlist = mayFriends.toArray(new IRelation[mayFriends.size()]);
				int [] listsize = new int [listscount]; 
				int sum = 0;
				for (int i = 0; i < relationlist.length; i++) {
					List<IRelationFriend> list = relationlist[i].getList();
					listsize[i] = list.size();
					sum += list.size();
				}
				
				while(sum > 0 && safeflag -- > 0){
					for (int i = 0; i < relationlist.length; i++) {
						List<IRelationFriend> list = relationlist[i].getList();
						if(listsize[i] -- > 0){//说明该List还有东西
							IRelationFriend f = list.get(listsize[i]);
							JSONObject joinner = new JSONObject();
							joinner.put("id", f.getId());
							joinner.put("head", f.getTiny_Url());
							joinner.put("name", f.getName());
							joinner.put("net", relationlist[i].getTypeName());
							retCandidate.put(joinner);
							sum -- ;
						}
					}
				}
				
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return "";
	}	
	
	public static String getJSONForFriendSelectorFromIds(int hostid ,List<Integer> ids) {
		String ret = "";
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			ret = getJSONForFriendSelectorFromCacheMapBySort(hostid,ucl);
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return ret;
	}
	
	public static String getJSONForFriendSelectorFromIdsForSearchfriend(List<Integer> ids) {
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			int i = 0;
			for (int curid : ids) {
				i++;
				WUserCache userCache = ucl.get(curid);
				if(userCache != null){
					JSONObject joinner = new JSONObject();
					joinner.put("id", curid);
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", MyStringUtils.getSubstring(userCache.getName(), 7));
					joinner.put("fullName", userCache.getName());
					joinner.put("net", MyStringUtils.getSubstring(userCache.getUnivName(), 7));
					joinner.put("fullNet", userCache.getUnivName());
			        retCandidate.put(joinner);
				}
				if(i>=200)break;
				
			}

			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return "";
	}
	
	public static String getJSONForFriendSelectorFromIdsForMorefriend(List<Integer> ids) {
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			int i = 0;
			for (int curid : ids) {
				i++;
				WUserCache userCache = ucl.get(curid);
				if(userCache != null){
					JSONObject joinner = new JSONObject();
					joinner.put("id", curid);
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", MyStringUtils.getSubstring(userCache.getName(), 5));
					joinner.put("fullName", userCache.getName());
					joinner.put("net", MyStringUtils.getSubstring(userCache.getUnivName(), 5));
					joinner.put("fullNet",userCache.getUnivName());
			        retCandidate.put(joinner);
				}
				if(i>=4)break;
				
			}

			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return "";
	}
	
	public static String getJSONForFriendSelectorFromIdsForPortrait(List<Integer> ids) {
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			
			int i = 0;
			for (int curid : ids) {
				i++;
				WUserCache userCache = ucl.get(curid);
				if(userCache != null){
					JSONObject joinner = new JSONObject();
					joinner.put("id", curid);
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", userCache.getName());
					joinner.put("net", userCache.getUnivName());
					retCandidate.put(joinner);
				}
				
			}
			
			for(;i < 8;i++){
				JSONObject joinner = new JSONObject();
				joinner.put("id", "0");
				joinner.put("head", heads[7-i]);
				joinner.put("name", "");
				joinner.put("net", "");
				retCandidate.put(joinner);
			}
			
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return "";
	}

	public static String getJSONForFriendSelectorForBounusResult(List<RecallBonusAwardModel> list, int limit) {
		try{
			if(list == null){
				GuideLogger.printLog("");
				return "";
			}
			List<Integer> ilist = new ArrayList<Integer>();
			List<RecallBonusAwardModel> finallist = new ArrayList<RecallBonusAwardModel>();
			for (RecallBonusAwardModel m : list) {
				if(!ilist.contains(m.getUid())){
					ilist.add(m.getUid());
					finallist.add(m);
				}
			}
			
			Map<Integer, WUserCache> ucl = getUserCacheMap(ilist);
			// 返回数据
			JSONObject jo = new JSONObject();
	        JSONArray retCandidate = new JSONArray();
	        int i = 0;
	        for (int curid : ilist) {
	        	WUserCache userCache = ucl.get(curid);
	        	if(userCache != null){
	        		if(MyStringUtils.contains(userCache.getName(), "人人测试")){
	        			break;
	        		}
	        		JSONObject joinner = new JSONObject();
					joinner.put("id", curid);
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", MyStringUtils.getSubstring(userCache.getName(), 5));
					joinner.put("fullName", userCache.getName());
					joinner.put("net", MyStringUtils.getSubstring(userCache.getUnivName(), 5));
					joinner.put("fullNet",userCache.getUnivName());
					joinner.put("award", finallist.get(i).getAward_remark());
	                retCandidate.put(joinner);
	        	}
	        	if(++i>=limit)break;
				
			}
	
	        jo.put("candidate", retCandidate);
	        return jo.toString();	
		}
		catch(Exception e){
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return "";
		
	}

	public static String getJSONForFriendSelectorFromIdsForKaixinDog(List<Integer> ids) {
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			int i = 0;
			for (int curid : ids) {
				if(curid < 700000000) continue;
				i++;
				WUserCache userCache = ucl.get(curid);
				if(userCache != null){
					JSONObject joinner = new JSONObject();
					joinner.put("id", curid);
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", userCache.getName());
					joinner.put("net", userCache.getUnivName());
			        retCandidate.put(joinner);
				}
				if(i>=200)break;
				
			}
	
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return "";
	}

	private static String [] heads={
		"http://hdn.xnimg.cn/photos/hdn321/20100208/1600/tiny_ERSA_52310o019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn601/20090202/18/15/tiny_CFOI_5440n198107.jpg",
		"http://hdn.xnimg.cn/photos/hdn321/20091215/1025/tiny_m5NT_16516g019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn221/20091109/1435/tiny_yTfp_110058l019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn321/20091016/1740/tiny_Qp6x_48834o019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn321/20100107/2000/tiny_iga4_49939k019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn421/20100302/2310/tiny_tkSg_162390j019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn121/20100206/0135/tiny_CdHc_39567f019118.jpg"
	};
	
	public static Map<Integer, WUserCache> getUserCacheMapByIsActive(List<Integer> idlist){
		Map<Integer, WUserCache> ucl = getUserCacheMap(idlist);
		//去掉不活的
		List<Integer> dellist = new ArrayList<Integer>();
		for (Entry<Integer, WUserCache> ent : ucl.entrySet()) {
			if(ent!=null && ent.getValue() != null && !ent.getValue().isActive()){
				dellist.add(ent.getKey());
			}
		}
		
		for (Integer deli : dellist) {
			ucl.remove(deli);
		}
		return ucl;
	}
	public static Map<Integer, WUserCache> getUserCacheMap(List<Integer> idlist){
		IUserCacheAdapter adapter = null;
		try {
			adapter = SnsAdapterFactory.getUserCacheAdapter();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Map<Integer, WUserCache> ucl = null;
		if(adapter != null){
			try {
				ucl = adapter.getUserCacheMap(idlist);
				
				//Integer.valueOf(i);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if(ucl == null){
			ucl = new HashMap<Integer, WUserCache>();
		}
		
		//去掉封禁
		/*List<Integer> dellist = new ArrayList<Integer>();
		for (Entry<Integer, WUserCache> ent : ucl.entrySet()) {
			if(ent!=null && ent.getValue() != null && ent.getValue().getStatus()!=0){
				dellist.add(ent.getKey());
			}
		}
		
		for (Integer deli : dellist) {
			ucl.remove(deli);
		}*/
		
		return ucl;
	}
	public static boolean isAppInviteRegisteredUser(User host){
		int appid = GuideXFiveUserRegisterExtraMessageUtil.getInvitedAppid(host);
		if(appid == 0) return false;
		return true;
	}
	
	public static boolean isConnectRegisteredUser(User host){
		int appid = GuideXFiveUserRegisterExtraMessageUtil.getConnectedAppid(host);
		if(appid == 0) return false;
		return true;
	}
	
	
	
	public static boolean hasExtra_XXX(User host,String XXX){
		boolean ret = false;
		if(host == null){
			GuideLogger.printLog(" host:"+0,GuideLogger.WARN);
		}
		else{
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					map = new HashMap<String, RegExtraMsg>();
				}
				RegExtraMsg msg = map.get(XXX);
				if(msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
				}
				else{
					//String strret = msg.getInput_value();
					ret = true;
				}
			}
			catch(NumberFormatException e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				//e.printStackTrace();
			}
			catch(Exception e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		
		return ret;
	}
	
	public static int getExtra_reg_school_renren(User host){
		int ret = 0;
		if(host == null){
			GuideLogger.printLog(" host:"+0,GuideLogger.WARN);
		}
		else{
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					map = new HashMap<String, RegExtraMsg>();
				}
				RegExtraMsg msg = map.get(IRegExtraMsgLogic.reg_school_renren);
				if(msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
				}
				else{
					//String strret = msg.getInput_value();
					ret = 1;
				}
			}
			catch(NumberFormatException e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				//e.printStackTrace();
			}
			catch(Exception e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		
		return ret;
	}
	public static String getExtra_XXXString(User host,String xxx){
		String ret = "";
		if(host == null){
			GuideLogger.printLog(" host:"+0,GuideLogger.WARN);
		}
		if(xxx == null || "".equals(xxx)){
			GuideLogger.printLog(" xxx:"+xxx,GuideLogger.WARN);
		}
		else{
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					map = new HashMap<String, RegExtraMsg>();
				}
				RegExtraMsg msg = map.get(xxx);
				if(msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
				}
				else{
					String strret = msg.getInput_value();
					ret = strret;
				}
			}
			catch(Exception e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
		}
		return ret;
	}
	
	public static int getExtra_XXX(User host,String xxx){
		int ret = 0;
		if(host == null){
			GuideLogger.printLog(" host:"+0,GuideLogger.WARN);
		}
		if(xxx == null || "".equals(xxx)){
			GuideLogger.printLog(" xxx:"+xxx,GuideLogger.WARN);
		}
		else{
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					map = new HashMap<String, RegExtraMsg>();
				}
				RegExtraMsg msg = map.get(xxx);
				if(msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
				}
				else{
					String strret = msg.getInput_value();
					if(!MyStringUtils.isNullOrEmpty(strret)){
					    ret=1;
					}
				}
			}
			catch(NumberFormatException e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				//e.printStackTrace();
			}
			catch(Exception e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		
		return ret;
	}
	
	public static int getExtra_appconnect_id(User host){
		int ret = 0;
		if(host == null){
			GuideLogger.printLog(" host:"+0,GuideLogger.WARN);
		}
		else{
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					map = new HashMap<String, RegExtraMsg>();
				}
				RegExtraMsg msg = map.get(IRegExtraMsgLogic.key_reg_app_connect);
				if(msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
				}
				else{
					String strret = msg.getInput_value();
					ret = Integer.parseInt(strret);
				}
			}
			catch(NumberFormatException e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				//e.printStackTrace();
			}
			catch(Exception e){
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		
		return ret;
	}
	public static RegInviteMap getAppInviteMap(User host){
		if(host == null){
			return null;
		}
		RegInviteMap info = null;
		if(MyStringUtils.contains(host.getDomain(), "kaixin")){
			com.kaixin.register.model.RegInviteMap k = com.kaixin.register.logic.additionalbz.RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());;
			if(k != null){
				info = new RegInviteMap();
				info.setId(k.getId());
				info.setAppId(k.getAppId());
				info.setInviterGroupName(k.getInviterGroupName());
				info.setInviterId(k.getInviterId());
				info.setOwnerId(k.getOwnerId());
				info.setTime(k.getTime());
			}
		}
		else{
			info = RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());
		}
		return info;
	}
	
	public static void main(String[] args) throws ParseException {
		for(int i = 0 ;i<8;i++){
			//System.out.println(7-i);
		}
		Date today = new Date();
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
	    //long to = df.parse("2010-3-25 00:00:01").getTime();
	    //long to = df.parse("2010-3-25 23:59:59").getTime();
	    long to = today.getTime();
	    long from = df.parse("2010-3-22 00:00:00").getTime();
	    System.out.println((to - from) / (1000 * 60 * 60 * 24));
	    String url = "a.do?a=1&amp;b=2&amp;c=3";
	    url = url.replace("&amp;","&");
	    System.out.println(url);
	    
	    System.out.println(MyStringUtils.getSubstring("中国是一个人",4));
	    
	    JSONObject jo1 = new JSONObject();
	    jo1.put("1", "1");
	    JSONObject jo2 = new JSONObject();
	    jo2.put("2", "2");
	    JSONObject jo3 = new JSONObject();
	    jo3.put("3", "3");
	    JSONObject jo4 = new JSONObject();
	    jo4.put("4", "4");
		JSONArray ja = new JSONArray();
		ja.put(0,jo1);
		ja.put(1,jo2);
		ja.put(2,jo3);
		ja.put(3,jo4);
		System.out.println(ja.toString());
		
		Map<Integer,String> map = new HashMap<Integer, String>();
		map.put(1, "asdfasdf");
		map.put(2, "asdfasdf1");
		map.put(5, "asdfasdf2");
		map.put(6, "asdfasdf3");
		
		for (Entry<Integer,String> ent : map.entrySet()) {
			System.out.println(ent.getValue());
			
		}
		
		System.out.println();
	}
	
	
	@SuppressWarnings("unused")
	private static int getKeepLoginDayFrom20100324(User host){
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.WARN);
		}
		Date today = new Date();
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
	    long to = today.getTime();
	    long from = today.getTime();
		try {
			from = df.parse("2010-3-22 00:00:00").getTime();
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		long limit = ((to - from) / (1000 * 60 * 60 * 24));
		if(limit < 1 ){
			limit = 1;
		}
		int cnt = 1;
		try {
			cnt = LoginLogicFactory.getPassportUserScore().getLoginConsecutiveCount(host.getId());
			GuideLogger.printLog(" host:"+host.getId()+" keepday:"+cnt);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		int ourdays = 1;
		ourdays = (int) (cnt >= limit ? limit : cnt);
		GuideLogger.printLog(" host:"+host.getId()+" cnt:"+cnt+" limit:"+limit+" ourdays:"+ourdays);
		return ourdays;
	}
	
	public static List<Integer> getFriendIdList(User host){
		List<Integer> list = new ArrayList<Integer>();
		if(host == null){
			return list;
		}
		try {
			list = com.xiaonei.platform.component.friends.home.FriendsHome.getInstance().getFriendsIdsByName(host.getId());
		} catch (TimeoutException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (IndexOutOfBoundsException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		}
		catch (Exception e){
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		}
		if(list.isEmpty()){
			GuideLogger.printLog(" host:"+host.getId(),GuideLogger.WARN);
		}
        return list;
	}
	public static User getAFriend(User host) {
		User ret = null;
		List<Integer> list = getFriendIdList(host);
		if(list != null && list.size() > 0){
			int id = list.get(0);
			try {
				ret = SnsAdapterFactory.getUserAdapter().get(id);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			}
		}
		return ret;
	}
	
	public static User getAInviter(User host){
		User ret = null;
		if(host == null){
		}
		else{
			RegInviteMap map = getAppInviteMap(host);
			if(map!=null){
				int id = map.getInviterId();
				try {
					ret = SnsAdapterFactory.getUserAdapter().get(id);
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				}
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" invite map null!",GuideLogger.WARN);
			}
		}
		return ret;
	}
	
    public static boolean hasUploaded(User user) {
        return !StringUtils.equals(user.getHeadUrl(), Globals.userWomenHeadUrl)
                && !StringUtils.equals(user.getHeadUrl(), Globals.userMenHeadUrl)
                && !StringUtils.equals(user.getHeadUrl(), Globals.userHeadUrl);

    }
    private static boolean isNotDefaultTinyUrl(String headurl) {
    	return !StringUtils.contains(headurl, Globals.userTinyUrl)
    	&& !StringUtils.contains(headurl, Globals.userMenTinyUrl)
    	&& !StringUtils.contains(headurl, Globals.userWomenTinyUrl);
    	
    }

	public static String getJSONForFriendSelectorFromCacheMapBySort(int hostid,Map<Integer, WUserCache> fs) {
		try {
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			List<WUserCache> nogoodman = new ArrayList<WUserCache>();
			List<WUserCache> isOnlineList = new ArrayList<WUserCache>();
			List<WUserCache> hasHeadList = new ArrayList<WUserCache>();
			int i = 0;
			int ng_i = 0;
			for (Entry<Integer, WUserCache> cur : fs.entrySet()) {
				//TODO 
				if(!"".equals("")){
					ng_i ++ ;
					continue;
				}
				i++;
				WUserCache userCache = cur.getValue();
				if(userCache != null){
					if( userCache.isSelected()){ //先取星级 
						JSONObject joinner = new JSONObject();
						joinner.put("id", userCache.getId());
						joinner.put("head", userCache.getTiny_Url());
						joinner.put("name", userCache.getName());
						joinner.put("net", userCache.getUnivName());
						retCandidate.put(joinner);
					}
					else if(isNotDefaultTinyUrl(userCache.getTinyUrl())){ //再取有头像
						hasHeadList.add(userCache);
					}
					else if(userCache.isOnline()){ //再取在线
						isOnlineList.add(userCache);
					}
					else{
						nogoodman.add(userCache);
					}
				}
				if(i>=200)break;
				
			}
			
			GuideLogger.printLog(" host:"+hostid+" i:"+i+" ng_i:"+ng_i);
			
			for (WUserCache userCache : hasHeadList) {//有头像
				JSONObject joinner = new JSONObject();
				joinner.put("id", userCache.getId());
				joinner.put("head", userCache.getTiny_Url());
				joinner.put("name", userCache.getName());
				joinner.put("net", userCache.getUnivName());
				retCandidate.put(joinner);
			}
			
			for (WUserCache userCache : isOnlineList) {//在线
				JSONObject joinner = new JSONObject();
				joinner.put("id", userCache.getId());
				joinner.put("head", userCache.getTiny_Url());
				joinner.put("name", userCache.getName());
				joinner.put("net", userCache.getUnivName());
				retCandidate.put(joinner);
			}
				
			
			for (WUserCache userCache : nogoodman) {//没头像的放后面
				JSONObject joinner = new JSONObject();
				joinner.put("id", userCache.getId());
				joinner.put("head", userCache.getTiny_Url());
				joinner.put("name", userCache.getName());
				joinner.put("net", userCache.getUnivName());
				retCandidate.put(joinner);
			}
			
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog("GuideUtil - e:"+e.toString());
			e.printStackTrace();
		}
		return "";
	}

	public static String getJSONForFriendSelectorFromCacheMap(int hostid, Map<Integer, WUserCache> fs, List<Integer> sortlist) {
		try {
			// 返回数据
			JSONObject jo = new JSONObject();
			JSONArray retCandidate = new JSONArray();
			int i = 0;
			int ng_i = 0;
			for (Integer uid : sortlist) {
				WUserCache userCache = fs.get(uid);
				if(userCache != null){
					i++;
					if( userCache.getStatus() == 0){ 
						JSONObject joinner = new JSONObject();
						joinner.put("id", userCache.getId());
						joinner.put("head", userCache.getTiny_Url());
						joinner.put("name", userCache.getName());
						joinner.put("net", userCache.getUnivName());
						joinner.put("act", userCache.isActive()?1:0);
						retCandidate.put(joinner);
					}
					if(!userCache.isActive()){
						ng_i++;
					}
				}
				if(i>=200)break;
			}
			/*
			for (Entry<Integer, WUserCache> cur : fs.entrySet()) {
				WUserCache userCache = cur.getValue();
				if(userCache != null){
					i++;
					if( userCache.getStatus() == 0){ 
						JSONObject joinner = new JSONObject();
						joinner.put("id", userCache.getId());
						joinner.put("head", userCache.getTiny_Url());
						joinner.put("name", userCache.getName());
						joinner.put("net", userCache.getUnivName());
						joinner.put("act", userCache.isActive()?1:0);
						retCandidate.put(joinner);
					}
					if(!userCache.isActive()){
						ng_i++;
					}
				}
				if(i>=200)break;
				
			}
			*/
			
			GuideLogger.printLog(" host:"+hostid+" i:"+i+" ng_i:"+ng_i);
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+hostid+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return "";
		
	}
}
