package com.xiaonei.reg.register.controllers.sb;

import java.util.Arrays;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.logic.RegNotifyLogic;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.RegCheckManager;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("wsreg")
public class WsregController {
    
    protected static final RegCheckManager checkManager = RegCheckManager.getInstance();
    private RegLogger wlogger = RegLogger.getLoggerWithType(this.getClass());
    private static final String startStr = "@"; 
    @SuppressWarnings("unchecked")
    @Get
    @Post
    public String post(Invocation inv){
        
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        int succ = 0;
        int fail = 1;
        
        /* 搞个form */
        String key = request.getParameter("k");
        if (!"342nvbew4j8xcvww8896n3z-z".equals(key)) {
            JSONObject o = new JSONObject();
            o.put("code", fail);
            o.put("msg", "密钥错误");
            return startStr + o.toString();
        }
        final RegShortCommonForm regForm = new RegShortCommonForm();
        regForm.setSs(request.getParameter("ss"));
        regForm.setAction_id(request.getParameter("action_id"));
        String strAccType = StringUtils.isNotEmpty(request.getParameter("accType")) 
                                && StringUtils.isNumeric(request.getParameter("accType")) 
                                ? request.getParameter("accType")
                                    : "1";
        regForm.setAccType(Integer.parseInt(strAccType));
        regForm.setNicknameId(request.getParameter("nicknameId"));
        regForm.setRegEmail(request.getParameter("regEmail"));
        regForm.setPwd(request.getParameter("pwd"));
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
        if (checkManager.isOK(rtmsg) == false) {
            JSONObject o = new JSONObject();
            o.put("code", fail);
            o.put("msg", rtmsg);
            return startStr + o.toString();
        }else{
            wlogger.debug("wreg check ok");
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
            JSONObject o = new JSONObject();
            o.put("code", fail);
            o.put("msg", "系统错误,请稍候尝试");
            return startStr + o.toString();
        }
        wlogger.debug("wreg create Account ok");
        
        /* 3,激活 */
        User user;
        try {
            user = SnsAdapterFactory.getUserAdapter().get(userId);
            
            
            //xid注册自动激活&登录
            if (user != null 
                    && !StringFormat.isValidEmailAddr(regForm.getRegEmail())) {
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
                
                ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
                wlogger.debug("wreg is xid reg,begin activate user");
                final User wu = user;
                final HttpServletRequest wreq = request;
                final HttpServletResponse wres = response;
                tasksMap.put("activate user", new Thread() {

                    @Override
                    public void run() {
                        ActivateLogic.getInstance().activateUser(wu);
                    }
                });
                tasksMap.put("login", new Thread() {
                    @Override
                    public void run() {
                        LoginLogic.getInstance().doLoginR(wu, wreq,
                                wres, 0, false);
                    }
                });
                //id注册完成后发送单独的notify
                tasksMap.put("xid notify", new Thread() {
                    @Override
                    public void run() {
                        RegNotifyLogic.getInstance().sendXidRegNofify(wu);
                    }
                });
                RunnableTaskExecutor.getInstance().runConcurrentTasks(
                        "short id common", tasksMap);
                wlogger.debug("wreg is xid reg,activate user end");
                JSONObject o = new JSONObject();
                o.put("code", succ);
                o.put("msg", "注册成功,并且激活成功");
                return startStr + o.toString();
            }
        } catch (Exception e) {
        	e.toString();
            JSONObject o = new JSONObject();
            o.put("code", succ);
            o.put("msg", "注册成功");
            return startStr + o.toString();
        }
        JSONObject o = new JSONObject();
        o.put("code", succ);
        o.put("msg", "注册成功");
        return startStr + o.toString();
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
                wlogger.debug(
                        "key: " + k + ",value: " + val);
            }
        }
    }
}
