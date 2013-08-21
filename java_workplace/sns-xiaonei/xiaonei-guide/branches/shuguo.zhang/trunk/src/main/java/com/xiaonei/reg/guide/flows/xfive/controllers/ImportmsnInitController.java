package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

@LoginRequired
@Path("import")
@IgnoreUserStatusImportedValidation
public class ImportmsnInitController {

    private static final String[] maillist = new String[] { "@163.com", "@126.com", "@sina.com",
            "@hotmail.com", "@yahoo.com.cn", "@gmail.com", "@live.cn", "@live.com", "@msn.com",
            "@msn.cn", "@xnmsn.cn", "@yahoo.cn", "@yahoo.com" };

    private static final String SUCC_DEFAULT = "/views/guide/flowxfivedark/importmsn_dark.jsp";
    private static final String INIT_LEAD_AAA = "/views/guide/flowxfivedarkchoco/importmsn_darkchoco.jsp"; //通讯录 曾纯强
    private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/importmsn_darkcoral.jsp"; //口香糖引导
    private static final String INIT_LEAD_AAH = "/views/guide/flowxfivedarkcorn/importmsn_darkcorn.jsp"; //无头带邀请好友引导
    
    @Get
    @Post
    public String get(Invocation inv) {
		String isnew = inv.getParameter("new");
		if("1".equals(isnew)){
			return "f:importnew";
		}
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
        HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
        User host = GuideXFiveRequestUtil.getCurrentHost(request);
        if (request == null || response == null) {
            GuideLogger.printLog(" request or response null " + (request == null)
                    + (response == null), GuideLogger.ERROR);
        } else if (host == null) {
            GuideLogger.printLog(" host null ", GuideLogger.ERROR);
        } else {
            try {
                
                if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
                    GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
                    return "r:http://reg.renren.com/sb/fillemail";
                }
                
                //GuideXFiveUtil.setInitAttributes(request);
                String fromsource = RegFromSourceLogic.getFromSource(host);
                int type = StepProcessLogic.getInstance().getGuideUserType(host,fromsource);
                GuideLogger.printLog(" host:" + host.getId() + " type:" + type);
                request.setAttribute("lead_user_type", type);
                String user_type_string = "___";
                switch (type) {
                    case IGuideUserLeadType.XFiveGeneral:
                        doDefault(request, host);
                        return SUCC_DEFAULT;
                    case IGuideUserLeadType.XFiveBrown:
                        doDefault(request, host);
                        return SUCC_DEFAULT;
                    case IGuideUserLeadType.Lead_AAA:
                    case IGuideUserLeadType.Lead_AAE:
                        doDefault(request, host);
                        return INIT_LEAD_AAA;
                    case IGuideUserLeadType.Lead_AAH:
                    	doDefault(request, host);
                    	return INIT_LEAD_AAH;
                    case IGuideUserLeadType.Lead_AAF:
                    	user_type_string = "AAF";
                    	request.setAttribute("user_type_string",user_type_string);
                    	doDefault(request, host);
                    	return INIT_LEAD_AAF;
                    case IGuideUserLeadType.OldMobile:
                    	user_type_string = "AAO";
                    	request.setAttribute("user_type_string",user_type_string);
                    	doDefault(request, host);
                    	return INIT_LEAD_AAF;
                    case IGuideUserLeadType.Lead_AAP:
                    	user_type_string = "AAP";
                    	request.setAttribute("user_type_string",user_type_string);
                    	doDefault(request, host);
                    	return INIT_LEAD_AAF;
                    case IGuideUserLeadType.Lead_AAQ:
                    	user_type_string = "AAQ";
                    	request.setAttribute("user_type_string",user_type_string);
                    	doDefault(request, host);
                    	return INIT_LEAD_AAF;
                    case IGuideUserLeadType.Lead_AAR:
                    	user_type_string = "AAR";
                    	request.setAttribute("user_type_string",user_type_string);
                    	doDefault(request, host);
                    	return INIT_LEAD_AAF;
                    default:
                        doDefault(request, host);
                        if(type==13&&fromsource.contains(":PVZ:")){
                            return "/views/guide/flowxfivedarkpvz/importmsn_dark.jsp";  
                          }
                        return SUCC_DEFAULT;
                }

            } catch (Exception e) {
                GuideLogger.printLog(" host:" + host.getId() + " err:" + e.toString(),
                        GuideLogger.ERROR);
                e.printStackTrace();
            }
        }

        return SUCC_DEFAULT;
    }

    public static void doDefault(HttpServletRequest request, User host) {

        GuideTimeCost cost = GuideTimeCost.begin();
        GuideLogger.printLog(" host:" + host.getId() + " - start");
        cost.endStep();
        try {
            UserDesc ud = WUserDescAdapter.getInstance().get(host.getId());
            cost.endStep();
            if (null != ud) {
                request.setAttribute("msn", ud.getMsn());
            }
            cost.endStep();
        } catch (Exception e) {
            GuideLogger.printLog(" host:" + host.getId() + " - error", 1);
            e.printStackTrace();
        }
        cost.endStep();
        request.setAttribute("user_email",
                StringFormat.isValidEmailAddr(host.getAccount()) ? StringUtils.trimToEmpty(host
                        .getAccount()) : "");
        request.setAttribute("is_f", StringFormat.isValidEmailAddr(host.getAccount()) ? "true"
                : "false");
        try {
            request.setAttribute("rurl", URLEncoder.encode(Globals.urlGuide + "/imports", "UTF-8"));
            request.setAttribute("ourl", URLEncoder.encode(Globals.urlGuide + "/import", "UTF-8"));
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        //算Form的Tab
        String acct = host.getAccount() == null ? "" : host.getAccount().trim();
        boolean isemail = false;
        String currentmailtail = "";

        for (String mailinlist : maillist) {
            if (MyStringUtils.contains(acct, mailinlist)) {
                isemail = true;
                currentmailtail = mailinlist;
                break;
            }
        }

        if (isemail) {
            request.setAttribute("mailtail", currentmailtail);
            String mailhead = acct;
            int indexofat = acct.indexOf('@');
            if (indexofat >= 0) {
                mailhead = acct.substring(0, indexofat);
            }
            request.setAttribute("mailhead", mailhead);
            request.setAttribute("initform", "webmail");
        } else {
            request.setAttribute("mailhead", "");
            request.setAttribute("mailtail", "");
            request.setAttribute("initform", "msnsend_form");
            if (MyStringUtils.contains(acct, "@qq.com")) {
                request.setAttribute("user_email", "");
            }
        }

        cost.endStep();
        String rt = errNo(request);
        cost.endStep();
        if (null != rt) {
            BaseActionUtil.addErr(request, rt);
        }
        //A090729Util.setInitAttributes(request);
        cost.endStep();
        GuideLogger.printLog(" host:" + host.getId() + "end");
    }

    private static String errNo(HttpServletRequest request) {
        String errNo = request.getParameter("errno");
        if ("1".equals(errNo)) return "密码或帐号错误, 要不要换个帐号试试？";
        if ("2".equals(errNo)) return "密码或帐号错误, 要不要换个帐号试试？";
        return null;
    }

}
