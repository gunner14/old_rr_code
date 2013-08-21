package com.xiaonei.reg.usertrace.logic;

import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.usertrace.dao.RegVisitCookieLogDAO;
import com.xiaonei.reg.usertrace.dao.RegVisitLogViewDAO;
import com.xiaonei.reg.usertrace.dao.RegVisitOperateDAO;
import com.xiaonei.reg.usertrace.dao.RegVisitOperateSubDAO;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.reg.usertrace.model.RegVisitCookieLogView;
import com.xiaonei.reg.usertrace.model.RegVisitLogView;
import com.xiaonei.reg.usertrace.model.RegVisitOperate;
import com.xiaonei.reg.usertrace.model.RegVisitOperateSub;
import com.xiaonei.reg.usertrace.q.Queue;
import com.xiaonei.reg.usertrace.q.QueueCookie;
import com.xiaonei.reg.usertrace.q.QueueOpr;
import com.xiaonei.reg.usertrace.q.QueueOprSub;
import com.xiaonei.reg.usertrace.q.UTUtils;

public class UserTraceLogic {
    
	private UserTraceLogic() {
		super();
	}

	private static UserTraceLogic _instance = new UserTraceLogic();

	public static UserTraceLogic getInstance() {
		return _instance;
	}

	/**
	 * 记录用户访问,第一步打开连接或者第一次进入注册页面
	 * 
	 * @param request
	 * @param actionid
	 * @param s1
	 * @param s2
	 * @param s3
	 * @param s4
	 * @return
	 */
	public int insertVisitHisFirst(HttpServletRequest request,
			String actionIdStr, String s1, String s2, String s3, String s4,
			int inviter_id) {
		int rt = 0;

		try {
			// 填充参数对象
			final RegVisitLogView logView = new RegVisitLogView();
			String xnSessId = null;
			if(request != null){
			    xnSessId = CookieManager.getInstance().getCookie(request,
			        "XNESSESSIONID");
			}				
			String remoteIp = getUserIp(request);
			logView.setIp(remoteIp == null ? "" : remoteIp);
			logView.setXnSessId(xnSessId == null ? "" : xnSessId);
			int actionid = 0;
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {
				}
			}
			logView.setActionid(actionid);
			String referer = request == null ? "" : request
					.getHeader("Referer");
			String tmp = (referer != null && referer.length() > 200) ? referer
					.substring(0, 200) : referer;
			logView.setReferer(tmp != null ? tmp : "");
			logView.setS1(s1 != null ? s1 : "");
			logView.setS2(s2 != null ? s2 : "");
			logView.setS3(s3 != null ? s3 : "");
			logView.setS4(s4 != null ? s4 : "");

			String regUrl = request != null && request.getRequestURL() != null ? request
					.getRequestURL().toString()
					: null;
			if (regUrl != null && regUrl.indexOf("http://") > -1) {
				String temp = regUrl.substring(7);
				if (temp != null && temp.indexOf("/") > -1) {
					regUrl = temp.substring(0, temp.indexOf("/"));
				}
			}
			logView.setDomainName(regUrl == null ? "" : regUrl);
			logView.setInviterId(inviter_id);
			// 插入
			RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).debug(logView.toString());
			// RegVisitLogViewDAO.getInstance().insertLogView(logView);
			Queue qu = Queue.getInstance();
			qu.push(logView);
		} catch (Exception e) {
		}

		return rt;
	}
	
	/**
	 * 有来源的情况
	 * 
	 * @param request
	 * @param host
	 * @param email
	 * @param actionIdStr
	 * @param stage
	 * @param s1
	 * @param s2
	 * @param s3
	 * @param s4
	 * @param inviter_id
	 * @param isSub
	 */
	public void insertRegHis(HttpServletRequest request, User host,
			String email, String actionIdStr, int stage, String s1, String s2,
			String s3, String s4, int inviter_id, boolean isSub) {

		if (isSub) {
			insertRegHis(request, host, email, actionIdStr, stage, s1, s2, s3,
					s4, inviter_id);
			if (s1 != null && !"".equals(s1)) {
				// 有来源的情况
				insertRegActSub(request, host, email, actionIdStr, stage, s1,
						s2, s3, s4);
			}
		}
	}

	/**
	 * 给管理员发邮件自动激活的情况
	 * 
	 * @param userId
	 * @param email
	 * @param stage
	 * @param s1
	 * @param s2
	 * @param s3
	 * @param s4
	 * @return
	 */
	public int insertRegHisForAdmin(int userId, String email, int stage,
			String s1, String s2, String s3, String s4) {
		int rt = 0;

		try {
			// 填充参数对象
			final RegVisitOperate regOpr = new RegVisitOperate();
			regOpr.setActionid(RegOprDefine.SEND_MAIL_ACTIVE);
			regOpr.setIp("");
			regOpr.setXnSessId("");
			regOpr.setEmail(email != null ? email : "");
			regOpr.setUserid(userId);
			regOpr.setStage(stage);
			regOpr.setS1(s1 != null ? s1 : "");
			regOpr.setS2(s2 != null ? s2 : "");
			regOpr.setS3(s3 != null ? s3 : "");
			regOpr.setS4(s4 != null ? s4 : "");
			regOpr.setDomainName("");
			regOpr.setInviterId(0);

			// 插入
			//RegVisitOperateDAO.getInstance().insertOprLog(regOpr);
			QueueOpr qu = QueueOpr.getInstance();
            qu.push(regOpr);
		} catch (Throwable e) {
			System.err.println("UserTraceLogic.insertRegHisForAdmin err");
		}

		return rt;
	}

	/**
	 * 记录用户访问,在注册页面上的行为记录，如果是一步注册则记录一条记录，如果是两步注册则产生两条不同的记录 取不到值的情况设为默认值
	 * 激活存在同一个表中，激活的时候email从用户资料中获取
	 * 
	 * @param request
	 * @param actionid
	 * @param s1
	 * @param s2
	 * @param s3
	 * @param s4
	 * @return
	 */
	public int insertRegHis(HttpServletRequest request, User host,
			String email, String actionIdStr, int stage, String s1, String s2,
			String s3, String s4, int inviter_id) {
		int rt = 0;

		try {
			// 填充参数对象
			RegVisitOperate regOpr = new RegVisitOperate();
			String xnSessId = null;
			if(request != null){
                xnSessId = CookieManager.getInstance().getCookie(request,
                    "XNESSESSIONID");
            }
			String remoteIp = getUserIp(request);
			int actionid = 0;
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {

				}
			}
			regOpr.setActionid(actionid);
			regOpr.setIp(remoteIp == null ? "" : remoteIp);
			regOpr.setXnSessId(xnSessId == null ? "" : xnSessId);
			regOpr.setEmail(email != null ? email : "");
			if (host != null) {
				regOpr.setUserid(host.getId());
			} else {
				regOpr.setUserid(0);
			}
			regOpr.setStage(stage);
			regOpr.setS1(s1 != null ? s1 : "");
			regOpr.setS2(s2 != null ? s2 : "");
			regOpr.setS3(s3 != null ? s3 : "");
			regOpr.setS4(s4 != null ? s4 : "");

			String regUrl = request != null && request.getRequestURL() != null ? request
					.getRequestURL().toString()
					: null;
			if (regUrl != null && regUrl.indexOf("http://") > -1) {
				String temp = regUrl.substring(7);
				if (temp != null && temp.indexOf("/") > -1) {
					regUrl = temp.substring(0, temp.indexOf("/"));
				}
			}
			regOpr.setDomainName(regUrl == null ? "" : regUrl);
			regOpr.setInviterId(inviter_id);

			// 插入
			RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).debug(regOpr.toString());
			//rt = RegVisitOperateDAO.getInstance().insertOprLog(regOpr);
			QueueOpr qu = QueueOpr.getInstance();
			qu.push(regOpr);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegHis err");
		}

		return rt;
	}

	/**
	 * 用户访问来源临时表
	 * 
	 * @param request
	 * @param host
	 * @param email
	 * @param actionIdStr
	 * @param stage
	 * @param s1
	 * @param s2
	 * @param s3
	 * @param s4
	 * @param inviter_id
	 * @return
	 */
	public int insertRegActSub(HttpServletRequest request, User host,
			String email, String actionIdStr, int stage, String s1, String s2,
			String s3, String s4) {
		int rt = 0;

		try {
			// 填充参数对象
			RegVisitOperateSub regOprSub = new RegVisitOperateSub();
			int actionid = 0;
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {

				}
			}
			regOprSub.setActionid(actionid);
			regOprSub.setEmail(email != null ? email : "");
			if (host != null) {
				regOprSub.setUserid(host.getId());
			} else {
				regOprSub.setUserid(0);
			}
			regOprSub.setS1(s1 != null ? s1 : "");
			regOprSub.setS2(s2 != null ? s2 : "");
			regOprSub.setS3(s3 != null ? s3 : "");
			regOprSub.setS4(s4 != null ? s4 : "");

			String regUrl = request != null && request.getRequestURL() != null ? request
					.getRequestURL().toString()
					: null;
			if (regUrl != null && regUrl.indexOf("http://") > -1) {
				String temp = regUrl.substring(7);
				if (temp != null && temp.indexOf("/") > -1) {
					regUrl = temp.substring(0, temp.indexOf("/"));
				}
			}
			// 插入
			//rt = RegVisitOperateSubDAO.getInstance().insertOprSubLog(regOprSub,	regUrl);
			RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).debug(regOprSub.toString());
			QueueOprSub qu = QueueOprSub.getInstance();
			qu.push(regOprSub, regUrl);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegActSub err");
		}
		return rt;
	}

	/**
	 * batch insert into table `act1`
	 * @param infos
	 * @return
	 */
    public int saveBatchVisitHisFirst(final HashMap<Integer, RegVisitLogView> infos) {
        Runnable save = new Runnable()
        {
            public void run()
            {
                try {
                    int[] result = RegVisitLogViewDAO.getInstance().batchSave(infos);
                    StringBuilder ss = new StringBuilder("batch save act1 result:");
                    for(int r : result){
                        ss.append(",").append(r);
                    }
                    RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).info(ss.toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }                
            }
        };
        RunnableTaskExecutor.getInstance().runTask(save);
        return 0;
    }	
	
    /**
     * batch insert into table `act2`
     * @param infos
     * @return
     */
    public int saveBatchRegHis(final HashMap<Integer, RegVisitOperate> infos) {
        Runnable save = new Runnable()
        {
            public void run()
            {
                try {
                    int[] result = RegVisitOperateDAO.getInstance().batchSave(infos);
                    StringBuilder ss = new StringBuilder("batch save act2 result:");
                    for(int r : result){
                        ss.append(",").append(r);
                    }
                    RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).info(ss.toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }                
            }
        };
        RunnableTaskExecutor.getInstance().runTask(save);
        return 0;
    }
    
    /**
     * batch insert into table `act_subs`
     * @param infos
     * @param domain
     * @return
     */
    public int saveBatchRegActSub(final HashMap<Integer, RegVisitOperateSub> infos, final String domain) {
        Runnable save = new Runnable()
        {
            public void run()
            {
                try {
                    int[] result = RegVisitOperateSubDAO.getInstance().batchSave(infos, domain);
                    StringBuilder ss = new StringBuilder("batch save act_subs result:");
                    for(int r : result){
                        ss.append(",").append(r);
                    }
                    RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).info(ss.toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }                
            }
        };
        RunnableTaskExecutor.getInstance().runTask(save);
        return 0;
    }
    
    /**
     * batch insert into table `act1_cookie`
     * @param infos
     * @return
     */
    public int saveBatchRegActCookie(final HashMap<Integer, RegVisitCookieLogView> infos) {
        Runnable save = new Runnable()
        {
            public void run()
            {
                try {
                    int[] result = RegVisitCookieLogDAO.getInstance().batchSave(infos);
                    StringBuilder ss = new StringBuilder("batch save act1_cookie result:");
                    for(int r : result){
                        ss.append(",").append(r);
                    }
                    RegLogger.getLoggerWithType(UserTraceLogic.class.getSimpleName(), UTUtils.authorEmail).info(ss.toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        RunnableTaskExecutor.getInstance().runTask(save);
        return 0;
    }
	
	/**
	 * 获取用户的ip信息
	 * 
	 * @param request
	 * @return
	 */
	private String getUserIp(HttpServletRequest request) {
		if (request == null)
			return "";
		String ip = request.getHeader("x-forwarded-for");
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getHeader("Proxy-Client-IP");
		}
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getHeader("WL-Proxy-Client-IP");
		}
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getRemoteAddr();
		}
		if (ip != null && ip.indexOf(",") > -1) {
			ip = ip.substring(0, ip.indexOf(","));
		}
		if (ip != null && ip.length() > 20) {
			ip = ip.substring(0, 20);
		}
		return ip;
	}

	/***************************************************************************************/
	/*** 新统计方法 ***/
	/***************************************************************************************/

	/**
	 * 用户访问来源临时表
	 * 
	 * @param regVisit
	 * @return
	 */
	public int insertRegActSub(ConcurrentHashMap<String, Object> regVisit) {
		int rt = 0;

		try {
			// 填充参数对象
			RegVisitOperateSub regOprSub = new RegVisitOperateSub();
			int actionid = 0;
			String actionIdStr = (String) regVisit
					.get(RegOprDefine.ACTION_ID_KEY);
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {

				}
			}
			regOprSub.setActionid(actionid);
			String email = (String) regVisit.get(RegOprDefine.USER_EMAIL_KEY);
			regOprSub.setEmail(email != null ? email : "");
			Integer userId = (Integer) regVisit.get(RegOprDefine.USER_ID_KEY);
			if (userId != null) {
				regOprSub.setUserid(userId);
			} else {
				regOprSub.setUserid(0);
			}
			String s1 = (String) regVisit.get(RegOprDefine.S1_KEY);
			String s2 = (String) regVisit.get(RegOprDefine.S2_KEY);
			String s3 = (String) regVisit.get(RegOprDefine.S3_KEY);
			String s4 = (String) regVisit.get(RegOprDefine.S4_KEY);
			regOprSub.setS1(s1 != null ? s1 : "");
			regOprSub.setS2(s2 != null ? s2 : "");
			regOprSub.setS3(s3 != null ? s3 : "");
			regOprSub.setS4(s4 != null ? s4 : "");

			// 插入
			// rt = RegVisitOperateSubDAO.getInstance().insertOprSubLog(regOprSub,	(String) regVisit.get(RegOprDefine.DOMAIN_NAME_KEY));
			QueueOprSub qu = QueueOprSub.getInstance();
            qu.push(regOprSub, (String) regVisit.get(RegOprDefine.DOMAIN_NAME_KEY));
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegActSub err");
		}

		return rt;
	}

	/**
	 * 记录用户访问,在注册页面上的行为记录，如果是一步注册则记录一条记录，如果是两步注册则产生两条不同的记录 取不到值的情况设为默认值
	 * 激活存在同一个表中，激活的时候email从用户资料中获取
	 * 
	 * @param regVisit
	 * @return
	 */
	public int insertRegHis(ConcurrentHashMap<String, Object> regVisit) {
		int rt = 0;

		try {
			// 填充参数对象
			RegVisitOperate regOpr = new RegVisitOperate();
			String xnSessId = (String) regVisit.get(RegOprDefine.XNSESS_ID_KEY);
			String remoteIp = (String) regVisit.get(RegOprDefine.IP_KEY);
			int actionid = 0;
			String actionIdStr = (String) regVisit
					.get(RegOprDefine.ACTION_ID_KEY);
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {

				}
			}
			regOpr.setActionid(actionid);
			regOpr.setIp(remoteIp == null ? "" : remoteIp);
			regOpr.setXnSessId(xnSessId == null ? "" : xnSessId);
			String email = (String) regVisit.get(RegOprDefine.USER_EMAIL_KEY);
			regOpr.setEmail(email != null ? email : "");
			Integer userId = (Integer) regVisit.get(RegOprDefine.USER_ID_KEY);
			if (userId != null) {
				regOpr.setUserid(userId);
			} else {
				regOpr.setUserid(0);
			}
			Object usk = regVisit.get(RegOprDefine.USER_STAGE_KEY);
			if (null == usk) {
				return rt;
			}
			int stage = (Integer) usk;
			regOpr.setStage(stage);
			String s1 = (String) regVisit.get(RegOprDefine.S1_KEY);
			String s2 = (String) regVisit.get(RegOprDefine.S2_KEY);
			String s3 = (String) regVisit.get(RegOprDefine.S3_KEY);
			String s4 = (String) regVisit.get(RegOprDefine.S4_KEY);
			regOpr.setS1(s1 != null ? s1 : "");
			regOpr.setS2(s2 != null ? s2 : "");
			regOpr.setS3(s3 != null ? s3 : "");
			regOpr.setS4(s4 != null ? s4 : "");
			String regUrl = (String) regVisit.get(RegOprDefine.DOMAIN_NAME_KEY);
			regOpr.setDomainName(regUrl == null ? "" : regUrl);
			Object iik = regVisit.get(RegOprDefine.INVITER_ID_KEY);
			if (null == iik)
				return rt;
			int inviter_id = (Integer) iik;
			regOpr.setInviterId(inviter_id);

			// 插入
			//rt = RegVisitOperateDAO.getInstance().insertOprLog(regOpr);
			QueueOpr qu = QueueOpr.getInstance();
            qu.push(regOpr);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegHis err");
		}

		return rt;
	}

	/**
	 * 有来源的情况
	 * 
	 * @param regVisit
	 * @param isSub
	 */
	public void insertRegHis(ConcurrentHashMap<String, Object> regVisit,
			boolean isSub) {

		if (isSub) {
			insertRegHis(regVisit);
			String s1 = (String) regVisit.get(RegOprDefine.S1_KEY);
			if (s1 != null && !"".equals(s1)) {
				// 有来源的情况
				insertRegActSub(regVisit);
			}
		}
	}

	/**
	 * 记录用户访问,第一步打开连接或者第一次进入注册页面
	 * 
	 * @param regVisit
	 * @return int
	 */
	public int insertVisitHisFirst(ConcurrentHashMap<String, Object> regVisit) {
		int rt = 0;

		try {
			// 填充参数对象
			RegVisitLogView logView = new RegVisitLogView();
			String xnSessId = (String) regVisit.get(RegOprDefine.XNSESS_ID_KEY);
			String remoteIp = (String) regVisit.get(RegOprDefine.IP_KEY);
			logView.setIp(remoteIp == null ? "" : remoteIp);
			logView.setXnSessId(xnSessId == null ? "" : xnSessId);
			String actionIdStr = (String) regVisit
					.get(RegOprDefine.ACTION_ID_KEY);
			int actionid = 0;
			if (actionIdStr != null && !"".equals(actionIdStr)) {
				try {
					actionid = Integer.parseInt(actionIdStr);
				} catch (Exception ex) {

				}
			}
			logView.setActionid(actionid);
			String referer = (String) regVisit
					.get(RegOprDefine.DOMAIN_NAME_KEY);
			String tmp = (referer != null && referer.length() > 200) ? referer
					.substring(0, 200) : referer;
			logView.setReferer(tmp != null ? tmp : "");
			String s1 = (String) regVisit.get(RegOprDefine.S1_KEY);
			String s2 = (String) regVisit.get(RegOprDefine.S2_KEY);
			String s3 = (String) regVisit.get(RegOprDefine.S3_KEY);
			String s4 = (String) regVisit.get(RegOprDefine.S4_KEY);
			logView.setS1(s1 != null ? s1 : "");
			logView.setS2(s2 != null ? s2 : "");
			logView.setS3(s3 != null ? s3 : "");
			logView.setS4(s4 != null ? s4 : "");

			String regUrl = (String) regVisit
					.get(RegOprDefine.REFERER_NAME_KEY);
			logView.setDomainName(regUrl == null ? "" : regUrl);
			Object iik = regVisit.get(RegOprDefine.INVITER_ID_KEY);
			if (null == iik) {
				return rt;
			}
			int inviter_id = (Integer) iik;
			logView.setInviterId(inviter_id);
			// 插入
			//rt = RegVisitLogViewDAO.getInstance().insertLogView(logView);
			Queue qu = Queue.getInstance();
            qu.push(logView);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertVisitHisFirst err e:"+e.toString());
		}

		return rt;
	}

	/***************************************************************************************/
	/*** 新统计方法 ***/
	/*** 2009.3.18 增加传递dao的方法 ***/
	/***************************************************************************************/

	/**
	 * 用户访问来源临时表
	 * 
	 * @param regVisit
	 * @return
	 */
	public int insertRegActSub(RegVisitOperateSub regOprSub, String domainName) {
		int rt = 0;

		try {
			// 插入
			//rt = RegVisitOperateSubDAO.getInstance().insertOprSubLog(regOprSub, domainName);
		    QueueOprSub qu = QueueOprSub.getInstance();
            qu.push(regOprSub, domainName);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegActSub err");
		}

		return rt;
	}

	/**
	 * 记录用户访问,在注册页面上的行为记录，如果是一步注册则记录一条记录，如果是两步注册则产生两条不同的记录 取不到值的情况设为默认值
	 * 激活存在同一个表中，激活的时候email从用户资料中获取
	 * 
	 * @param regVisit
	 * @return
	 */
	public int insertRegHis(RegVisitOperate regOpr) {
		int rt = 0;

		try {
			// 插入
			//rt = RegVisitOperateDAO.getInstance().insertOprLog(regOpr);
			QueueOpr qu = QueueOpr.getInstance();
            qu.push(regOpr);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertRegHis err");
		}

		return rt;
	}

	/**
	 * 有来源的情况
	 * 
	 * @param regVisit
	 * @param isSub
	 */
	public void insertRegHis(RegVisitOperate regOpr,
			RegVisitOperateSub regOprSub, boolean isSub) {

		if (isSub) {
			insertRegHis(regOpr);
			String s1 = (String) regOpr.getS1();
			if (s1 != null && !"".equals(s1)) {
				// 有来源的情况
				String domainName = regOpr.getDomainName();
				insertRegActSub(regOprSub, domainName);
			}
		}
	}

	/**
	 * 记录用户访问,第一步打开连接或者第一次进入注册页面
	 * 
	 * @param regVisit
	 * @return int
	 */
	public int insertVisitHisFirst(RegVisitLogView logView) {
		int rt = 0;
		try {
			// 插入
			//rt = RegVisitLogViewDAO.getInstance().insertLogView(logView);
		    Queue qu = Queue.getInstance();
            qu.push(logView);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertVisitHisFirst err");
		}
		return rt;
	}

	/**
	 * 记录用户访问cookie, 第一步打开连接
	 * 
	 * @param regVisit
	 * @return int
	 */
	public int insertVisitCookieFirst(RegVisitCookieLogView regVisit) {
		int rt = 0;

		try {
			// 插入
			//rt = RegVisitCookieLogDAO.getInstance().insertLogView(regVisit);
		    QueueCookie qu = QueueCookie.getInstance();
		    qu.push(regVisit);
		} catch (Exception e) {
			System.err.println("UserTraceLogic.insertVisitCookieFirst err");
		}
		return rt;
	}

	public static void main(String[] args) {
		String aa = "192.168.1.110, 192.168.1.120, 192.168.1.130, 192.168.1.100";
		System.out.println("helllo");
		System.out.println(aa.substring(0, aa.indexOf(",")));
		System.out.println(aa.substring(0, 20));
		String s = "ff";
		System.out.println(Integer.parseInt(s,16));
	}
}
