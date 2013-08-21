package com.xiaonei.platform.core.utility;

import java.util.Date;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;

public class AntiSpam {
	private final int postTribeProhibTime = 10 * 1000; //以毫秒计，禁发帖间隔时间
	private final int tribePostSumm = 100; //发帖内容的前面截留的内容数
	
	//要加随机码antispam的业务
	public final static String AS_BIZ_SAVE_HOT_FRIEND = "hotfriend_temp_sys_code_";
	public final static String AS_BIZ_UPLOAD_HEAD_IMG = "uploadh_temp_sys_code_";
	public final static String AS_BIZ_SAVE_NEW_BLOG = "newblog_temp_sys_code_";
	public final static String AS_BIZ_SAVE_EDIT_BLOG = "editblog_temp_sys_code_";
	public final static String AS_BIZ_COMMENT_BLOG = "commentblog_temp_sys_code_";
	public final static String AS_BIZ_COMMENT_PHOTO = "commentphoto_temp_sys_code_";
	public final static String AS_BIZ_GOSSIP = "gossip_temp_sys_code_";
	public final static String AS_BIZ_DOING_STATUS = "doingstatus_temp_sys_code_";
	public final static String AS_BIZ_SHARE = "share_temp_sys_code_";
	public final static String AS_BIZ_TRIBE_POST = "tribepost_temp_sys_code_";
	public final static String AS_BIZ_TRIBE_REPLY = "tribereply_temp_sys_code_";
	public final static String AS_BIZ_SCHOOLCLASS_POST = "classpost_temp_sys_code_";
	public final static String AS_BIZ_SCHOOLCLASS_REPLY = "classreply_temp_sys_code_";
		
	private static AntiSpam instance = new AntiSpam();

	public static AntiSpam getInstance() {
		if (instance == null) {
			synchronized (AntiSpam.class) {
				if (instance == null)
					instance = new AntiSpam();
			}
		}
		return instance;
	}	
	
	/**
	 * 功能：假如发帖间隔小于10秒就禁止发帖10秒
	 * @param host
	 * @param eventDate
	 * @return
	 */
	public boolean canPostTribeByTime(User host, Date eventDate, HttpSession session, String spamSource)
	{   
		if ( session.getAttribute("eventPostDate" + spamSource) == null )
        {	
        	session.setAttribute("eventPostDate" + spamSource, new Date().getTime());       	        	
        	return true;
        }
        
        if ( (eventDate.getTime() - (Long)(session.getAttribute("eventPostDate" + spamSource))) < postTribeProhibTime)
        	return false;
        
        session.setAttribute("eventPostDate" + spamSource, eventDate.getTime());
        
        return true;		
	}
	
	/**
	 * 功能：假如发的内容跟上次的摘要相同就禁止发帖10秒
	 * @param host
	 * @param eventDate
	 * @return
	 */
	public boolean canPostTribeByContent(User host, String content, HttpSession session, String spamSource)
	{		
		String s = content.length() > tribePostSumm ? content.substring(0, tribePostSumm) : content; //内容截留
         
		if ( session.getAttribute("eventPostContent" + spamSource) == null )
        {	
        	session.setAttribute("eventPostContent" + spamSource, s);       	        	
        	return true;
        }
        
		if ( s.equals((String)session.getAttribute("eventPostContent" + spamSource)))
        	return false;
        
        session.setAttribute("eventPostContent" + spamSource, s);
        
        return true;	        
	}
	
	/**
	 * 为post提交产生随机码
	 * 这个策略过段时间需要变一下
	 * @param hostId
	 * @return
	 */
	private String createBizRandCodeForPost(final int hostId)
	{
		//2008.03.24 策略
		return MathUtils.MD5( Long.toString(hostId + System.currentTimeMillis()));
	}
	
	/**
	 * 根据用户ID,业务类型和给定的缓存的过期时间,在memcache中设定一个随机码,并返回这个随机码
	 * @param hostId
	 * @param antispamBizName
	 * @param interval 单位为分钟,既输入的时间换算为分钟,例如:如果设过期时间为2小时,则应该输入 2 * 60 
	 * @return String
	 */
	public String getBizRandCodeForPost(final int hostId, final String antispamBizName,final int interval){
		String s_myth = createBizRandCodeForPost(hostId);	
		
		try
		{
			MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_rand_code) ;
			manager.set(antispamBizName + hostId, s_myth, new Date(MemCacheKeys.minute*interval));			
		}catch(Exception e){e.printStackTrace();}
		return s_myth;
	}
	
	/**
	 * 根据用户ID,业务类型,在memcache中设定一个默认过期时间的随机码,并存到request对象中，默认的时间为１０分钟
	 * @param hostId
	 * @param antispamBizName
	 * @param request
	 */
	public void setBizRandCodeForPost(final int hostId, final String antispamBizName, HttpServletRequest request)
	{
		request.setAttribute("temp_sys_code", getBizRandCodeForPost(hostId,antispamBizName,10));		
	}
	
	/**
	 * 根据用户ID,业务类型和给定的缓存的过期时间,在memcache中设定一个随机码,并存到request对象中
	 * @param hostId
	 * @param antispamBizName
	 * @param interval　单位为分钟,既输入的时间换算为分钟,例如:如果设过期时间为2小时,则应该输入 2 * 60
	 * @param request
	 */
	public void setBizRandCodeForPost(final int hostId, final String antispamBizName,final int interval, HttpServletRequest request)
	{
		request.setAttribute("temp_sys_code", getBizRandCodeForPost(hostId,antispamBizName,interval));		
	}
	
	public boolean handleBizRandCodeForPost(final int hostId, final String antispamBizName, HttpServletRequest request, HttpServletResponse response)
	{
		MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_rand_code) ;
		String s_myth = (String)manager.get(antispamBizName + hostId);		
	
		if (s_myth == null || !s_myth.equals(request.getParameter("tsc")))
		{
			return false;
		} else {
			manager.delete(antispamBizName + hostId);
		}
		return true;
	}
}
