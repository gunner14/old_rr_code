package com.xiaonei.platform.core.opt.base.action;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.sql.SQLException;
import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.MD5;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.xce.log.TimeCost;

/**
 * 此Action的设计意图：如果操作是需要登录后才能进行的，请继承此Action<br>
 * 此Action发现用户未登录，将引导用户去登录页，
 * 
 * @author
 */
public abstract class ProtectAction extends BaseAction {

    private final Log ProtectLogger = LogFactory.getLog("core");

    public abstract ActionForward executePt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException;

    @Override
    public ActionForward executeBt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException {
        // 场景：例如外站提供一个发日志功能，提交到 http://blog.xiaonei.com/NewEntry.do
        TimeCost timeLog = TimeCost.begin(ProtectLogger, "ProtectAction");
        if ("post".equalsIgnoreCase(request.getMethod())) {
            String referer = request.getHeader("Referer");
            if (logger.isInfoEnabled()) {
                logger.info("-------------- ProtectAction【 referer 】" + referer);
            }
            timeLog.endStep("Check Post and get Referer");
            if (referer != null && referer.trim().length() > 0) {
                URL url = null;
                try {
                    referer = referer.replaceAll("#", "");
                    url = new URL(referer);
                    timeLog.endStep("new url");
                } catch (MalformedURLException e) {
                    timeLog.endException(e);
                    e.printStackTrace(System.err);
                    return mapping.findForward("forlogin");
                }
                if (url != null) {
                    String hosturl = url.getHost().toLowerCase();
                    if (logger.isInfoEnabled()) {
                        logger.info("-------------- ProtectAction【 hosturl 】" + hosturl);
                    }
                    timeLog.endStep("getHostUrl");
                    if (hosturl.endsWith(OpiConstants.domainMain.toString())
                            || hosturl.endsWith(OpiConstants.domainStatic.toString())
                            || hosturl.endsWith("5q.com")) {
                        timeLog.endStep("check referer");
                    } else {
                        // 这个改造成redirect 到 home的方式
                        try {
                            response.sendRedirect(OpiConstants.urlHome + "/Home.do");
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        timeLog.endStep("sendRedirect to Home.do");
                        // return mapping.findForward("home");
                        return null;
                    }
                }
            } else {
                // 没有referer的情况，也需要处理，暂时还没有处理，
                // 发生了一种情况：学校资料页，先保存到本地，再用这个页提交，虽然没有referer但是可以提交成功，这是个漏洞
                timeLog.endStep("noreferer");
            }
            request.setAttribute("isSubmit", "true");
        }
        timeLog.endStep("after post check");
        User host = BaseThreadUtil.currentHost(request);
        // 用户在未登录的情况下提交
        if (host == null) {
            if ("post".equalsIgnoreCase(request.getMethod())) {
                request.setAttribute("method", "POST");
                // request.getSession().setAttribute("form", form);
                Enumeration en = request.getAttributeNames();
                timeLog.endStep("get AttributeNames");
                while (en.hasMoreElements()) {
                    String formName = (String) en.nextElement();
                    if (formName.indexOf("Form") > -1) {
                        request.setAttribute("formName", formName);
                    }
                }
                timeLog.endStep("set formName");
            } else {
                request.setAttribute("method", "GET");
            }

            if (isFromWap(response, request)) {
                // addErr(request.getSession(), "为保护同学隐私，登录用户才能查看具体信息");
                // 这个改造成redirect
                try {
                    response.sendRedirect(OpiConstants.urlMobile + "/whome.do");
                } catch (IOException e) {
                    e.printStackTrace();
                }
                timeLog.endStep("send Redirect to whome.do");
                timeLog.endFinally();
                return null;
                // return mapping.findForward("forwaplogin");
            }
            // 新的逻辑 add by lifs
            try {
                String url = getOrigURL(request);
                timeLog.endStep("getOrigURL");
                response.sendRedirect(OpiConstants.urlLogin + "/Login.do?rf=r&origURL=" + url);
                timeLog.endFinally();
                return null;
            } catch (IOException e) {
                e.printStackTrace();
            }
            // return mapping.findForward("forlogin");
            timeLog.endFinally();
            return null;
        }

        // for 黄欢
        if (host != null) {
            String xeferer = request.getHeader("Xeferer");
            if (ProtectLogger.isDebugEnabled()) {
                ProtectLogger.debug("httpHeader.Xeferer=" + xeferer + "; hostId=" + host.getId());
            }
            if (xeferer != null && xeferer.length() == 32) {
                // "1e4548100fa84a18b070b4b628e323e1"此常量来自黄欢
                String result = MD5.digest(host.getId() + "1e4548100fa84a18b070b4b628e323e1");
                if (ProtectLogger.isDebugEnabled()) {
                    ProtectLogger.debug("expected Xeferer result: " + result + "   "
                            + (xeferer.equals(result) ? "ok" : "fail"));
                }
                request.setAttribute("isFromIM", xeferer.equals(result) ? Boolean.TRUE
                        : Boolean.FALSE);
            } else {
                request.setAttribute("isFromIM", Boolean.FALSE);
                if (ProtectLogger.isDebugEnabled()) {
                    ProtectLogger.debug("xeferer is null or illegel: xeferer=" + xeferer);
                }
            }
        }

        timeLog.endStep("executePt start");
        ActionForward af = executePt(mapping, form, request, response);
        timeLog.endStep("executePt end");
        timeLog.endFinally();
        return af;
    }

    public static void main(String[] args) {
        System.out.println("=========");
        //        System.out.println("===" + MD5.digest("asasdfasdfd"));
    }

    private String getOrigURL(HttpServletRequest request) {
        String query = request.getQueryString();
        String uri = request.getRequestURI();
        String domain = request.getServerName();
        String URL = request.getRequestURL().toString();
        try {
            String url = "http://" + domain + uri;
            if (query != null) {
                url += "?" + query;
            }
            String encodeUrl = URLEncoder.encode(url, "UTF-8");
            if (logger.isInfoEnabled()) {
                logger.info("-------------- ProtectAction【query】=" + query);
                logger.info("-------------- ProtectAction【uri】=" + uri);
                logger.info("-------------- ProtectAction【domain】=" + domain);
                logger.info("-------------- ProtectAction【URL】=" + URL);
                logger.info("-------------- ProtectAction encode 【url】=" + encodeUrl);
            }
            return encodeUrl;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
