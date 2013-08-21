package com.xiaonei.reg.guide.root.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.root.configs.FlowConfig;
import com.xiaonei.reg.guide.root.configs.StepStatusDefinition;
import com.xiaonei.reg.guide.root.configs.UnitConfig;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.logics.StepStatusLogic;
import com.xiaonei.reg.guide.root.models.Flow;
import com.xiaonei.reg.guide.root.models.Unit;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

@LoginRequired
@IgnoreUserStatusImportedValidation
public class LeadController {
	//private static final String GUIDE_PAGE = "r:guide.do";
	//private static final String LEAD_CONTROLLER = "lead";
    @Get
    @Post
	public String get(Invocation inv){
		//前提：系统会初始化一个<最默认流程>，里面只有一步，是一个默认的模块。
		//　　　目前的<最默认模块>，就是/guide.do
		//　　：系统会初始化一个当前默认流程，里面是配置好的一个流程，由配置文件决定
		//　　：完成标记！会在遇到<endpoint>结点时打上。
		//当一个人进来以后，在不带参数的情况下 需要知道以下几件事情
		//假定：配置文件的编写是完全正确
		//　　　即，Mapping里有的fromsource肯定能取出flowname，这个flowname肯定能取出flow，这flow里的module肯定不空（空对象或无元素）
		//
		
		//１：他现在的状态码，可推知他已经走了哪些模块［DB Status］
		//２：他应该走的流程名称，可推知他要走哪些模块［DB Fromsource］
		//３：他已经走了哪些模块
		//４：他将要走哪些模块
		//５：他下一步去哪个模块
		
		//--start-- 1
		//目前的情况是，进来的时候取，肯定取不着。也就是说，现在为空就是没走过的，需要走的……
		try {
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			HttpServletRequest request = inv.getRequest();
			User host = BaseThreadUtil.currentHost(request);
			int stepstatus = StepStatusLogic.getInstance().getStatusById(host.getId());
			GuideLogger.printLog("| "+host.getId()+" | stepstatus is:"+stepstatus);
			cost.endStep();
			if( isCompleted(stepstatus)){//stepstatus == 0 || 这个条件去掉，因为刚来的人就是0……
				Unit endpoint = UnitConfig.getEndPointUnit();
				GuideLogger.printLog("| "+host.getId()+" | goto:"+endpoint.getUrl());
				cost.endStep();
				cost.endFinally();
				return "r:"+endpoint.getUrl();
			}

			//--start-- 2
			String fromsource = RegFromSourceLogic.getFromSource(host);
			GuideLogger.printLog("| "+host.getId()+" | fromsource:"+fromsource);
			cost.endStep();
			
			if("".equals("")){
			    
			    
			    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
	                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
	                return "r:http://reg.renren.com/sb/fillemail";
	            }
			    
				String ret = StepProcessLogic.getInstance().goWhere(host,fromsource,stepstatus); 
				cost.endStep();    
				cost.endFinally();
				GuideLogger.printLog(" "+host.getId()+" fromsource:"+fromsource+" leadret:"+ret+" Account:"+host.getAccount());
				return ret;
			}
		} catch (Exception e) {
			GuideLogger.printLog("lead error!" + e.toString()+GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		return "r:/guide.do";
		
		
		/*
		//==================================================================================================
		Flow currentflow = null;
		if(MyStringUtils.isNullOrEmpty(fromsource)){
			currentflow = FlowConfig.getConfiguredDefaultFlow();
			GuideLogger.printLog("| "+host.getId()+" | no source - currentflow:"+currentflow.getName());
		}
		
		else{
			Mapping flowmapping = MappingConfig.getMappingByFromsource(fromsource);
			if(flowmapping == null){
				currentflow = FlowConfig.getConfiguredDefaultFlow();
				GuideLogger.printLog("| "+host.getId()+" | mapping is null! return defautl!:"+currentflow.getName());
			}
			else{
				currentflow = MappingConfig.getFlowByFromsource(fromsource); //Maybe get null?!
				GuideLogger.printLog("| "+host.getId()+" | has source - currentflow:"+currentflow.getName());
			}
		}

		//--start-- 3
		Module curm = FlowLogic.getInstance().getNextModule(currentflow, stepstatus);
		if(curm.isAutocomplete()){
			GuideLogger.printLog("| "+host.getId()+" | autocomplete:"+curm.getUnit().getHumanReadableInfo());
			ProcessLogic.getInstance().endTheStep(host, curm.getUnit());
		}
		if(!curm.isAvailable()){
			ProcessLogic.getInstance().endTheStep(host, curm.getUnit());
			return LEAD_CONTROLLER;
		}
		GuideLogger.printLog("| "+host.getId()+" | currentflow:"+currentflow.getName()+" will to:"+curm.getUnitId()+"-"+curm.getUrl());
		return curm.getUrl();
		
		
		//１：一次DB操作取Stepstatus。取不到的，（可以去完结，也可以重做插入！不过重做插入不太好，因为可能删老数据）
		//１：１若Stepstatus标明他COMPLETE过，直接去 <endpoint> －
		//１：２若Stepstatus标明他还没COMPLETE，进行２－
		
		//１：一次DB操作取Fromsource
		//２：１：取Fromsource，可能能取到，可能取出来是空　
		//２：２：如果取出来是空，则打上Log，然后让他去当前默认流程 －
		//２：２：如果取出来不是空，则从取Mapping。ＸＸ里取对应的FlownameＸ
		//２：３：如果取出来是空，则打上log，然后让他去当前默认流程 －  !//移到Logic里去做了！但是是ＴＯＤＯ
		//２：３：如果取出来不是空，则取对应的Flow，肯定能取到Flow － 

		//３：根据Flow和Stepstatus来计算他的下一去向。此时Flow必不为空，Stepstatus为非负整数
		//３：１：运算已经走过的列表A，可能是空，可能非空
		//３：２：运算没走过的列表B，可能是空，可能非空
		//３：３：出现三种情况。A空B不空，B空A不空，AB均不空
		//３：４：取下一步。（查参数）。如果发现是endpoint，则标记COMPLETE。取url
		//
		//跳下一步。
		//
		
		
		//若有参数的怎么办？再说吧……

		 */
	}
	
	private boolean isCompleted(int stepstatus){
		if((stepstatus  & StepStatusDefinition.STEP_COMPLETE_GUIDE) == StepStatusDefinition.STEP_COMPLETE_GUIDE){
			return true;
		}
		return false;
	}

	@SuppressWarnings("unused")
	private Unit getUnitByParam_nu(String unitid) {
		Unit unit = UnitConfig.getUnitById(unitid);
		return unit;
	}

	@SuppressWarnings("unused")
	private Flow getFlowByParam_fid(String flowid) {
		Flow flow = FlowConfig.getFlowById(flowid);
		return flow;
	}
}
