package com.xiaonei.reg.guide.flows.xfive.logics;

import net.paoding.rose.web.Invocation;

import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.model.AntispamCheckResult;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;

public class GuideXFiveUserNameLogic {
	public static final String ERR_01_NEEDNAME = "1";//"请输入真实姓名";
	public static final String ERR_02_NEEDGD = "2";//"请输入性别";
	public static final String ERR_02_SYSBUSY = "3";//"系统忙，请重试";
	
    private static final String OK = "OK";
    private static final String ERR_NAME_NOT_SAFE = "ERR_NAME_NOT_SAFE";
    private static final String ERR_NAME_NOT_REAL = "ERR_NAME_NOT_REAL";
    
    public static String saveUserNameAndGendar(User host, Invocation inv){
    	String ret = OK;
    	ret = saveUserName(host,inv);
    	if(ret == OK){
    		ret = saveUserGendar(host,inv);
    	}
    	return ret;
    }
    public static String saveUserGendar(User host, Invocation inv) {
    	String usersex = inv.getParameter("user_gendar");
    	GuideLogger.printLog(" host:"+host.getId()+" set sex:"+usersex);
    	String ret = OK;
    	if(usersex == null){
    		GuideLogger.printLog(" host:"+host.getId()+" need not setsex");
    		return ret;
    	}
    	else if(MyStringUtils.contains(usersex, "男")){
            usersex = "男生";
        }
        else{
            usersex = "女生";
        }
		UserBorn uborn = null;
        try {
			uborn = SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" set sex1:"+usersex+" e:"+e.toString(),GuideLogger.ERROR);
		}
		
		if(uborn != null){
			try {
				uborn.setGender(usersex);
				SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), uborn);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" set sex2:"+usersex+" e:"+e.toString(),GuideLogger.ERROR);
			}
		}
    	return ret;
    }
	public static String saveUserName(User host, Invocation inv) {
		String username = inv.getParameter("user_name");
		GuideLogger.printLog(" host:"+host.getId()+" set name:"+username);
		String checkanti = OK;
		try {
            
			if(username == null && !MyStringUtils.isNullOrEmpty(host.getName())){
				GuideLogger.printLog(" host:"+host.getId()+" need not setname");
				return OK;
			}
			else if(username == null && MyStringUtils.isNullOrEmpty(host.getName())){
				GuideLogger.printLog(" host:"+host.getId()+" need set name!",GuideLogger.WARN);
				return ERR_01_NEEDNAME;
			}
			else if(username != null && username.trim().equals("")){
                GuideLogger.printLog(" host:"+host.getId()+" no_name",GuideLogger.WARN);
                return ERR_01_NEEDNAME;
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
            
            if(checkanti != OK){		
                GuideLogger.printLog(" host:"+host.getId()+" anti:"+checkanti+" username:"+username,GuideLogger.WARN);
                return "1";
            }
            else{
                GuideLogger.printLog(" host:"+host.getId()+" atok:"+checkanti+" username:"+username);
            }
            
            UserName uname = null;
            try {
				try {
					uname = SnsAdapterFactory.getUserNameAdapter().getUserName(host.getId());
				} catch (Exception e) {
					GuideLogger.printLog(" host:" + host.getId() + " e:" + e.toString(),GuideLogger.ERROR);
				}

				if (uname == null) {
					GuideLogger.printLog(" host:" + host.getId() + " sys_busy");
					return ERR_02_SYSBUSY;
				} else {
					uname.setName(username);
					SnsAdapterFactory.getUserNameAdapter().setUserName(host.getId(), uname);
				}
			} 
            catch (Exception e) {
				GuideLogger.printLog(" host:" + host.getId() +" set"+ " e:" + e.toString(),GuideLogger.ERROR);
			}
                 
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        return OK;
	}

}
