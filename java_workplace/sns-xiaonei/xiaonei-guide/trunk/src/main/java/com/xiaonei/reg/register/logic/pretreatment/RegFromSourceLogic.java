/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.IRegFromSource;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.controllers.sb.ProfileRegisterController;
import com.xiaonei.reg.register.controllers.sb.ShortCommonRegisterController;
import com.xiaonei.reg.register.controllers.sb.ShortIDCommonRegisterController;
import com.xiaonei.reg.register.controllers.sb.ShortInviteRegisterController;
import com.xiaonei.reg.register.controllers.sb.ShortMobileRegisterController;
import com.xiaonei.reg.register.controllers.sb.ShortPureIDCommonRegisterController;
import com.xiaonei.reg.register.dao.NetbarIpDAO;
import com.xiaonei.reg.register.dao.RegFromSourceDAO;
import com.xiaonei.reg.register.model.RegFromSource;
import com.xiaonei.reg.register.pojo.RegFromSourcePojo;

/**
 * RegFromSourceLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午04:51:19
 * 2010-8-30 添加 public static final String FS_FROMLEADTOAPP = "LTAPP";
 * 2010-8-30 添加 public static final String FS_XS2010 = "XS2010";
 * 
 */
public class RegFromSourceLogic {
	public static final String FS_FARMREG = "S_FARMREG";
	public static final String FS_COOKREG = "S_COOKREG";
	public static final String FS_NETBAR = "NETBAR";
	public static final String FS_NETBARXFIVE = "NBX5";
	public static final String FS_MAZIMIANGUAN = "S_MZMG"; 
	public static final String FS_FROMAPPTOLEAD = "APPTL";
	public static final String FS_PAGE_LOVER = "LOVER";
	public static final String FS_FROMLEADTOAPP = "LTAPP";
	public static final String FS_XS2010 = "XS2010";
	public static final String FS_LEAD_AAA = "_AAA";
	public static final String FS_LEAD_AAE = "_AAE";
	public static final String FS_LEAD_AAF = "_AAF";
	public static final String FS_LEAD_AAH = "_AAH";
	public static final String FS_LEAD_AAI = "_AAI";
	public static final String FS_LEAD_AAJ = "_AAJ";
	public static final String FS_LEAD_AAO = "_AAO";
	public static final String FS_LEAD_AAP = "_AAP";
	public static final String FS_LEAD_AAQ = "_AAQ";
	public static final String FS_LEAD_AAR = "_AAR";
	
	public static final String FS_LEAD_ACB = "_ACB";
	public static final String FS_LEAD_ACE = "_ACE";
	
	
	private static final String MEM_CACHE_FROM_SOURCE = "mem_cache_from_source_";
	private static final MemCacheManager MEM_POOL = MemCacheFactory
			.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
	private static final int MEM_CACHE_TIME = (int) (MemCacheKeys.minute * 30);

