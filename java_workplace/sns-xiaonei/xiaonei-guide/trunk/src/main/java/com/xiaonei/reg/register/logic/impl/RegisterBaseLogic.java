package com.xiaonei.reg.register.logic.impl;

import org.apache.log4j.Logger;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.adapter.NetworkAdapter;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.register.dao.RegAuditDAO;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.additional.InitializtionCountLogic;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.additional.RegNetworkLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;

/**
 * 大家好，我是负责无需审核或审核后的注册的基类<br>
 * 例如：白领注册（无需审核），高中审核后注册<br>
 * 我包括: <br>
 * public void register(RegForm regForm);<br>
 * private void buildPerson(RegForm regForm);<br>
 * protected abstract void additional(RegForm regForm);<br>
 * 大家需要填充一下 addtiional 方法<br>
 * <br>
 * 请所有处理处理提交信息的类集成我
 * <p/>
 * RegisterBaseLogic.javas
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-25 上午11:13:02
 */
public abstract class RegisterBaseLogic implements IRegisterLogic {

	RegLogger log = RegLogger.getLogger("regLog", "REGBASE");

	public int register(final RegisterPojo regPojo) throws Exception {
		TimeCost cost = TimeCost.begin();
		final User oceUser;
		cost.endStep();
		try {
			oceUser = this.buildPerson(regPojo);
		} catch (Throwable e) {
			throw new Exception(e);
		}
		cost.endStep();
		additionalSync(oceUser, regPojo);
		cost.endStep();
		Runnable task = new Runnable() {
			public void run() {
				additionalBase(oceUser, regPojo);
			}
		};
		cost.endStep();
		RunnableTaskExecutor.getInstance().runTask(task);
		cost.endStep();
		int logCode = IRegisterLogic.err_noerr;
		if(oceUser!=null){
		    logCode = -oceUser.getId();
		    Logger.getLogger("regSSLog").info(
				regPojo.getActionId() + "," + oceUser.getId() + ","
						+ regPojo.getSs());
		}
		cost.endFinally();
		return logCode;
	}

	/**
	 * 在创建一个活生生的用户，这是关键部分
	 * 
	 * @param regPojo
	 * @param request
	 * @param response
	 * @return 新生儿
	 * @throws Throwable
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-25 下午05:30:13
	 */
	protected abstract User buildPerson(RegisterPojo regPojo) throws Throwable;

	/**
	 * ; 后注册时代<br>
	 * 给新人找朋友，加属性，添加app，等等
	 * 
	 * @param oceUser
	 * @param regPojo
	 * @param audit
	 * @return 错误代码
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-9-24 上午11:01:14
	 */

	private int additionalBase(User oceUser, RegisterPojo regPojo) {
	    if(oceUser != null)
	        log.info(oceUser.getId() + " additinal start");
		int rt = additional(oceUser, regPojo);
		if(oceUser != null)
		    log.info(oceUser.getId() + " additinal end");
		return rt;
	}

	protected abstract int additionalSync(User oceUser, RegisterPojo regPojo);

	protected abstract int additional(User oceUser, RegisterPojo regPojo);

	protected RegUserConfigLogic userConfigLogic = RegUserConfigLogic
			.getInstance();

	protected InitializtionCountLogic initializtionCountLogic = InitializtionCountLogic
			.getInstance();

	protected RegAuditDAO regAuditDAO = RegAuditDAO.getInstance();

	protected NetworkAdapter networkAdapter = AdapterFactory
			.getNetworkAdapter();

	protected RegNetworkLogic regNetworkLogic = RegNetworkLogic.getInstance();

	protected RegEmailLogic regEmailLogic = RegEmailLogic.getInstance();

	protected RegAccountLogic regAccountLogic = RegAccountLogic.getInstance();

}
