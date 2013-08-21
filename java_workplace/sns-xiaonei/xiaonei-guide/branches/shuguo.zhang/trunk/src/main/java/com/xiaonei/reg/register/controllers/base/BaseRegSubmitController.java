package com.xiaonei.reg.register.controllers.base;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Map;
import java.util.Random;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegTools;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegInviteFriendBusinessLogic;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.logic.additional.RegMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegNoActivateLogic;
import com.xiaonei.reg.register.logic.additional.RegNoActivateRecordLogic;
import com.xiaonei.reg.register.logic.additional.RegReferLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.RegCheckManager;
import com.xiaonei.reg.register.model.RegExtraMsg;
import com.xiaonei.reg.register.pojo.RegFromSourcePojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.reg.usertrace.q.UTUtils;

@AsSuperController
public abstract class BaseRegSubmitController extends BaseRegController {

	protected static final RegCheckManager checkManager = RegCheckManager
			.getInstance();

	private final ThreadLocal<String> pageId = new ThreadLocal<String>();

	private final ThreadLocal<String> regAccount = new ThreadLocal<String>();

	protected Object succForward = new Object() {

		public String toString() {
			return getSuccForward();
		};
	};

	@Override
	final public RegForward executeRt(final RegMapping mapping,
			final RegForm regForm, final HttpServletRequest request,
			final HttpServletResponse response) {
		final RegForward suRt = this.setUp(mapping, regForm, request, response);
		if (null != suRt) {
			return down(suRt, mapping, regForm, request, response);
		}
		final TimeCost cost = TimeCost.begin(this.getClass().getName());
		pageId.set(getPageId(request));
		regAccount.set(request.getParameter("regEmail"));
		final BaseRegForm baseRegForm = getFormClazz().cast(regForm);
		cost.endStep();

		/* 1,表单数据验证 */
		final String rtmsg = checkManager.check(baseRegForm, request, response);
		cost.endStep();
		if (checkManager.isOK(rtmsg) == false) {
			request.setAttribute("regForm", baseRegForm);
			initErrForm(request);			
			cost.endStep();
			cost.endFinally();
			addErr(request, rtmsg);
			rlog.debug("flow into right error page , init it ..");
			return down(errForward(mapping, request, baseRegForm, null, rtmsg),
					mapping, regForm, request, response);
		}
		cost.endStep();

		/* 2,业务逻辑,生成用户啥的... */
		int regCode = IRegisterLogic.err_noerr;
		regMonitorLog(baseRegForm, request, RegUtil.bizAccount, RegUtil.chkBegin, regCode);
		final RegForward brt = before(mapping, baseRegForm, request, response);
		if (null != brt) {
			return down(brt, mapping, regForm, request, response);
		}
		final RegisterPojo regPojo = new RegisterPojo(baseRegForm,
				getFromIp(request));
		try {
			regCode = getRegisterLogic().register(regPojo);
		} catch (final Exception e) {
		    regMonitorLog(baseRegForm, request, RegUtil.bizAccount, RegUtil.chkFail, regCode);
			return down(errForward(mapping, request, baseRegForm, e,
					IRegCheckCodeDef.ERR_SYSTEM), mapping, regForm, request,
					response);
		}
		if (regCode >= IRegisterLogic.err_noerr) {
		    regMonitorLog(baseRegForm, request, RegUtil.bizAccount, RegUtil.chkFail, regCode);
			return down(errForward(mapping, request, baseRegForm, null,
					IRegCheckCodeDef.ERR_SYSTEM), mapping, regForm, request,
					response);
		}

		final int userId = -regCode;
		final User user = UserAdapter.getUser(userId);
		if (null == user) {
		    regMonitorLog(baseRegForm, request, RegUtil.bizAccount, RegUtil.chkFail, -regCode);
			return down(errForward(mapping, request, baseRegForm, null,
					IRegCheckCodeDef.ERR_SYSTEM), mapping, regForm, request,
					response);
		}
		regMonitorLog(baseRegForm, request, RegUtil.bizAccount, RegUtil.chkEnd, -regCode);
		
		final RegForward art = after(mapping, baseRegForm, request, response,
				user);
		if (null != art) {
			return down(art, mapping, regForm, request, response);
		}
		
		/* 增加用户行为跟踪日志 */
		RegFromSourceLogic.saveFromSource(getFormSourcePojo(userId, request));
				
		//注册增加推荐人的临时统计
		loggerRecomm(regPojo, user, request);
		
		/* 保存注册的关键信息 */
		recordExtraMsg(regPojo, user, request);
		
		/* refer统计 */
		String regRefer = "";
        if(StringUtils.isNotEmpty(baseRegForm.getFromUrl())){
            if(baseRegForm.getFromUrl().length() > 255){
                regRefer = baseRegForm.getFromUrl().substring(0, 255);
            }else{
                regRefer = baseRegForm.getFromUrl();
            }
        }
		RegReferLogic.getInstance().set(user.getId(),
				baseRegForm.getAction_id(), regRefer);
		
		//统计，运营
		this.addUserTraceLogRegSucc(regPojo, request, user, baseRegForm);
		regCode = IRegisterLogic.err_noerr;
		cost.endStep();

		/* 3,跳转 */
		cost.endFinally();
		boolean isAuto = isAutoMailInviteLink(user);
		rlog.info("Is Auto Mail Invite Link result::::::" + isAuto);
		
		
		//跟好友相关的操作
		RegInviteFriendBusinessLogic.getInstance().doFriendsBusiness(user);
		
		//后续统计，运营
		this.addUserTraceLogForward(regPojo, request, user, baseRegForm);
		
		//MSN注册用户需要直接激活
		final RegForward isMsn = isMsnForward(request, response, mapping, user);
		if (null != isMsn) {
			return down(isMsn, mapping, regForm, request, response);
		}
		
		if(isAuto){
			//自动激活相关的操作
			final RegForward succRt = succForward(request, response, mapping, user);
			return down(succRt, mapping, regForm, request, response);
		}else{
			//激活后置
			final RegForward noAcRt = noActivateForward(request, response, mapping,
					user);
			if (null != noAcRt) {
				return down(noAcRt, mapping, regForm, request, response);
			}
			
			//直接激活流程
			final RegForward directNoAcRt = noActivate(request, response, mapping,
	                user);
	        if (null != directNoAcRt) {
	            return down(directNoAcRt, mapping, regForm, request, response);
	        }
			final RegForward succRt = succForward(request, response, mapping, user);
			return down(succRt, mapping, regForm, request, response);
		}
	}

