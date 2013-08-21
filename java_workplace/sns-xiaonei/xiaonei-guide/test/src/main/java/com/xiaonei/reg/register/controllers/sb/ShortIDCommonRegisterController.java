package com.xiaonei.reg.register.controllers.sb;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.logic.RegNotifyLogic;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegIDCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/s-c-i-reg"
 * type="com.xiaonei.reg.register.action.sb.ShortIDCommonRegisterAction"
 * name="regIDCommonForm" scope="request"> <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="success-qq"
 * path="/pages/register/reg/register_ok_1105.jsp" /> <forward name="err"
 * path="/init/xn6212" /> </action>
 * 
 * @author tai.wang@opi-corp.com
 */
@Path("s-c-i-reg")
public class ShortIDCommonRegisterController extends BaseRegSubmitController {

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegIDCommonForm.class;
	}

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		if (user != null && !StringFormat.isValidEmailAddr(user.getAccount())) {
			try {
				// 邀请链接上的邀请人id
				int ivId = 0;
				String i = request.getParameter("i");
				if (StringUtils.isNotEmpty(i)) {
					ivId = StringUtils.isNumeric(i) ? Integer.parseInt(i) : 0;
				}

				String actionStr = String
						.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE);
				if (StringFormat.isValidEmailAddr(user.getAccount())) {
				} else if (StringFormat.isValidMobile(user.getAccount())) {
				} else {
					actionStr = String.valueOf(RegOprDefine.AUTO_XID_ACTIVE);
				}
				String source = request.getParameter("ss");
				UserTraceLogic.getInstance().insertRegHis(request, user,
						user.getAccount(), actionStr, 0, source, null,
						baseRegForm.getG(), baseRegForm.getB(), ivId, true);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
		String source = request.getParameter("ss");
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(), actionStr, 0, source, null,
				baseRegForm.getG(), baseRegForm.getB(), 0, true);		
		
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
				actionStr, 0, source, null,
				baseRegForm.getG(), baseRegForm.getB(), 0, true);
	}

	@Override
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response, User user) {
		return null;
	}

	@Override
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		return null;
	}

	@Override
	protected IRegisterLogic getRegisterLogic() {
		return RegisterLogicFactory.getRegShortCommon();
	}

	@Override
	protected RegForward succForward(final HttpServletRequest request,
			final HttpServletResponse response, RegMapping mapping,
			final User user) {
		if (null == user) {
			return null;
		}
		String pageId = request.getParameter("pageId");
		//game特殊处理
		String actId = StringUtils.trimToEmpty(request.getParameter("action_id"));
		try {
			if (user != null
					&& !StringFormat.isValidEmailAddr(user.getAccount())) {
				ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
				tasksMap.put("activate user", new Thread() {

					@Override
					public void run() {
						ActivateLogic.getInstance().activateUser(user);
					}
				});
				tasksMap.put("login", new Thread() {

					@Override
					public void run() {
						LoginLogic.getInstance().doLoginR(user, request,
								response, 0, false);
					}
				});
				//id注册完成后发送单独的notify
				tasksMap.put("xid notify", new Thread() {
                    @Override
                    public void run() {
                        RegNotifyLogic.getInstance().sendXidRegNofify(user);
                    }
                });
				RunnableTaskExecutor.getInstance().runConcurrentTasks(
						"short id common", tasksMap);
				//page 注册特殊处理
				if(!StringUtils.isEmpty(pageId)){
				    String fs = RegFromSourceLogic.getFromSource(user);
			        List<String> ls = RegFromSourceLogic.getFromSoureList(fs);
			        String toUrl = Globals.urlWww + "/Home.do";
			        if(ls.contains("S_IDPAGE;")){
			            toUrl = DodoyoResource.getString("activate.jump.url.S_PAGEREG", "pageids") + pageId;
			        }
				    return new RegForward(toUrl, true);
				}
				//game注册特殊处理
				else if("205230".equals(actId)){
				    String fs = RegFromSourceLogic.getFromSource(user);
                    List<String> ls = RegFromSourceLogic.getFromSoureList(fs);
                    String toUrl = Globals.urlWww + "/Home.do";
                    if(ls.contains("S_GAMEREG")){
                        toUrl = DodoyoResource.getString("activate.jump.url.S_GAMEREG", "pageids");
                    }
                    return new RegForward(toUrl, true);
				}else{
				    return new RegForward(Globals.urlWww + "/Home.do", true);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		request.setAttribute("email", user.getAccount());
		request.setAttribute("emailPage", RegUtil.emailLoginUrl(user
				.getAccount()));
		if(!StringUtils.isEmpty(pageId)){
		    int pid = Integer.parseInt(pageId);
		    Page page;
            try {
                page = PageService.getInstance().get(pid);
                if(page!= null){
                    request.setAttribute("page", page);
                }
            } catch (PageException e) {
                e.printStackTrace();
            }
		}
		return mapping.findForward(succForward.toString());
	}

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		/*map.put("success-qq", new RegForward(
				"/pages/register/reg/register_ok_1105.jsp"));*/
		//page注册，特殊激活页面
        map.put("success-6224", new RegForward("/pages/register/reg/register_ok_6224.jsp"));        
		map.put("err", new RegForward("/init/xn6212"));
		return map;
	}

}
