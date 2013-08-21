package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.dao.InviteUrlParasDAO;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.RegExtraMsgDAO;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.model.InviteUrlParas;
import com.xiaonei.reg.register.model.RegExtraMsg;
import com.xiaonei.reg.register.pojo.RegisterPojo;

/**
 * 保存注册的额外信息
 * @author wei.cheng@opi-corp.com
 * @date
 */
public class RegExtraMsgLogic {
    
    private static final String MEM_CACHE_REG_EXTRA_MSG = "mem_cache_reg_extra_msg_";
    
    private static RegExtraMsgLogic _instance = new RegExtraMsgLogic();

    public static RegExtraMsgLogic getInstance() {
        return _instance;
    }

    private RegExtraMsgLogic() {
        super();
    }
    
    /**
     * 保存用户的额外信息，同时保存到memcache中一份
     * @param user
     * @param msgMap
     */
    public void saveMsg(User user, Map<String,RegExtraMsg> msgMap){
        if(msgMap == null || msgMap.size() == 0) return ;
        
        for (java.util.Map.Entry<String, RegExtraMsg> b : msgMap.entrySet()) {           
            final RegExtraMsg v = (RegExtraMsg)b.getValue();
            Thread aaa = new Thread() {
                @Override
                public void run() {
                    try {
                        RegExtraMsgDAO.getInstance().save(v);
                    } catch (SQLException e) {
                    }
                }
            };
            RunnableTaskExecutor.getInstance().runTask(aaa);
        }
        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
        String memKey = MEM_CACHE_REG_EXTRA_MSG + user.getId();
        mem.set(memKey, msgMap, (int) (MemCacheKeys.day * 1));
    }
    
    /**
     * 保存单条信息
     * @param user
     * @param input_key
     * @param input_value
     */
    public void saveMsg(User user, String input_key, String input_value) {
        if(user == null){
            return ;
        }
        //保存注册信息到数据
        RegExtraMsg msg = new RegExtraMsg();
        msg.setUid(user.getId());
        msg.setInput_key(input_key);
        msg.setInput_value(input_value);
        try {
            RegExtraMsgDAO.getInstance().save(msg);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    
    public void asyncUpdateMsgFlagByUidAndInputKey(User host,String key,int flag){
    	if(host == null){
    		GuideLogger.printLog("host null!"+" "+key+" "+flag,GuideLogger.WARN);
    		return;
    	}
    	final RegExtraMsg msg = new RegExtraMsg();
    	msg.setUid(host.getId());
    	msg.setInput_key(key);
    	msg.setReg_flag(flag);
    	GuideLogger.printLog(" host:"+host.getId()+" key:"+key+" flag:"+flag,GuideLogger.WARN);
    	MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
    	Map<String, RegExtraMsg> msgMap = new HashMap<String, RegExtraMsg>();
    	msgMap.put(msg.getInput_key(), msg);
    	String memKey = MEM_CACHE_REG_EXTRA_MSG + host.getId();
    	mem.set(memKey, msgMap, (int) (MemCacheKeys.minute * 15));
    	try {
    		Thread save = new Thread() {
                @Override
                public void run() {
                    try {
                        RegExtraMsgDAO.getInstance().updateMsgFlagByUidAndInpuKey(msg);
                    } catch (SQLException e) {
                    	// TODO Auto-generated catch block
                    	e.printStackTrace();
                    }
                }
            };
            RunnableTaskExecutor.getInstance().runTask(save);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    @SuppressWarnings("unchecked")
	public Map<String,RegExtraMsg> loadExtraMsgMap(User host){
    	Map<String,RegExtraMsg> map = null;
    	MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
        String memKey = MEM_CACHE_REG_EXTRA_MSG + host.getId();
        Object obj = mem.get(memKey);
        if(obj != null){
        	map = (Map<String,RegExtraMsg>)obj;
        }
        else{
        	RegExtraMsg msg = new RegExtraMsg();
        	msg.setUid(host.getId());
        	
        	try {
				List<RegExtraMsg> list = RegExtraMsgDAO.getInstance().getMsgInfoByUid(msg);
				if(list != null){
					map = new HashMap<String, RegExtraMsg>();
					for (RegExtraMsg regExtraMsg : list) {
						map.put(regExtraMsg.getInput_key(), regExtraMsg);
					}
					mem.set(memKey, map, (int) (MemCacheKeys.day * 1));
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
        }
    	return map;
    }
    /**
     * 根据注册请求获取需要保存的额外信息列表
     * @param regPojo
     * @param oceUser
     * @param request
     * @return
     */
    public Map<String,RegExtraMsg> getMsgList(RegisterPojo regPojo, User oceUser, HttpServletRequest request) {
    	Map<String,RegExtraMsg> lst = new HashMap<String, RegExtraMsg>();
        //注册填写推荐人的情况
        if(request != null && "206221".equals(regPojo.getActionId()) && StringUtils.isNotEmpty(request.getParameter("recommend"))){
            RegExtraMsg msg = new RegExtraMsg();
            msg.setUid(oceUser.getId());
            msg.setInput_key(IRegExtraMsgLogic.key_reg_recommond);
            msg.setInput_value(request.getParameter("recommend") != null ? request.getParameter("recommend") : "");
            lst.put(IRegExtraMsgLogic.key_reg_recommond, msg);
        }
        //connect邀请的情况.只有appId，但是没有uuid的情况
        if(request != null && StringUtils.isNotEmpty(request.getParameter("appId")) 
                && StringUtils.isEmpty(request.getParameter("uuid"))){
            RegExtraMsg msg = new RegExtraMsg();
            msg.setUid(oceUser.getId());
            msg.setInput_key(IRegExtraMsgLogic.key_reg_app_connect);
            msg.setInput_value(request.getParameter("appId") != null ? request.getParameter("appId") : "");
            lst.put(IRegExtraMsgLogic.key_reg_app_connect, msg);
        }
        //站外邀请的情况。有uuid同时反解析后appid存在
        String uuid = request == null? "": request.getParameter("uuid");
        if(StringUtils.isNotEmpty(uuid)){
            InviteUrlParas paras = null;
            try {
                paras = InviteUrlParasDAO.getInstance().getParas(uuid);
            } catch (SQLException e) {
                e.printStackTrace();
            }
            if(paras != null && paras.getParas() != null 
                    && StringUtils.isNotEmpty(paras.getParas().get("appId"))){
                RegExtraMsg msg = new RegExtraMsg();
                msg.setUid(oceUser.getId());
                msg.setInput_key(IRegExtraMsgLogic.key_reg_app_id);
                msg.setInput_value(paras.getParas().get("appId"));
                lst.put(IRegExtraMsgLogic.key_reg_app_id, msg);
            }
        }
        return lst;
    }  
    
}
