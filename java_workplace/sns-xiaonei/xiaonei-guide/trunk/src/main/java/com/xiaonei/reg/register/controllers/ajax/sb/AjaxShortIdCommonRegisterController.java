package com.xiaonei.reg.register.controllers.ajax.sb;

import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegMsgLogic;
import com.xiaonei.reg.register.logic.buildperson.RegAccountLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.RegCheckManager;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;


/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Apr 19, 2010 3:53:26 PM
 * Ajax 异步注册接口
 */

@Path("asreg")
public class AjaxShortIdCommonRegisterController extends BaseRegAjaxController {

    protected static final RegCheckManager checkManager = RegCheckManager.getInstance();
    private RegLogger alogger = RegLogger.getLoggerWithType(this.getClass());
    
    @SuppressWarnings("unchecked")
    @Override
    public RegForward executeRAt(RegMapping mapping, RegForm form, HttpServletRequest request,
            HttpServletResponse response) {
        int succ = 0;
        int fail = 1;
        
        /* 搞个form */
        final RegShortCommonForm regForm = new RegShortCommonForm();
        String regEmail = StringUtils.trimToEmpty(request.getParameter("regEmail"));
        String pwd = request.getParameter("pwd");
        regForm.setSs(request.getParameter("ss"));
        regForm.setAction_id(request.getParameter("action_id"));
        String strAccType = StringUtils.isNotEmpty(request.getParameter("accType")) 
                                && StringUtils.isNumeric(request.getParameter("accType")) 
                                ? request.getParameter("accType")
                                    : "1";
        regForm.setAccType(Integer.parseInt(strAccType));
        regForm.setNicknameId(request.getParameter("nicknameId"));
        regForm.setRegEmail(regEmail);
        regForm.setPwd(pwd);
        regForm.setName(request.getParameter("name"));
        regForm.setGender(request.getParameter("gender"));
        regForm.setBirth_year(request.getParameter("birth_year"));
        regForm.setBirth_month(request.getParameter("birth_month"));
        regForm.setBirth_day(request.getParameter("birth_day"));
        regForm.setG(request.getParameter("g"));
        regForm.setB(request.getParameter("b"));
        this.printObj(request.getParameterMap());
        
        /* 1,表单数据验证 */
        final String rtmsg = checkManager.check(regForm, request, response);

        JSONObject o;
        String tmsg = "注册成功";
        
        if (checkManager.isOK(rtmsg) == false) {            
            o = outJson(fail, regEmail, rtmsg, null);
            return output(response, o.toString());
        }else{
            alogger.debug("AjaxShortIdCommonRegisterController check ok");
        }

        /* 2,业务逻辑,生成用户啥的... */
        final RegisterPojo regPojo = new RegisterPojo(regForm,
                BaseActionUtil.getClientIP(request));
        int userId;
        try {
            userId = -RegisterLogicFactory.getRegShortCommon().register(
                    regPojo);
        } catch (final Exception e) {
            /* err */
            tmsg = "系统错误,请稍候尝试";
            o = outJson(fail, regEmail, tmsg, null);
            return output(response, o.toString());
        }
        alogger.debug("AjaxShortIdCommonRegisterController create Account ok");
        
        /* 3,激活 */
        User user = null;
        try {
            user = SnsAdapterFactory.getUserAdapter().get(userId);
        }catch(Exception ex){
            ex.printStackTrace();
        }
        if(user == null){
        	o = outJson(fail, regEmail, tmsg, null);
        	return output(response, o.toString());
        }
        try{
            //usertrace
            UserTraceLogic.getInstance().insertRegHis(request, user,
                    regPojo.getRegEmail(), regForm.getAction_id(), 0,
                    regForm.getSs(), null, regForm.getG(), regForm.getB(), 0,
                    true);
            
            //2010.03.25修改
            String actionStr = String.valueOf(RegOprDefine.NORMAL_REG_OK);
            if (StringFormat.isValidEmailAddr(user.getAccount())) {
                // do nothing
            } else if (StringFormat.isValidMobile(user.getAccount())) {
                actionStr = String.valueOf(RegOprDefine.NORMAL_MOBILE_REG_OK);
            } else {
                actionStr = String.valueOf(RegOprDefine.NORMAL_ID_REG_OK);
            }
            UserTraceLogic.getInstance().insertRegHis(request, user,
                    regPojo.getRegEmail(),
                    actionStr, 0,
                    regForm.getSs(), null, regForm.getG(), regForm.getB(), 0,
                    true);
            
            final User wu = user;
            //登录
            if(user != null){
                LoginLogic.getInstance().doLoginR(user, request, response, 0, false);
                noActivateForward(user);
            }
            
            //xid注册自动激活&登录
            if (user != null 
                    && !StringFormat.isValidEmailAddr(regForm.getRegEmail())) {
                ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
                alogger.debug("AjaxShortIdCommonRegisterController is xid reg,begin activate user");

                tasksMap.put("activate user", new Thread() {
                    @Override
                    public void run() {
                        ActivateLogic.getInstance().activateUser(wu);
                    }
                });               
                RunnableTaskExecutor.getInstance().runConcurrentTasks(
                        "short id common", tasksMap);
                alogger.debug("AjaxShortIdCommonRegisterController is xid reg,activate user end");
                
                o = outJson(succ, regEmail, tmsg, user);
                return output(response, o.toString());
            }
        } 
        catch (Exception e) {
            o = outJson(succ, regEmail, tmsg, user);
            return output(response, o.toString());
        }
        o = outJson(succ, regEmail, tmsg, user);
        return output(response, o.toString());
    }

