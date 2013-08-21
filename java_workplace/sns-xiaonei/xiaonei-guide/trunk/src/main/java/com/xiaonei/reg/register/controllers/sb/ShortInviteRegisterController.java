package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.ObjectHolder;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegShortInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegInviteTypeLogic;
import com.xiaonei.reg.register.pojo.RegFromSourcePojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/s-reg"
 * type="com.xiaonei.reg.register.action.sb.ShortInviteRegisterAction"
 * name="regShortInviteForm" scope="request"> <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="success-qq"
 * path="/pages/register/reg/register_ok_1021.jsp" /> <forward name="err"
 * path="/init/xn6205" /> </action>
 * 
 * @author wangtai
 * 
 */
@Path("s-reg")
public class ShortInviteRegisterController extends BaseRegSubmitController {

    // 需要完成跳转
	private static final String errForward = "err";

	@Override
	protected String getErrForward() {
		return errForward;
	}

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegShortInviteForm.class;
	}

	private static final ThreadLocal<Thread> taskGetInvitType = new ThreadLocal<Thread>();

	private static final ThreadLocal<ObjectHolder<Integer>> inviteType = new ThreadLocal<ObjectHolder<Integer>>();

	private static final ThreadLocal<Boolean> isAutoActivate = new ThreadLocal<Boolean>();

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		if (!isAutoActivate.get()) {
			return;
		}
		// 邀请链接上的邀请人id
		int ivId = 0;
		final String i = request.getParameter("i");
		if (StringUtils.isNotEmpty(i)) {
			ivId = StringUtils.isNumeric(i) ? Integer.parseInt(i) : 0;
		}

		String actionStr = String.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE);
		if (StringFormat.isValidEmailAddr(user.getAccount())) {
			// do nothing
		} else if (StringFormat.isValidMobile(user.getAccount())) {
			// do noting
		} else {
			actionStr = String.valueOf(RegOprDefine.AUTO_XID_ACTIVE);
		}
		final String source = request.getParameter("ss");
		UserTraceLogic.getInstance().insertRegHis(request, user,
				user.getAccount(), actionStr, 0, source,
				request.getParameter("appId"), baseRegForm.getG(),
				baseRegForm.getB(), ivId, true);
	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		// 邀请链接上的邀请人id
		int ivId = 0;
		final String i = request.getParameter("i");
		if (StringUtils.isNotEmpty(i)) {
			ivId = StringUtils.isNumeric(i) ? Integer.parseInt(i) : 0;
		}

		String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
		if (!StringUtils.equals(actionStr, "" + RegOprDefine.STD_REG)) {
			actionStr = "20" + StringUtils.substring(actionStr, 2);
		}
		final String source = request.getParameter("ss");
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(), actionStr, 0, source,
				request.getParameter("appId"), baseRegForm.getG(),
				baseRegForm.getB(), ivId, true);
		//2010.03.25修改
		actionStr = String.valueOf(RegOprDefine.NORMAL_REG_OK);
        if (StringFormat.isValidEmailAddr(user.getAccount())) {
            // do nothing
        } else if (StringFormat.isValidMobile(user.getAccount())) {
            actionStr = String.valueOf(RegOprDefine.NORMAL_MOBILE_REG_OK);
        } else {
            actionStr = String.valueOf(RegOprDefine.NORMAL_ID_REG_OK);
        }
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(),
				actionStr, 0, source,
				request.getParameter("appId"), baseRegForm.getG(),
				baseRegForm.getB(), ivId, true);
	}

	@Override
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response, User user) {
		isAutoActivate.set(false);
		try {
			taskGetInvitType.get().join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		// 判断是否自动激活
		if (inviteType.get().get() == RegInviteTypeLogic.REG_INVITE_LINK) {
			isAutoActivate.set(true);
			//把自动激活保存到缓存，在后面的激活后置中进行判断使用			
			try {
                MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
                String memKey = Globals.MEM_CACHE_REG_INVITE_MAIL + user.getId();
                mem.set(memKey, "OK", (int) (MemCacheKeys.hour * 1));
            } catch (RuntimeException e) {
                e.printStackTrace();
            }
		}
		request.setAttribute("userId", user.getId());
		final String url = request.getParameter("fromUrl");
		request.setAttribute("fromUrl", url);
		return null;
	}

	@Override
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		final RegisterPojo regPojo = new RegisterPojo(baseRegForm,
				getFromIp(request));

		final ObjectHolder<Integer> i = new ObjectHolder<Integer>();
		inviteType.set(i);
		taskGetInvitType.set(new Thread() {

			@Override
			public void run() {
				i.set(RegInviteTypeLogic.getInstance().checkInviteType(
						regPojo.getInviterId(), regPojo.getInviteeEmail(),
						regPojo.getRegEmail()));
			}
		});
		taskGetInvitType.get().start();
		return null;
	}

	@Override
	protected IRegisterLogic getRegisterLogic() {
		return RegisterLogicFactory.getRegShortInvite();
	}

	@Override
	protected RegForward succForward(final HttpServletRequest request,
			final HttpServletResponse response, RegMapping mapping,
			final User user) {

		/* 激活 */
		if (isAutoActivate.get() && null != user) {
			ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
			tasksMap.put("activate user", new Thread() {

				@Override
				public void run() {
					ActivateLogic.getInstance().activateUser(user);
				}
			});
			tasksMap.put("login tiket", new Thread() {

				@Override
				public void run() {
					LoginLogic.getInstance().doLoginR(user, request, response,
							0, false);
				}
			});
			RunnableTaskExecutor.getInstance().runConcurrentTasks(
					"short invite activate", tasksMap);
			return new RegForward(Globals.urlWww + "/Home.do", true);
		} else {
			// 发送激活信
			// RegEmailLogic.getInstance().sendActivateEmail(user);
			if (user != null) {
				request.setAttribute("email", user.getAccount());
				request.setAttribute("emailPage", RegUtil.emailLoginUrl(user
						.getAccount()));
			}
			return mapping.findForward(succForward.toString());
		}
	}

	@Override
	protected RegFromSourcePojo getFormSourcePojo(int userId,
			HttpServletRequest request) {
		return super.getFormSourcePojo(userId, request).setIp(
				getFromIp(request)).setActionId(request.getParameter("action_id"));
	}

	@Override
	protected RegForward tearDown(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		ShortInviteRegisterController.inviteType.remove();
		ShortInviteRegisterController.isAutoActivate.remove();
		ShortInviteRegisterController.taskGetInvitType.remove();
		return super.tearDown(mapping, baseRegForm, request, response);
	}

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		// <forward name="success" path="/pages/register/register_ok.jsp" />
		// <forward name="success-qq"
		// path="/pages/register/reg/register_ok_1021.jsp" />
		// <forward name="err" path="/init/xn6205" />
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		map.put("success-qq", new RegForward(
				"/pages/register/reg/register_ok_1021.jsp"));
		map.put("err", new RegForward("/init/xn6205"));
		return map;
	}
}