	/**
	 * 错误页面的处理
	 * @param request
	 */
	private void initErrForm(final HttpServletRequest request) {
		//获取page相关的一些信息
		String pageId = request.getParameter("pageId");
		if(!StringUtils.isEmpty(pageId) && StringUtils.isNumeric(pageId)){
		    int pid = Integer.parseInt(pageId);
		    Page page;
		    try {
		        page = PageService.getInstance().get(pid);
		        if(page!= null){
		            request.setAttribute("page", page);
		            request.setAttribute("pageId", pageId);
		        }
		    } catch (PageException e) {
		        e.printStackTrace();
		    }
		}
		String appId = StringUtils.trimToEmpty(request.getParameter("appId"));
		if(StringUtils.isNotEmpty(appId)){
		    request.setAttribute("appId", appId);
		}
		String msnRegToken = StringUtils.trimToEmpty(request.getParameter("m"));
		if(StringUtils.isNotEmpty(msnRegToken)){
		    request.setAttribute("m", msnRegToken);
		}
		//5234, 5235特殊的注册页面
		String rMail = StringUtils.trimToEmpty(request.getParameter("regEmail"));
		if(StringUtils.isNotEmpty(rMail)){
		    request.setAttribute("regEmail", rMail);
		}
		
	}

	/**
	 * down
	 * run before exit
	 *
	 * @param rt
	 * @param mapping
	 * @param regForm
	 * @param request
	 * @param response
	 * @return
	 *
	 * @author tai.wang@opi-corp.com Dec 15, 2009 - 5:51:56 PM
	 */
	private RegForward down(final RegForward rt, final RegMapping mapping,
			final RegForm regForm, final HttpServletRequest request,
			final HttpServletResponse response) {
		final RegForward tdRt = this.tearDown(mapping, (BaseRegForm) regForm,
				request, response);
		if (null != tdRt) {
			return tdRt;
		}
		this.pageId.remove();
		this.regAccount.remove();
		return rt;
	}