    private void noActivateForward(final User user) {
        boolean isConfirmed = true;// 已经验证标志
        try {
            isConfirmed = UserConfirmLogic.getInstance().isConfirmed(user);
        } catch (final Exception e) {
            e.printStackTrace();
        }
        //剔除掉id注册的情况
        if ((!isConfirmed && StringFormat.isValidEmailAddr(user.getAccount()))) {
            
            ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
            alogger.debug("AjaxShortIdCommonRegisterController is xid reg,begin activate user");

            //激活后置用户发送站内信
            tasksMap.put("send welcome letter", new Thread() {
                @Override
                public void run() {
                    try {
                        RegMsgLogic.getInstance().sendWelcomeMessageNew(user);
                    } catch (Exception e) {
                    }
                }
            });
            //设置导人的标志
            /*
            tasksMap.put("set import msn flag", new Thread() {
                @Override
                public void run() {
                    try {
                        StepStatusLogic.updateStepStatus(user.getId(), IStepStatusDefinition.STEP_IMPORT_MSN);
                    } catch (Exception e) {
                    }
                }
            });
            */
            RunnableTaskExecutor.getInstance().runConcurrentTasks(
                    "deal with no activate", tasksMap);            
        }
    }
    
    private JSONObject outJson(int result, String regEmail, final String rtmsg, User user) {
        JSONObject o = new JSONObject();
        o.put("code", result);
        o.put("msg", rtmsg);
        o.put("email", regEmail);
        o.put("emailPage", RegUtil.emailLoginUrl(regEmail));
        
        String vc = "";
        if(user != null){
            String vc_key = RegAccountLogic.MEM_CACHE_NEW_INVITEE_KEY + user.getId();
            try {
                MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
                vc = (String)manager.get(vc_key);
            } catch (RuntimeException e) {
                e.printStackTrace();
            }
            if(StringUtils.isEmpty(vc)){
                vc = UserAdapter.get(user).getVerifyCode();
            }
        }
        o.put("verify", vc);
        alogger.info("ajax register info. " + o.toString());
        return o;
    }

    private RegForward output(HttpServletResponse response, String msg) {
        response.setContentType("text/html;charset=utf-8");
        try {
            PrintWriter out = response.getWriter();
            out.print(msg);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
    }
    
    private void printObj(Map<String, String> map) {
        if (map != null) {
            for (java.util.Map.Entry<String, String> b : map.entrySet()) {
                String k = (String)b.getKey();
                String val = "";
                Object obj = b.getValue();
                if(obj instanceof String[]){
                    val = Arrays.toString((String[])obj);
                }else{
                    val = (String)obj;
                }
                alogger.debug(
                        "key: " + k + ",value: " + val);
            }
        }
    }
}
