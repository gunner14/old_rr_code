package com.renren.sns.lead.controllers;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.lead.core.LeadCoreLogic;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;


@LoginRequired
@AsSuperController
@IgnoreUserStatusImportedValidation
public abstract class LeadBaseController {
	protected static final String STRING_LEAD_TYPE_FLAG = "lead_type_flag";
	@Get
	@Post
	public String index(Invocation inv,Portal portal,CacheManager cm){
		String ret = "@";
		if(inv == null){
			System.err.print(" inv null!");
		}
		else{
			User host = UserProvider.getInstance().get(cm);
			if(host == null){
				GuideLogger.printLog(" host:null"+" req:"+inv.getRequestPath(),GuideLogger.WARN);
				return ret;
			}
			String skip = inv.getParameter("skip");
			if(MyStringUtils.equals(skip, "1")){
				doSkip(host);
				ret = "r:lead";
			}
			else{
				String old_fromsource = RegFromSourceLogic.getFromSource(host);
				String local_fromsource = LeadCoreLogic.get_fromsource();
				/*if(LeadCoreLogic.get_fromsource()==null){
				}*/
				String lead_flag = LeadCoreLogic.getInstance().getLeadCoreFlag(host, old_fromsource);
				inv.addModel(STRING_LEAD_TYPE_FLAG, lead_flag);
				long begin = System.currentTimeMillis();
				long end = 0;
				try {
					doIndex(inv,portal,host,cm);
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.WARN);
					e.printStackTrace();
				} finally {
					end =  System.currentTimeMillis();
					
				}
				long cost = end - begin;
				AbstractLeadPageMapper mapper = getMapper(host);
				ret = mapper.getThisJspUrl(host, old_fromsource);
				GuideLogger.printLog(" host:"+host.getId()+" lead_type_flag:"+lead_flag+" cost:"+cost+" mp:"+mapper.getClass().getSimpleName()+" lcheck:["+MyStringUtils.equals(old_fromsource, local_fromsource)+"] <"+old_fromsource+","+local_fromsource+"> ret:"+ret);
				LeadCoreLogic.clearThreadLocal();
			}
			//List<String> flist = RegFromSourceLogic.getFromSoureList(fromsource);
			//int stepstatus = getMapper(host).getThisStep();
			
			
		}
		return ret;
	}
	
	
	protected void doingCustomMethod(Invocation inv, Portal portal, User host, CacheManager cm, String initmethodkey) {
		GuideTimeCost tc = GuideTimeCost.begin();
		Method m = null;
		try {
			Class<?> a = this.getClass();
			tc.endStep();
			m =  a.getDeclaredMethod("doing"+initmethodkey,Invocation.class, Portal.class,User.class,CacheManager.class);
			tc.endStep();
		} catch (SecurityException e) {
			GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (NoSuchMethodException e) {
			GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		tc.endStep();
		
		if(m!=null){
			try {
				m.setAccessible(true);
				tc.endStep();
				m.invoke(this, inv,portal,host,cm);
				tc.endStep();
			} catch (IllegalArgumentException e) {
				GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (IllegalAccessException e) {
				GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (InvocationTargetException e) {
				GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" mkey:"+initmethodkey+" mnull",GuideLogger.WARN);
		}
		tc.endStep();
		tc.endFinally();
	}
	
	protected void addWindow(Portal portal, String name, String path) {
        try {
            Window window = portal.addWindow(name, "/window/dataWindows/" + path);
            window.set(Window.FUTURE_CANCEL_ENABLE_ATTR, false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
	
	public void doSkip(User host){
		int thisStep = getMapper(host).getThisStep();
		if(thisStep == StepStatusLogic.STEP_FILLINFO && (host.getName() == null || host.getName().trim().equals(""))){
            GuideLogger.printLog(" host:"+host.getId()+" illskip:"+thisStep,GuideLogger.WARN);
		}
		else{
		    StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		    GuideLogger.printLog(" host:"+host.getId()+" skiped:"+thisStep);
		}
	}
	
	public abstract String doIndex(Invocation inv, Portal portal, User host,CacheManager cm);
	public abstract AbstractLeadPageMapper getMapper(User host);
}