	/**
	 * 免激活
	 * 目前策略：QQ邮箱注册的用户且尾号为2/4/6的，注册后无需进入邮箱，自动激活
	 * @param request
	 * @param response
	 * @param mapping
	 * @param user
	 * @return RegForward
	 * @author wei.cheng@opi-corp.com
	 */
	private RegForward noActivate(final HttpServletRequest request,
            final HttpServletResponse response, final RegMapping mapping,
            final User user) {
	    boolean isNoActivated = RegNoActivateLogic.getInstance().isNoActivatedEmail(user);
        if (isNoActivated) {
            //激活用户
            ActivateLogic.getInstance().activateUser(user);
            //插入用户访问记录,用于用户注册引导的跟踪
            int ivId = RegInviterInviteeLogic.getInstance().getInviterId(
                    user.getId());
            String ss = request.getParameter("ss");
            UserTraceLogic.getInstance().insertRegHis(request, user,
                    user.getAccount(),
                    String.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE),
                    UserAdapter.get(user).getStage(), ss, null, null, null, ivId);
            //自动登录
            LoginLogic.getInstance()
                    .doLoginR(user, request, response, 0, false);
            RegNoActivateRecordLogic.getInstance().save(user);
            //进行下一步
            return new RegForward(Globals.urlHome.toString(), true);
        }
        return null;
	}
	
	/**
	 * msn注册用户的流程，需要自动激活
	 * @param request
	 * @param response
	 * @param mapping
	 * @param user
	 * @return
	 */
	private RegForward isMsnForward(final HttpServletRequest request,
			final HttpServletResponse response, final RegMapping mapping,
			final User user) {
		boolean isValid = RegNoActivateLogic.getInstance().isValidMsnUserPost(user, request);
        if (isValid) {
            //激活用户
            ActivateLogic.getInstance().activateUser(user);
            //插入用户访问记录,用于用户注册引导的跟踪
            int ivId = RegInviterInviteeLogic.getInstance().getInviterId(
                    user.getId());
            String ss = request.getParameter("ss");
            UserTraceLogic.getInstance().insertRegHis(request, user,
                    user.getAccount(),
                    String.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE),
                    UserAdapter.get(user).getStage(), ss, null, null, null, ivId);
            //自动登录
            LoginLogic.getInstance()
                    .doLoginR(user, request, response, 0, false);
            RegNoActivateRecordLogic.getInstance().save(user);
            //给这部分用户发站内信
			Thread letter = new Thread() {
                @Override
                public void run() {
                    try {
                        RegMsgLogic.getInstance().sendWelcomeMessageNew(user);
                    } catch (Exception e) {
                    }
                }
            };
            RunnableTaskExecutor.getInstance().runTask(letter);
            //开始跳转啦
            String to = "";
            try {
				to = URLEncoder.encode(Globals.urlHome.toString(), "UTF-8");
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
            return new RegForward(Globals.urlInvite.toString() +"/crtmt?tUrl=" + to, true);
        }
        return null;
	}
	
	private RegForward noActivateForward(final HttpServletRequest request,
			final HttpServletResponse response, final RegMapping mapping,
			final User user) {
		if(user == null)return null;
		boolean isConfirmed;// 已经验证标志
		try {
			isConfirmed = UserConfirmLogic.getInstance().isConfirmed(user);
		} catch (final Exception e) {
			e.printStackTrace();
			return null;
		}
		//剔除掉id注册的情况
		if ((!isConfirmed && StringFormat.isValidEmailAddr(user.getAccount()))
		        || (Globals.DEBUG_ENV && "true".equals(DodoyoResource.getString("renren.reg.no.activate.forward", "check-rules"))
		                && StringFormat.isValidEmailAddr(user.getAccount()))) { 
			LoginLogic.getInstance()
					.doLoginR(user, request, response, 0, false);
			//激活后置用户发送站内信
			Thread letter = new Thread() {
                @Override
                public void run() {
                    try {
                        RegMsgLogic.getInstance().sendWelcomeMessageNew(user);
                    } catch (Exception e) {
                    }
                }
            };
            RunnableTaskExecutor.getInstance().runTask(letter);
            //完成后，可以跳转啦
            if (user != null) {
                request.setAttribute("email", user.getAccount());
                request.setAttribute("emailPage", RegUtil.emailLoginUrl(user
                        .getAccount()));
            }
            if(!RegNoActivateLogic.getInstance().jumpActivatedPage(user)){
            	return new RegForward("/pages/register/reg/register_ok_no_activated.jsp");            	
            }else{
            	return new RegForward(Globals.urlHome.toString(), true);
            }
		}
		return null;
	}

	

	/**
	 * setUp
	 *
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 *
	 * @author tai.wang@opi-corp.com Dec 15, 2009 - 5:52:28 PM
	 */
	protected RegForward setUp(final RegMapping mapping, final RegForm form,
			final HttpServletRequest request, final HttpServletResponse response) {
		return null;
	}

	/**
	 * before
	 * run before register
	 *
	 * @param mapping
	 * @param baseRegForm
	 * @param request
	 * @param response
	 * @return
	 *
	 * @author tai.wang@opi-corp.com Dec 15, 2009 - 5:52:47 PM
	 */
	protected abstract RegForward before(final RegMapping mapping,
			final BaseRegForm baseRegForm, HttpServletRequest request,
			HttpServletResponse response);

	protected RegFromSourcePojo getFormSourcePojo(final int userId,
			final HttpServletRequest request) {
		final String sAccType = request.getParameter("accType");
		final int accType = StringUtils.isNumeric(sAccType) ? Integer
				.valueOf(sAccType) : 0;
		return RegFromSourcePojo.getNew(this.getClass(), userId).setIp(
				getFromIp(request)).setAccType(accType)
				.setActionId(request.getParameter("action_id"));
	}

	protected abstract RegForward succForward(final HttpServletRequest request,
			HttpServletResponse response, RegMapping mapping, User user);

	protected abstract RegForward after(RegMapping mapping,
			BaseRegForm baseRegForm, HttpServletRequest request,
			HttpServletResponse response, User user);

	protected abstract void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm);

	protected abstract void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm);

	protected abstract IRegisterLogic getRegisterLogic();

	protected RegForward tearDown(final RegMapping mapping,
			final BaseRegForm baseRegForm, final HttpServletRequest request,
			final HttpServletResponse response) {
		return null;
	}

	/**
	 * 错误跳转name
	 * 
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-8 - 下午11:03:15
	 */
	protected String getErrForward() {
		return "err";
	}

	/**
	 * 判断用户是否是需要自动激活
	 * @param user
	 * @return
	 * @author wei.cheng@opi-corp.com
	 * @version 1.0
	 * @date 创建时间：Apr 29, 2010 3:53:12 PM
	 */
	private boolean isAutoMailInviteLink(final User user){
	    try {
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
            String memKey = Globals.MEM_CACHE_REG_INVITE_MAIL + user.getId();
            Object obj = mem.get(memKey);
            if(obj!=null && "OK".equals((Object)obj)){
                return true;
            }
        } catch (RuntimeException e) {
            return false;
        }
        return false;
	}
	/**
	 * 
	 * @param regPojo
	 * @param oceUser
	 * @param request
	 * @author wei.cheng@opi-corp.com
	 */
	private void recordExtraMsg(RegisterPojo regPojo, User oceUser, HttpServletRequest request){
	  //需要记录引导使用的用户名称
	    Map<String,RegExtraMsg> msgList = RegExtraMsgLogic.getInstance().getMsgList(regPojo, oceUser, request);
        RegExtraMsgLogic.getInstance().saveMsg(oceUser, msgList);
	}
	/**
	 * 注册增加推荐人的临时统计
	 * @param regPojo
	 * @param oceUser
	 * @param request
	 */
	private final RegLogger rlog = RegLogger.getLoggerWithType(this.getClass().getSimpleName(),UTUtils.authorEmail);
	private void loggerRecomm(RegisterPojo regPojo, User oceUser, HttpServletRequest request){
        if(isNeedLogRecord(regPojo)){
            rlog.info("[RecommendRegStat]email=" + regPojo.getRegEmail() + ",name=" +
                regPojo.getName()+",gender="+regPojo.getGender()+",birth="+
                regPojo.getBirth_year()+regPojo.getBirth_month()+regPojo.getBirth_day()+",action_id=" + 
                regPojo.getActionId() + ",uid=" + oceUser.getId() + ",ss=" + 
                regPojo.getSs() + ",ip=" + getFromIp(request) + ",recommend=" + 
                request.getParameter("recommend")+ ",recommend2=" + 
                request.getParameter("recommend2")+ ",recommend3=" + 
                request.getParameter("recommend3"));
        }
    }

	/**
	 * 推荐好友的页面
	 * @param regPojo
	 * @return
	 */
    private boolean isNeedLogRecord(RegisterPojo regPojo) {
        return "206221".equals(regPojo.getActionId()) 
                || "206222".equals(regPojo.getActionId()) 
                || "206223".equals(regPojo.getActionId())
                || "206227".equals(regPojo.getActionId())
                || "206207".equals(regPojo.getActionId());
    }
    
    /**
     * 生人监控
     * @param regForm
     * @param request
     * @param biz
     * @param checkResult
     */
    private void regMonitorLog(final BaseRegForm regForm, final HttpServletRequest request, String biz, int checkResult, int userId) {
        RegUtil.rlog.info(regForm.getSs()+","+regForm.getAction_id()+ ","+biz+","+userId+","+RegUtil.getDomain(request)+"," + checkResult);
    }
    
	protected String getSuccForward() {
		final String pageId = this.pageId.get();
		//final String account = this.regAccount.get();
		this.pageId.remove();
		this.regAccount.remove();
		final String defaultFlow = "success";

		final String key = "ok." + pageId;
		final String flow = DodoyoResource.getString(key, "pageids");
		RegLogger.getLoggerWithType(this.getClass()).debug(
                "[getSuccForward] key: " + key + ",forw: " + flow);
		if (DodoyoResource.isNotValue(key, flow)) {
			return defaultFlow;
		}
		return flow;
	}

	/**
	 * 对应的regform
	 * 
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-9 - 上午09:03:18
	 */
	public abstract Class<? extends BaseRegForm> getFormClazz();

	/**
	 * 出错处理
	 * 
	 * @param mapping
	 * @param request
	 * @param baseRegForm
	 * @param e
	 * @param errMsg
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-5-6 - 下午04:09:49
	 */
	final protected RegForward errForward(final RegMapping mapping,
			final HttpServletRequest request, final BaseRegForm baseRegForm,
			final Throwable e, String errMsg) {
		if (e != null)
			e.printStackTrace();
		if (StringUtils.isEmpty(errMsg))
			errMsg = "系统错误，请稍后重试";
		addErr(request, errMsg);
		final String rt = getNewErrForward(request);
		if (null != rt) {
			return new RegForward(rt);
		}
		rlog.debug("getNewErrForward is null , begin get default error page.");
		return mapping.findForward(getErrForward());
	}

	/**
	 * 判断是如何进入出错注册页面，返回地址
	 * 
	 * @param request
	 * @return
	 */
	private String getNewErrForward(final HttpServletRequest request) {
		final String action_id = StringUtils.trimToEmpty(request
				.getParameter("action_id"));
		final String urlp = "^20[5|6][0-9]{3}$";
		try {
		    //6227iframe地址
		    String sUrl = StringUtils.trimToEmpty(request.getParameter("surl"));
	        if(StringUtils.isEmpty(sUrl)){
	            sUrl = (String)request.getAttribute("iframeUrl");
	        }
	        if(StringUtils.isNotEmpty(sUrl)){
	            if(internalURL(sUrl)){
                    request.setAttribute("iframeUrl", sUrl);
                }
	        }
			if (Pattern.compile(urlp).matcher(action_id).matches()) {
				rlog.debug("error page is right, the action is " + action_id);
				return "/init/xn" + action_id.substring(2, 6);
			}
		} catch (final Exception e) {
		}
		return null;
	}

	/**
	 * 6位pageid
	 * 
	 * @param request
	 * @return
	 */
	private String getPageId(final HttpServletRequest request) {
		final String actionId = request.getParameter("action_id");
		if (StringUtils.isNotEmpty(actionId)) {
			return actionId;
		}
		return "";
	}

	public boolean internalURL(String url) {
        String s = "(.*\\.)?(renren|xiaonei|kaixin)\\.(com|cn)";
        String host = RegTools.getHost(url);
        boolean r = host.matches(s);
        return r;
    }
	
	public static void main(final String[] aaa) {
		int less = 0;
		int more = 0;
		for (int i = 0; i < 500; i++) {
			final Random rand = new Random();
			final int r = rand.nextInt(10);
			if (r <= 4) {
				less++;
			} else {
				more++;
			}
		}
		System.out.println(less + "," + more);
	}
}
