package com.xiaonei.login.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.Path;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.controllers.base.LoginController;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.afterlogin_chains.WeakPasswordEnhenceChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.AccountChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.AdminGroupResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CheckUserStatusResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CrossLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.GameRenrenComChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.IPBindResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SafeCheckResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SaveTicketChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.TaskAfterLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.WeakPasswordChainNode;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.failswitcher.ILoginFailSwitch;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeBanIp;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeDefault;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeInput;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeNoPost;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeNoSafeUser;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeNotifyGrayUser;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUnknown;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUser;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUserActive;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUserBan;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUserNotExist;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeUserSuicide;
import com.xiaonei.login.logic.failswitcher.nodes.weblogin.NodeValidateLogin;

@Path(value = { "plogin.do", "PLogin.do", "Login.do", "login.do" })
@AsSuperController
@DontPutOrigURL
@Singleton
public class WebLoginController extends LoginController {
    @Autowired
    private WeakPasswordEnhenceChainNode weakPasswordEnhenceChainNode;

    @Autowired
    private AccountChainNode accountChainNode;

    @Autowired
    private AdminGroupResultChainNode adminGroupResultChainNode;

    @Autowired
    private CheckUserStatusResultChainNode checkUserStatusResultChainNode;

    @Autowired
    private CrossLoginChainNode crossLoginChainNode;

    @Autowired
    private GameRenrenComChainNode gameRenrenComChainNode;

    @Autowired
    private IPBindResultChainNode iPBindResultChainNode;

    //    @Autowired
    //    private ICodeChainNode iCodeChainNode;

    @Autowired
    private NodeBanIp nodeBanIp;

    @Autowired
    private NodeDefault nodeDefault;

    @Autowired
    private NodeInput nodeInput;

    @Autowired
    private NodeNoPost nodeNoPost;

    @Autowired
    private NodeNoSafeUser nodeNoSafeUser;

    @Autowired
    private NodeNotifyGrayUser nodeNotifyGrayUser;

    @Autowired
    private NodeUnknown nodeUnknown;

    @Autowired
    private NodeUser nodeUser;

    @Autowired
    private NodeUserActive nodeUserActive;

    @Autowired
    private NodeUserBan nodeUserBan;

    @Autowired
    private NodeUserNotExist nodeUserNotExist;

    @Autowired
    private NodeUserSuicide nodeUserSuicide;

    @Autowired
    private NodeValidateLogin nodeValidateLogin;

    @Autowired
    private SafeCheckResultChainNode safeCheckResultChainNode;

    @Autowired
    private SaveTicketChainNode saveTicketChainNode;

    @Autowired
    private TaskAfterLoginChainNode taskAfterLoginChainNode;

    @Autowired
    private WeakPasswordChainNode weakPasswordChainNode;


    @Override
    protected void ahead(final HttpServletRequest request, final HttpServletResponse response) {
        final String loginType = request.getParameter("loginType");
        if (loginType != null) {
            // 保留登录前缀
            request.setAttribute(LoginParamDef.ATTRIBUTE_LOGINTYPE_PRE, loginType);
        }
        request.setAttribute("LOGIN_TYPE", "web");
        request.setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE, LoginSourceType.Web);
    }

    @Override
    protected boolean needNotify() {
        return true;
    }

    @Override
    protected void setUpFailSwitcher(final ILoginFailSwitch failChains) {
        failChains.setUpSwitchNode(this.nodeBanIp);
        failChains.setUpSwitchNode(this.nodeDefault);
        failChains.setUpSwitchNode(this.nodeInput);
        failChains.setUpSwitchNode(this.nodeNoPost);//非POST请求
        failChains.setUpSwitchNode(this.nodeNoSafeUser);//用户安全异常
        failChains.setUpSwitchNode(this.nodeNotifyGrayUser);//灰名单用户
        failChains.setUpSwitchNode(this.nodeUnknown);//未知异常
        failChains.setUpSwitchNode(this.nodeUser);//密码错误异常
        failChains.setUpSwitchNode(this.nodeUserActive);//用户未激活异常
        failChains.setUpSwitchNode(this.nodeUserBan);//用户封禁异常
        failChains.setUpSwitchNode(this.nodeUserNotExist);//用户不存在异常
        failChains.setUpSwitchNode(this.nodeUserSuicide);//用户自杀异常
        failChains.setUpSwitchNode(this.nodeValidateLogin);//验证码错误异常
    }

    @Override
    protected void setUpResultChains(final LoginResultChains resultChains) {
        //resultChains.setNode(this.iCodeChainNode);//验证码
        resultChains.setNode(this.safeCheckResultChainNode);//登陆安全检查，通知安全部门
        resultChains.setNode(this.accountChainNode);//判断是否登陆成功
        resultChains.setNode(this.weakPasswordEnhenceChainNode);
        resultChains.setNode(this.checkUserStatusResultChainNode);//用户status判断
        resultChains.setNode(this.iPBindResultChainNode);//判断指定用户是否在指定IP登陆
        resultChains.setNode(this.adminGroupResultChainNode);//管理员登陆检查
        resultChains.setNode(this.taskAfterLoginChainNode);//登陆后续任务，打印iplog，加登陆积分等
        resultChains.setNode(this.crossLoginChainNode);//跨站登陆提示
        resultChains.setNode(this.weakPasswordChainNode);//若密码登陆通知安全不盆
        resultChains.setNode(this.saveTicketChainNode);//保存p票
        resultChains.setNode(this.gameRenrenComChainNode);//游戏登陆强制跳转
    }

}
