package com.xiaonei.platform.core.sysmsg;

import java.text.MessageFormat;
import java.util.Date;
import java.util.Iterator;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;
import org.apache.struts.util.MessageResources;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
/**
 * 系统错误信息处理机制
 * 该机制依赖于 memorycache,原理sessionid+hostname 作为每一个访问用户错误信息的关键字，同时将该关键字存放于cookie.
 * 每次页面检查 cookie中 "SYSMSGKEK" 的直是否合法如果合法调用 getSysMsg 获取错误信息。
 * 对外接口 
 *   添加错误信息的方法 已经放到BaseXnAction 中
 *   获取错误信息的方法 需要在封装taglib 
 * @author yunlong.bai@opi-corp.com
 *
 */
public class SysMsgManager {
	public static final String sysMsgKey="SYS_MSG_KEY" ;
	
	/**
	 * 消息类型定义：
	 * MSG_TYPE_ERROR: 错误消息，通常显示为红色
	 * MSG_TYPE_WARNING: 警告消息，通常显示为黄色
	 */
	public static final int MSG_TYPE_ERROR = 0;
	public static final int MSG_TYPE_WARNING = 1;
	private static SysMsgManager instance = new SysMsgManager();
	public static SysMsgManager getInstance(){
		return instance ;
	}
	
	/**
	 * 添加错误类型信息
	 * @param request
	 * @param response
	 * @param msg
	 */
	public void adSysMsg(HttpServletRequest request,HttpServletResponse response,String msg){
		adSysMsg(request, response, msg, MSG_TYPE_ERROR);
	}
	/**
	 * 添加消息，消息类型由参数msgType指定
	 * @param request
	 * @param response
	 * @param msg
	 * @param msgType 两种可选消息类型：SysMsgManager.MSG_TYPE_ERROR和SysMsgManager.MSG_TYPE_WARNING
	 */
	public void adSysMsg(HttpServletRequest request,HttpServletResponse response,String msg, int msgType){
        String encodedMsg = encodeSysMsg(msg, msgType);
        //同时存到request对象，应对addErr,addMsg用错的情况，以及兼容以前的addErr
        request.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, encodedMsg);
        
