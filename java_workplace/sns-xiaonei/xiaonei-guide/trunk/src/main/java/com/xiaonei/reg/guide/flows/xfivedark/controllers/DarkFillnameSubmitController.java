package com.xiaonei.reg.guide.flows.xfivedark.controllers;

import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckICode;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckName;
import com.xiaonei.reg.register.model.AntispamCheckResult;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;

/**
 * DarkFillnameSubmitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-6 新建文件
 */
@LoginRequired
@Path({"fillnames"})
@IgnoreUserStatusImportedValidation
public class DarkFillnameSubmitController {
    public final static String SUCC = "f:lead";
    public final static String ERROR = "f:fillinfo";
    
    private static final String OK = "OK";
    private static final String ERR_NAME_NOT_SAFE = "ERR_NAME_NOT_SAFE";
    private static final String ERR_NAME_NOT_REAL = "ERR_NAME_NOT_REAL";
	@Get
	public String get(Invocation inv,CoralFormInfo form){
		return "r:lead";
	}
	@Post
	public String index(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		String ret = SUCC;
		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
		    
			boolean isdone = !MyStringUtils.isNullOrEmpty(host.getName());
			if(isdone){
				GuideLogger.printLog(" host:"+host.getId()+" refillname",GuideLogger.WARN);
			}
			else{
			    boolean cont = true;
			    String checkanti = OK;
			    String username = request.getParameter("username");
			    String usersex = request.getParameter("gender");
			    String useryear = request.getParameter("useryear");
			    String usermonth = request.getParameter("usermonth");
			    String userday = request.getParameter("userday");
			    String icode = request.getParameter("icode");
			    GuideXFiveRequestUtil.setAttribute(request,"username",username);
			    GuideXFiveRequestUtil.setAttribute(request,"gender",usersex);
			    GuideXFiveRequestUtil.setAttribute(request,"useryear",useryear);
			    GuideXFiveRequestUtil.setAttribute(request,"usermonth",usermonth);
			    GuideXFiveRequestUtil.setAttribute(request,"userday",userday);
			    GuideXFiveRequestUtil.setAttribute(request,"icode",icode);
			    //String icode = request.getParameter("icode");
			    GuideLogger.printLog(" host:"+host.getId()+"[username:"+username+" usersex:"+usersex +" "+useryear+"-"+usermonth+"-"+userday+"]"+" icode:"+icode);
			    UserName uname = null;
			    
			    if(cont && needIcode(host)){
			    	String cret = CheckICode.checkICodeByUserid(inv.getRequest(),host.getId());
			    	if(!"OK".equals(cret)){
			    		GuideXFiveRequestUtil.setErrorMsg(request, "验证码错误 ");
			    		GuideLogger.printLog(" host:"+host.getId()+" icode_error"+" cret:"+cret);
                        ret = ERROR;
                        cont = false;
			    	}
			    }
			    
			    if(cont){
                    try{
                        uname = SnsAdapterFactory.getUserNameAdapter().getUserName(host.getId());
                    }
                    catch(Exception e){
                        GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
                    }
                    
                    if(uname ==null){
                        GuideXFiveRequestUtil.setErrorMsg(request, "系统忙，请稍后再试");
                        GuideLogger.printLog(" host:"+host.getId()+" sys_busy");
                        ret = ERROR;
                        cont = false;
                    }
                    
                    //inshion
                    if(CheckName.containsIllegalChars(username)){
                    	GuideLogger.printLog(" host:"+host.getId()+" illchar "+" username:"+username);
                    	GuideXFiveRequestUtil.setErrorMsg(request, IRegCheckCodeDef.ERR_NAME_ILLEGLE_CHAR);
                        ret = ERROR;
                        cont = false;
                    }
			    }

			    if(cont){//处理username
			        GuideLogger.printLog(" host:"+host.getId()+" proc username");
			        try {
			            
                        if(MyStringUtils.isNullOrEmpty(username)){
                            GuideLogger.printLog(" host:"+host.getId()+" no_name");
                            GuideXFiveRequestUtil.setErrorMsg(request, "请输入真实姓名");
                            ret = ERROR;
                            cont = false;
                        }
                        else{
                            AntispamCheckResult cr = null;
                            try {
                                cr = AntispamCheckResult.cover(AntiSpamHome.getInstance().checkRealName(0, username));
                            } catch (final Throwable e) {
                                GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
                                checkanti = OK;
                            }
                            if(null == cr){
                                checkanti = OK;
                            }
                            else if (cr.getM_type() == CheckResult.PROHIBITED) {
                                checkanti =  ERR_NAME_NOT_SAFE;
                            } else if (cr.getM_type() == CheckResult.SYSTEM_ERROR) {
                                checkanti =  OK;
                            } else if (cr.getM_type() == CheckResult.NEED_REAL_NAME) {
                                checkanti =  ERR_NAME_NOT_REAL;
                            }
                        }
                        
                        if(cont){
                            if(checkanti != OK){		
                                GuideLogger.printLog(" host:"+host.getId()+" anti:"+checkanti+" username:"+username);
                                GuideXFiveRequestUtil.setErrorMsg(request, "请重新输入真实姓名");
                                ret = ERROR;
                                cont = false;
                            }
                            else{
                                //will save but change to bottom
                                GuideLogger.printLog(" host:"+host.getId()+" atok:"+checkanti+" username:"+username);
                                
                            }
                        }
                    } catch (Exception e) {
                        GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
                        e.printStackTrace();
                    }
			    }
			    
			    if(cont){//处理性别
			        GuideLogger.printLog(" host:"+host.getId()+" proc usersex");
			        try {
                        if(MyStringUtils.isNullOrEmpty(usersex)){
                            GuideLogger.printLog(" host:"+host.getId()+" no_sex");
                            GuideXFiveRequestUtil.setErrorMsg(request, "请选择性别");
                            ret = ERROR;
                            cont = false;
                        }
                        else{
                            if(cont){
                                if(MyStringUtils.isNullOrEmpty(useryear) ||MyStringUtils.isNullOrEmpty(usermonth) ||MyStringUtils.isNullOrEmpty(userday)){
                                    GuideLogger.printLog(" host:"+host.getId()+" no_birth");
                                    GuideXFiveRequestUtil.setErrorMsg(request, "请选择生日");
                                    ret = ERROR;
                                    cont = false;
                                }
                                
                                if(cont){
                                    try {
                                        uname.setName(username);
                                        SnsAdapterFactory.getUserNameAdapter().setUserName(host.getId(), uname);
                                        cont = true;
                                    } catch (Exception e) {
                                        GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
                                        cont = false;
                                    }
                                    
                                    if(MyStringUtils.contains(usersex, "男")){
                                        usersex = "男生";
                                    }
                                    else{
                                        usersex = "女生";
                                    }
                                    UserBorn uborn = SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
                                    uborn.setGender(usersex);
                                    int birthYear = MyStringUtils.stringToInt(useryear);
                                    if(birthYear == -1 || birthYear == 0){
                                        birthYear = getCurrentYear() - 25;
                                    }
                                    uborn.setBirthYear(birthYear);
                                    int birthMonth = MyStringUtils.stringToInt(usermonth);
                                    if(birthMonth == 0|| birthYear == -1){
                                        birthMonth = 1;
                                    }
                                    uborn.setBirthMonth(birthMonth);
                                    int birthDay = MyStringUtils.stringToInt(userday);
                                    if(birthDay == 0|| birthDay == -1){
                                        birthDay = 1;
                                    }
                                    uborn.setBirthDay(birthDay);
                                    SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), uborn);
                                }
                            }
                        }
                    } catch (Exception e) {
                        GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
                    }
			    }
			    
			    GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
			}
		}
		if("l".equals(request.getParameter("f")) && SUCC.equals(ret)){
			ret = "r:lead";
		}
		return ret;
	}
	
	private boolean needIcode(User host) {
		boolean ret = true;
		String nocode = DodoyoResource.getString("renren.lead.no.icode.imopfillname","pageids" );
		GuideLogger.printLog(" host:"+host.getId()+" need code nocode:"+nocode);
		if("true".equals(nocode)){
			ret = false;
		}
		return ret;
	}
	private int getCurrentYear(){
		int ret = 0;
		try {
			Date currenttime = new Date();
			SimpleDateFormat formatyear = new SimpleDateFormat("yyyy");
			String year = formatyear.format(currenttime);
			ret = Integer.parseInt(year);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return ret;
	}

	public static void main(String[] args) {
		Date currenttime = new Date();
		SimpleDateFormat formatyear = new SimpleDateFormat("yyyy");
		String year = formatyear.format(currenttime);
		System.out.println(year);
		
	}
}
