package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.NetworkBigObjectN;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONObject;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.component.BaseDatum;
import com.dodoyo.datum.component.DatumDispatcher;
import com.dodoyo.datum.component.NoBigObjectDatum;
import com.renren.sns.lead.dataproviders.util.StatLogger;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

/**
 * FillinfoInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-8-30 添加 SUCC_DARKBROWN
 * 2010-9-6 添加 SUCC_DARKBROWN_FILLNAME
 */
@LoginRequired
@Path({"fillinfo","fiblue"})
@IgnoreUserStatusImportedValidation
public class FillinfoInitController {
	private static final String SUCC_DEFAULT = "/views/guide/flowxfivedarkblue/fillinfo_darkblue.jsp";
	//private static final String SUCC_BROWN = "/views/guide/flowxfivebrown/fillinfo.jsp";
	private static final String SUCC_CRIMSON = "/views/guide/flowxfivecrimson/fillinfo.jsp";
	private static final String SUCC_CORAL = "/views/guide/flowxfivecoral/fillinfo.jsp";
	private static final String SUCC_CORALB = "/views/guide/flowxfivecoral/fillinfo_useicon.jsp";
	private static final String SUCC_DARKBLUE = "/views/guide/flowxfivedarkblue/fillinfo_darkblue.jsp";
	private static final String SUCC_DARKBROWN = "/views/guide/flowxfivedarkbrown/fillinfo_darkbrown.jsp"; //2010新生
	//private static final String SUCC_DARKBROWN_FILLNAME = "/views/guide/flowxfivedarkbrown/fillname_darkbrown.jsp"; //IMOP
	private static final String INIT_LEAD_AAA = "/views/guide/flowxfivedarkchoco/fillinfo_darkchoco.jsp"; //通讯录 曾纯强
	private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/fillinfo_darkcoral.jsp"; //口香糖引导
	private static final String INIT_LEAD_AAH = "/views/guide/flowxfivedarkcorn/fillinfo_darkcorn.jsp"; //无头带邀请好友引导
	@Get
	@Post
	public String index(Invocation inv){
		String isnew = inv.getParameter("new");
		if("1".equals(isnew)){
			return "f:fillinfonew";
		}
		
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{	
			try {
				//GuideXFiveUtil.setInitAttributes(request);
				//传IP
				try {
					String ip = BizFilterUtil.getClientIP(request);
					com.xiaonei.xce.friendfinderbyip.FriendFinderByIPAdapter.getInstance().accessIPStr(host.getId(),ip);
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" getClientIP failed! e:"+e.toString(),GuideLogger.ERROR);
				}
				String fromsource = RegFromSourceLogic.getFromSource(host);
				int type = StepProcessLogic.getInstance().getGuideUserType(host,fromsource);
				GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
				request.setAttribute("lead_user_type", type);
				switch(type){
				case IGuideUserLeadType.XFiveGeneral:
				case IGuideUserLeadType.XFiveBrown:
					return SUCC_DEFAULT;
				case IGuideUserLeadType.Netbar:
				case IGuideUserLeadType.XFiveNetbar:
					return SUCC_CRIMSON;
				case IGuideUserLeadType.XFiveCoral:
					return SUCC_CORAL;
				case IGuideUserLeadType.XFiveCoralB:
					return SUCC_CORALB;
				case IGuideUserLeadType.XFiveDarkBlue:
					return SUCC_DARKBLUE;
				case IGuideUserLeadType.XS2010:
				    return SUCC_DARKBROWN;
				case IGuideUserLeadType.IMopFirst:
				    
				    int status=host.getStatus();
		            if(status==10){
		                GuideLogger.printLog(" host:"+host.getId()+"imop to register system");
		                return "r:http://reg.renren.com/sb/fillemail";
		            }
				    /*if(MyStringUtils.isNullOrEmpty(host.getName())){
				        GuideLogger.printLog(" host:"+host.getId()+" to_fillname");
				        return SUCC_DARKBROWN_FILLNAME;
				    }*/
				    return SUCC_DEFAULT;
                 case IGuideUserLeadType.Com56First:
                    
                    int com56status=host.getStatus();
                    if(com56status==13){
                        GuideLogger.printLog(" host:"+host.getId()+"56.com to register system");
                        return "r:http://reg.renren.com/sb/wuliuentry";
                    }
                    /*if(MyStringUtils.isNullOrEmpty(host.getName())){
                        GuideLogger.printLog(" host:"+host.getId()+" to_fillname");
                        return SUCC_DARKBROWN_FILLNAME;
                    }*/
                    return SUCC_DEFAULT;
				case IGuideUserLeadType.Lead_AAA:
				case IGuideUserLeadType.Lead_AAE:
				    return INIT_LEAD_AAA;
				case IGuideUserLeadType.Lead_AAF:
					StatLogger.info(host.getId(), ""+"type"+":"+type+""+" stage:"+host.getStage(),StatLogger.S_ARV_ENTERINTOO_FI_,StatLogger.TYPE_AAF);
					doInitStage(request,host);
					//doInitDatum(request,host);
					return INIT_LEAD_AAF;
				case IGuideUserLeadType.OldMobile:
					StatLogger.info(host.getId(), ""+"type"+":"+type+""+" stage:"+host.getStage(),StatLogger.S_ARV_ENTERINTOO_FI_,StatLogger.TYPE_AAO);
					doInitStage(request,host);
					//doInitDatum(request,host);
					return INIT_LEAD_AAF;
				case IGuideUserLeadType.Lead_AAH:
					return INIT_LEAD_AAH;
				case IGuideUserLeadType.Lead_AAP:
					StatLogger.info(host.getId(), ""+"type"+":"+type+""+" stage:"+host.getStage(),StatLogger.S_ARV_ENTERINTOO_FI_,StatLogger.TYPE_AAP);
					doInitStage(request,host);
					return INIT_LEAD_AAF;
				case IGuideUserLeadType.Lead_AAQ:
					StatLogger.info(host.getId(), ""+"type"+":"+type+""+" stage:"+host.getStage(),StatLogger.S_ARV_ENTERINTOO_FI_,StatLogger.TYPE_AAQ);
					doInitStage(request,host);
					return INIT_LEAD_AAF;
				case IGuideUserLeadType.Lead_AAR:
					StatLogger.info(host.getId(), ""+"type"+":"+type+""+" stage:"+host.getStage(),StatLogger.S_ARV_ENTERINTOO_FI_,StatLogger.TYPE_AAR);
					doInitStage(request,host);
					return INIT_LEAD_AAF;
				default:
				    if(type==13&&fromsource.contains(":PVZ:")){
				      return "/views/guide/flowxfivedarkpvz/fillinfo_darkblue.jsp";  
				    }
					return SUCC_DEFAULT;
				}
				
			} catch (Exception e) {
				GuideLogger.printLog("setInitAttributes failed! err "+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			return SUCC_DEFAULT;
		}
		return SUCC_DEFAULT;
	}
	private void doInitStage(HttpServletRequest request, User host) {
		if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
		}
		else{
			try {
				request.setAttribute("lead_fillinfo_stage", host.getStage());
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString()+" ",GuideLogger.ERROR);
			}
		}
	}
	@SuppressWarnings("unused")
	private void doInitDatum(HttpServletRequest request,User host) {
		String att = "";
		if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
		}
		else{
			GuideTimeCost cost = GuideTimeCost.begin();
			try {
				NetworkBigObjectN nbn = DatumInfoClient.getInstance().getNetworkBigObject(host.getId());
				JSONObject joDatum = new JSONObject();
				
				int cnt = 0;
				for (Entry<String, BaseDatum> entry : DatumDispatcher.getInstance().getDatumMap().entrySet()) {
					final String key = entry.getKey();
					final BaseDatum datum = entry.getValue();
					if (datum instanceof NoBigObjectDatum) {
						joDatum.put(key, ((NoBigObjectDatum) datum).display(host.getId()));
					} else {
						joDatum.put(key, datum.diplay(nbn));
					}
					cnt ++ ;
				}
				att = joDatum.toString();
				GuideLogger.printLog(" host:"+host.getId()+" get_datum:"+att.length());
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString()+" ",GuideLogger.ERROR);
			}
			cost.endFinally();
		}
		request.setAttribute("datum_init", att);
		
	}
}