        String key = getSysKey(request);
        if (key == null || "".equals(key)) {
        	return;
        }
        //存到memCache
		MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default).
		add(key, encodedMsg, new Date(MemCacheKeys.minute)) ;	
	}
	
	private String encodeSysMsg(String msg, int msgType) {
		//一个字符的prefix前缀区分是哪种消息类型
		char prefix;
		if (msgType == MSG_TYPE_WARNING) {
			prefix = 'w';
		}
		else {
			prefix = 'e';
		}
		return prefix + msg;
	}

	private String decodeSysMsg(String encodedMsg) {
		return encodedMsg.substring(1);
	}
	
	private int getMsgType(String encodedMsg) {
		char prefix = encodedMsg.charAt(0);
		if (prefix == 'w') {
			return MSG_TYPE_WARNING;
		}
		else {
			return MSG_TYPE_ERROR;
		}
	}
	
	private static String getSysKey(HttpServletRequest request){
		User host = (User) request.getAttribute("visiter");
		if (host == null) {
			return null;
		}
		//System.err.println("getSysKey: request = " + request + "; class = " + request.getClass());
		//System.err.println("getSysKey: host="+host);
		int uid = host.getId();
		//System.err.println("uid="+uid);
		return sysMsgKey + uid;
	}
	
	/**
	 * 获取错误信息
	 * @param request
	 * @param response
	 * @return
	 */
	public SysMsg getSysMsg(HttpServletRequest request,HttpServletResponse response) {
		try{
			String encodedMsg = null;
			//System.err.println("----------- in getSysMsg");
			
			Object reqMsgObj = request.getAttribute(org.apache.struts.Globals.MESSAGE_KEY);
			if (reqMsgObj instanceof String) {
				//通过新的SysMsgManager.adSysMsg存放的消息
				encodedMsg = (String) reqMsgObj;
				//System.err.println("----------- msg string from request (new): " + encodedMsg);
				//别忘了删除存在memCache中的消息
				String key = getSysKey(request);
				if (key == null || "".equals(key)) {
					return null ;
				}
				MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default) ;
				mem.delete(key) ;
			}
			else if (reqMsgObj instanceof ActionMessages) {
				//通过原来的BaseXnAction.addMsg存放的消息，是为了向前兼容；将来要去掉这个处理(即原来的addErr应该改写)
				ActionMessages ams = (ActionMessages) reqMsgObj;
				String msgStr = getActionMessagesString(ams);
				encodedMsg = encodeSysMsg(msgStr, MSG_TYPE_WARNING);
				//System.err.println("----------- msg string from request (old, warning): " + encodedMsg);
			}
			else {
				//通过原来的BaseXnAction.addErr存放的消息，是为了向前兼容；将来要去掉这个处理
				reqMsgObj = request.getAttribute(org.apache.struts.Globals.ERROR_KEY);
				if (reqMsgObj instanceof ActionErrors) {
					//处理ActionErrors，暂时放在这，稍后并入ActionMessages的处理；目前只有EmailAction用到了ActionErrors
					ActionMessages ams = (ActionMessages) reqMsgObj;
					String msgStr = getActionMessagesFormatString(request, ams);
					encodedMsg = encodeSysMsg(msgStr, MSG_TYPE_ERROR);
					//System.err.println("----------- msg string from request (ActionErrors: old, error): " + encodedMsg);
				}
				else if (reqMsgObj instanceof ActionMessages) {
					ActionMessages ams = (ActionMessages) reqMsgObj;
					String msgStr = getActionMessagesString(ams);
					encodedMsg = encodeSysMsg(msgStr, MSG_TYPE_ERROR);
					//System.err.println("----------- msg string from request (ActionMessages: old, error): " + encodedMsg);
				}
			}

			if (encodedMsg == null) {
				//在request对象里没有取到，到memCache里取
				String key = getSysKey(request);
				if (key == null || "".equals(key)) {
					return null ;
				}
				MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default) ;
				encodedMsg = (String) mem.get(key) ;
				mem.delete(key) ;
				//System.err.println("----------- msg string from memcache: " + encodedMsg);
			}

			if (encodedMsg == null) {
				//所有可能的地方都没有找到，返回空值
				return null;
			}

			SysMsg msg = new SysMsg();
			msg.setMsgType(getMsgType(encodedMsg));
			msg.setMsg(decodeSysMsg(encodedMsg));
			return msg ;
		}catch(Exception e){
			e.printStackTrace() ;
		}
		return null ;
	}

	private String getActionMessagesString(ActionMessages ams) {
		StringBuilder msgStrBuilder = new StringBuilder();
		//for (Iterator i = ams.get(ActionMessages.GLOBAL_MESSAGE); i.hasNext(); ) {
		for (Iterator i = ams.get(); i.hasNext(); ) {
			ActionMessage am = (ActionMessage) i.next();
			String msgStr = (String) am.getValues()[0];
			if (msgStr != null) {
				msgStrBuilder.append(msgStr);
			}
		}
		return msgStrBuilder.toString();
	}

	private String getActionMessagesFormatString(HttpServletRequest request, ActionMessages ams) {
		StringBuilder msgStrBuilder = new StringBuilder();
		for (Iterator i = ams.get(); i.hasNext(); ) {
			ActionMessage am = (ActionMessage) i.next();
			//System.err.println("--------- am key = " + am.getKey());
			MessageResources messages = ((MessageResources) request.getAttribute(org.apache.struts.Globals.MESSAGES_KEY));
			if (messages != null) {
				String msgPattern = messages.getMessage(am.getKey());
				//System.err.println("--------- resources msgPattern = " + msgPattern);
				MessageFormat format = new MessageFormat(msgPattern);
				String formattedMsg = format.format(am.getValues());
				if (formattedMsg != null) {
					msgStrBuilder.append(formattedMsg);
				}
				//System.err.println("--------- resources formattedMsg = " + formattedMsg);
			}
			else {
				String msgStr = (String) am.getValues()[0];
				if (msgStr != null) {
					msgStrBuilder.append(msgStr);
				}
			}
		}
		//System.err.println("--------- resources msgStrBuilder = " + msgStrBuilder);
		return msgStrBuilder.toString();
	}

}