	/**
	 * 保存用户信息来源信息
	 * 
	 * @param userId
	 * @param from
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午04:52:18
	 */
	private static int save(final String userId, final String from) {
		if (StringUtils.isEmpty(from)) {
			return 0;
		}
		int ref = 1;
		Runnable tl = new Runnable() {
			@Override
			public void run() {
				try {
					RegFromSourceDAO.getInstance().save("" + userId, from);
				} catch (SQLException e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(tl);
		try {
			MEM_POOL.set(MEM_CACHE_FROM_SOURCE + userId, from, MEM_CACHE_TIME);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ref;
	}

	private static int save(final int userid, final String from) {
		return save("" + userid, from);
	}

	public static int update(final int userid, final String from) {
		return save("" + userid, from);
	}

	/**
	 * 使用用户id 反查来源
	 * 
	 * @param userid
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午05:07:00
	 */
	public static String getFromSource(final User host) {
		String from = null;
		try {
			from = (String) MEM_POOL.get(MEM_CACHE_FROM_SOURCE + host.getId());
		} catch (Exception e1) {
			GuideLogger.printLog("get from Mem failed! host:" + host.getId(),
					GuideLogger.ERROR);
			e1.printStackTrace();
		}
		if (StringUtils.isNotEmpty(from)) {
			return from;
		}
		try {
			if (StringFormat.isValidMobile(host.getAccount())) {
				from = RegFromSourceDAO.getInstance()
						.getFrom(host.getAccount());
			} else {
				from = RegFromSourceDAO.getInstance()
						.getFrom("" + host.getId());
			}
		} catch (SQLException e) {
			e.printStackTrace();
			from = "";
		}
		return from;
	}

	private static String getSourceAsActionId(final String actionId,
			final int userid) {
		if (StringUtils.isEmpty(actionId) || StringUtils.equals("0", actionId)) {
			return null;
		}
		String basename = RegFromSource.PAGEIDS;
		String def = DodoyoResource.getString("rd.actionid." + actionId.trim(),
				basename);
		if (isNotValue("rd.actionid." + actionId.trim(), def)) {
			return null;
		}
		String def2 = DodoyoResource.getString(def, basename);
		if (isValue(def, def2)) {
			return def2;
		}
		return def;
	}

	/**
	 * 保存 reg from source, 封装了获得from source 逻辑, 解除调用端判断source类型的耦合
	 * 
	 * @param pojo
	 */
	public static void saveFromSource(RegFromSourcePojo pojo) {
		String fromSource = "";
		if (pojo.getFormType() == ShortCommonRegisterController.class) {
			String fs = getSourceAsActionId(pojo.getActionId(), pojo
					.getUserId());
			if (null != fs) {
				fromSource = fs;
			} else {
				fromSource = IRegFromSource.S_SCREG;
			}
		} else if (pojo.getFormType() == ShortInviteRegisterController.class) {
		    //5117麻子面馆注册
		    if("205117".equals(pojo.getActionId())){
		        fromSource = IRegFromSource.S_MZMG;
		    }else{
		        fromSource = IRegFromSource.S_SIREG;
		    }
		} else if (pojo.getFormType() == ProfileRegisterController.class) {
			fromSource = IRegFromSource.S_SCREG;
		} else if (pojo.getFormType() == ShortIDCommonRegisterController.class) {
		    //GAME注册
		    if( "205230".equals(pojo.getActionId())){
		        fromSource = IRegFromSource.S_GAME;
		    }else if( "206224".equals(pojo.getActionId()) || "206225".equals(pojo.getActionId())){ //PAGE注册
                fromSource = IRegFromSource.S_PAGE;
            } else if (pojo.getAccType() == IAccountType.XID) {
				fromSource = StringUtils.isNotEmpty(fromSource) ? (fromSource + ":" + IRegFromSource.S_IDREG) : IRegFromSource.S_IDREG;
			} else if (pojo.getAccType() == IAccountType.EMAIL) {
			    fromSource = StringUtils.isNotEmpty(fromSource) ? (fromSource + ":" + IRegFromSource.S_SCREG) : IRegFromSource.S_SCREG;
			}
		} else if (pojo.getFormType() == ShortPureIDCommonRegisterController.class) {
			fromSource = IRegFromSource.S_IDREG;
		}else if (pojo.getFormType() == ShortMobileRegisterController.class){
		    fromSource = IRegFromSource.S_SCREG;
		}

		try {
			if (StringUtils.isNotEmpty(pojo.getIp())
					&& NetbarIpDAO.getInstance().getIp(pojo.getIp()) > 0) {
				fromSource = fromSource + ":" + IRegFromSource.S_SUF_NETBAR;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		if (fromSource.equals(DodoyoResource.getString("S_PAGE",
				RegFromSource.PAGEIDS)) && StringUtils.isNotEmpty(pojo.getPageUuid())) {
			fromSource = fromSource + ":" + pojo.getPageUuid();
		}
		RegFromSourceLogic.save(pojo.getUserId(), fromSource);
	}

	/**
	 * @param fromSource
	 * @return
	 */
	public static List<String> getFromSoureSufix(String fromSource) {
		if (StringUtils.isEmpty(fromSource)) {
			return new ArrayList<String>();
		}
		String[] sufs = fromSource.split(":");
		if (sufs.length <= 1) {
			return new ArrayList<String>();
		}
		List<String> sufList = new ArrayList<String>();
		// start with index 2
		for (int i = 1; i < sufs.length; i++) {
			sufList.add(sufs[i]);
		}
		return sufList;
	}
	
	public static List<String> getFromSoureList(String fromSource) {
		if (StringUtils.isEmpty(fromSource)) {
			return new ArrayList<String>();
		}
		String[] sufs = fromSource.split(":");
		if(sufs == null) return new ArrayList<String>();
		List<String> sufList = new ArrayList<String>();
		
		for(String s : sufs){
			sufList.add(s);
		}
		return sufList;
	}

	private static boolean isNotValue(String key, String value) {
		return DodoyoResource.isNotValue(key, value);
	}

	private static boolean isValue(String key, String value) {
		return !DodoyoResource.isNotValue(key, value);
	}
	
	public static void main(String[] args){
		List<String> l = getFromSoureSufix("ab:");
		System.out.println(l.indexOf(""));
	}
}
