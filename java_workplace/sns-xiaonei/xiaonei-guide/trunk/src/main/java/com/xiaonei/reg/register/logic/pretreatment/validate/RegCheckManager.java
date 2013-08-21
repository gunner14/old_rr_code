/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment.validate;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.reg.common.ObjectHolder;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.constants.RegFormCheckMap;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

/**
 * RegCheckManager
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-30 - 下午07:29:00
 */
public class RegCheckManager implements IRegCheckCodeDef {

	private static final RegCheckManager _instance = new RegCheckManager();
	
	private RegCheckManager() {

	}

	public static RegCheckManager getInstance() {
		return _instance;
	}

	private static final Map<Class<?>, Class<? extends BaseRegCheckLogic>> fcMap = RegFormCheckMap
			.getMap();

	final public List<BaseRegCheckLogic> getChecker(
			final Class<? extends BaseRegForm> clazz) {
		final Class<?>[] interfaces = clazz.getInterfaces();
		final List<BaseRegCheckLogic> cl = new ArrayList<BaseRegCheckLogic>();
		for (final Class<?> i : interfaces) {
			final Class<? extends BaseRegCheckLogic> checkClass = fcMap.get(i);
			if (null == checkClass) {
				continue;
			}
			cl.add(ProxyMethodLog.getLogProxy(checkClass));
		}
		return cl;
	}

	RegLogger logger = RegLogger.getLoggerWithType("check manager tasks");

	final public String check(final BaseRegForm regForm,
			final HttpServletRequest request, final HttpServletResponse response) {
		final TimeCost cost = TimeCost.begin();
		String rtStr = OK;
		
		// check icode
		regMonitorLog(regForm, request, RegUtil.bizIcode, RegUtil.chkBegin);
		rtStr = CheckICode.checkICode(request);
		cost.endStep();
		if (!OK.equals(rtStr)) {
		    regMonitorLog(regForm, request, RegUtil.bizIcode, RegUtil.chkFail);
			return rtStr;
		}
		regMonitorLog(regForm, request, RegUtil.bizIcode, RegUtil.chkEnd);
		cost.endStep();
		
		// check form
		regMonitorLog(regForm, request, RegUtil.bizRegForm, RegUtil.chkBegin);
		final List<BaseRegCheckLogic> cll = this.getChecker(regForm.getClass());
		final ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
		final ObjectHolder<String> tRtStr = new ObjectHolder<String>();
		cost.endStep();
		for (final BaseRegCheckLogic cl : cll) {
			tasksMap.put(cl.getClass().getName(), new Thread() {
				@Override
				public void run() {
					String rtStr = cl.check(regForm);
					synchronized (rtStr) {
						if (null == (tRtStr.get()) || OK.equals(tRtStr.get())) {
							tRtStr.set(rtStr);
						} else {
							return;
						}
					}
				}
			});
			cost.endStep(cl.getClass().getName());
		}
		for (Entry<String, Thread> task : tasksMap.entrySet()) {
			task.getValue().start();
			logger.info("task " + task.getKey() + " start");			
		}
		for (Entry<String, Thread> task : tasksMap.entrySet()) {
			try {
				task.getValue().join();
				rtStr = StringUtils.trimToEmpty(tRtStr.get());
				logger.info("task " + task.getKey() + " over, result:" + rtStr);
				if (!OK.equals(rtStr)) {
					logger.debug("tasks break loop ");
					break;
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		cost.endStep();
		if (!OK.equals(rtStr)) {
		    regMonitorLog(regForm, request, RegUtil.bizRegForm, RegUtil.chkFail);
			return rtStr;
		}
		regMonitorLog(regForm, request, RegUtil.bizRegForm, RegUtil.chkEnd);
		cost.endStep();
		
		// anti spamer
		regMonitorLog(regForm, request, RegUtil.bizRegSpam, RegUtil.chkBegin);
		rtStr = antispam(regForm, request, response, rtStr);
		cost.endStep();
		if (!OK.equals(rtStr)) {
		    regMonitorLog(regForm, request, RegUtil.bizRegSpam, RegUtil.chkFail);
			return rtStr;
		}
		regMonitorLog(regForm, request, RegUtil.bizRegSpam, RegUtil.chkEnd);
		cost.endFinally();

		return rtStr;
	}

	/**
	 * reg表单验证log跟踪
	 * @param regForm
	 * @param request
	 * @param biz
	 * @param checkResult
	 */
    private void regMonitorLog(final BaseRegForm regForm, final HttpServletRequest request, String biz, int checkResult) {
        RegUtil.rlog.info(regForm.getSs()+","+regForm.getAction_id()+ ","+biz+",,"+RegUtil.getDomain(request)+"," + checkResult);
    }

	private String antispam(final BaseRegForm regForm,
			final HttpServletRequest request,
			final HttpServletResponse response, final String rtStr) {
		final String rtStrTmp = CheckAntispam.getInstance().checkAntispam(
				regForm, request, response);
		final String email = StringUtils.trimToEmpty(request
				.getParameter("regEmail"));
		if (Globals.NO_ANTISPAM_ENV
				&& email.endsWith(DodoyoResource.getString(
						"email.no.antispam.domain", "check-rules"))) {
			RegLogger.getLoggerWithType("NO_ANTISPAM_ENV").warn(rtStrTmp);
			return rtStr;
		}
		return rtStrTmp;
	}

	final public String getErrorValue(final String errCode) {
		final String errMsg = errCode;
		return errMsg;
	}

	final public boolean isOK(final String msg) {
		return OK.equals(msg);
	}	
	
}
