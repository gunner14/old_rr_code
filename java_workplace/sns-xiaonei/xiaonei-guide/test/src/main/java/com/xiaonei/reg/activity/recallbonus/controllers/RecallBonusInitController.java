package com.xiaonei.reg.activity.recallbonus.controllers;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import Ice.TimeoutException;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.recallbonus.constant.IRecallBonusConst;
import com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAccountLogic;
import com.xiaonei.reg.activity.recallbonus.logic.RecallBonusAwardLogic;
import com.xiaonei.reg.activity.recallbonus.logic.RecallBonusInviteLogic;
import com.xiaonei.reg.activity.recallbonus.logic.RecallBonusObjectLogic;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAwardModel;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusInviteModel;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;


@LoginRequired
@Path("rcbonus")
public class RecallBonusInitController {
	private static final String TO_INIT = "/views/activity/recallbonus/recallbonus.jsp";
	private static final String TO_DEBUG_MAIN = "r:/views/activity/recallbonus/d_rc_main.jsp";
	private static final String AJAX_RETURN = "@";
	private static final String ACTION_INIT = "init";
	@Get
	@Post
	public String index(Invocation inv){
		
		//看操作
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
			addModel(inv,"rc_user_host",""+host.getId());
			String action = GuideXFiveRequestUtil.getParamString(request, "a", host.getId());
			if(MyStringUtils.isNullOrEmpty(action)){
				action = ACTION_INIT;
			}
			
			GuideLogger.printLog(" host:"+host.getId()+" a:"+action);
			if(MyStringUtils.equals("init", action)){//init
				doInit(inv,request,host); //2010-3-24 16:37:45 改过了，加钱的逻辑已变化 
				
				return TO_INIT;
			}
			else if(MyStringUtils.equals("invite", action)){//invite
				doInvite(request,response,host); //2010-3-24 16:38:01 这个不用改
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("invitefriends", action)){//invite
				//邀请好友
				
				//取参数列表
				//写邀请表
				doInviteSubmit(request,response,host); //2010-3-24 16:38:43 这个改过了，不加钱了！
				
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("get", action)){//get
				//抽奖
				doGetBonus(request,response,host);
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("accept", action)){//accept
				//被邀请
				doAccept(request,response,host); //这个不用改，要加3个钱
				doInit(inv,request,host);
				return TO_INIT;
			}
			else if(MyStringUtils.equals("getlink", action)){//accept
				//取连接
				doGetLink(request,response,host);
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("debug", action)){
				if(Globals.DEBUG_ENV){
					return TO_DEBUG_MAIN;
				}
			}
			
		}
		
		return "@";
	}
	
	
	private void doInvite(HttpServletRequest request,HttpServletResponse response, User host) {
		// TODO Auto-generated method stub
		if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
			return ;
		}
		
		List<Integer> friends = null;
		try {
			//friends = FriendsHome.getInstance().getFriends(host.getId());
			friends = FriendsHome.getInstance().getFriendsIds(host.getId());
			//找出今天已经发过的人
			List<RecallBonusInviteModel> todaylist = RecallBonusInviteLogic.getInstance().getByInviterIdAndToday(host);
			List<Integer> todaylist_int = new ArrayList<Integer>();
			if(todaylist != null){
				for (RecallBonusInviteModel m : todaylist) {
					todaylist_int.add(m.getInvitee_id());
				}
			}
			friends.removeAll(todaylist_int);
			
		} catch (TimeoutException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IndexOutOfBoundsException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch(Exception e){
			e.printStackTrace();
		}
		
		String ret = "";
		if( friends != null ){
			ret = GuideUtil.getJSONForFriendSelectorFromIds(host.getId(),friends);
		}
		GuideXFiveRequestUtil.writeAjax(response, ret);
		
	}


	private void doGetLink(HttpServletRequest request,	HttpServletResponse response, User host) {
		final String urlguide = Globals.urlGuide.toString();
		String link = RecallBonusInviteLogic.getInstance().getInviteeLink(host.getId(),urlguide);
		GuideLogger.printLog(" host:"+host.getId()+" link:"+link);
		
	}


	private void doAccept(HttpServletRequest request, HttpServletResponse response, User host) {
		String ids = GuideXFiveRequestUtil.getParamString(request, "ids", host.getId());
		if (ids != null && !"".equalsIgnoreCase(ids)) {//有参数
			String [] strs_id = ids.split("_");
			List<Integer> ilist = new ArrayList<Integer>();
			if(strs_id != null){//有列表
				for (String sid : strs_id) {
					int id = 0;
					try {
						id = Integer.parseInt(sid.trim());
					} catch (Exception e) {
						GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
						e.printStackTrace();
					}
					if(id != 0){
						ilist.add(id);
					}
				}
			
				//给谁加过钱先记下来
				List<Integer> donelist = new ArrayList<Integer>(); 
				
				//给他们加钱
				for (Integer id : ilist) {
					RecallBonusInviteModel invitem = RecallBonusInviteLogic.getInstance().getById(id);
					if(invitem == null){
						GuideLogger.printLog(" host:"+host.getId()+" inviter:"+id+" ",GuideLogger.WARN);
						continue;
					}
					else{
						if(invitem.getInvitee_id() == host.getId()){
							if(invitem.getAccept_flag() == 0){
								GuideLogger.printLog(" host:"+host.getId()+" inviter:"+invitem.getInviter_id()+" id:"+id,GuideLogger.WARN);
								RecallBonusAccountLogic.getInstance().doAcceptedIncreament(host, invitem.getInviter_id(), IRecallBonusConst.AMOUNT_ACCEPT_INCREMENT);
								donelist.add(id);
							}
							else{
								GuideLogger.printLog(" host:"+host.getId()+" inviter:"+invitem.getInviter_id()+" "+" invitem.getAccept_flag()="+invitem.getAccept_flag()+" id:"+id,GuideLogger.WARN);
							}
						}
						else{
							GuideLogger.printLog(" host:"+host.getId()+" != invitee:"+invitem.getInvitee_id()+" id:"+id,GuideLogger.WARN);
						}
					}
				}
				
				//我host接受邀请，最后再把记下来的id们更新一下，使链接失效
				RecallBonusInviteLogic.getInstance().acceptBatch(host, donelist, host.getId());
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" ",GuideLogger.WARN);
			}
			
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" ",GuideLogger.WARN);
		}
		
	}

	private boolean isInBonus50Time(User host){
		Date today = new Date();
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
	    long now = today.getTime();
	    long from = today.getTime();
	    long to = today.getTime();
		try {
			from = df.parse("2010-3-27 00:00:00").getTime();
			to = df.parse("2010-3-27 23:59:59").getTime();
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		boolean ret = false; 
		if( now > from && now < to ){
			ret = true;
		}
		else{
			ret = false;
		}
		//GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret+" f:"+from+" t:"+to+" n:"+now );
		return ret;
	}

	private void doGetBonus(HttpServletRequest request,	HttpServletResponse response, User host) {
		//int bonusid = GuideXFiveRequestUtil.getParamInteger(request, "bid", host.getId());
		//2010年4月1日15:24:51 所有奖品下线
		//if( 1==1 || bonusid <= 1 || bonusid >= 12){ //id错了
		if(true){
			GuideLogger.printLog(" host:"+host.getId()+" code:"+5);
			GuideXFiveRequestUtil.writeAjax(response, "5#您选择的奖品号不存在。#"+0);
		}
		/*else{
			RecallBonusObjectModel bonusinfo = RecallBonusObjectLogic.getInstance().getBounusInfo_FromMem(host, bonusid);
			if(bonusinfo == null){
				GuideLogger.printLog(" host:"+host.getId()+" binfo null!");
			}
			else{
				int need = bonusinfo.getBonus_cost();
				int current = RecallBonusAccountLogic.getInstance().getAccountCurrent(host);
				if(current < need){//分不够  3
					GuideLogger.printLog(" host:"+host.getId()+" NEEDCURRENT:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" current:"+current,GuideLogger.WARN);
					GuideXFiveRequestUtil.writeAjax(response, "3#您的抽奖豆不够了哦~立即通过邀请好友的方式获得吧:)！#"+0);
				}
				else{//分够
					if(bonusinfo.getBonus_id() == 11 ){
						//看是不是在抽奖时段
						boolean isinbonustime = isInBonus50Time(host);
						//看看还有没有剩的
						int remain = doQueryBonusRemain(11);
						
						if(remain <= 0){
							GuideLogger.printLog(" host:"+host.getId()+" NOREMAIN:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+remain+"]",GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "5#对不起，这个小时的100张电话卡已经被抽取一空，下个小时再来试试手气吧！#"+0);
						}
						else{
							if(isinbonustime){//是时段里
								//看是不是已经抽过了
								boolean alreadyget = RecallBonusAccountLogic.getInstance().alreadyDoDailyRecordByType(host, IRecallBonusConst.ACCOUNT_RECORD_TYPE_0327FLAG);
								//int days = GuideUtil.getKeepLoginDayFrom20100324(host);
								//GuideLogger.printLog(" host:"+host.getId()+" days:"+days);
								if( !alreadyget ){//有资格
									//看概率                      10000000
									RecallBonusAccountLogic.getInstance().doDailyRecordByType(host,IRecallBonusConst.ACCOUNT_RECORD_TYPE_0327FLAG,0);
									int ratenumber = 5000000;//10000000
									if(lessThanPoint(ratenumber)){//中概率，判断有没有
										GuideLogger.printLog(" host:"+host.getId()+" HIT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+ratenumber+","+current+"]",GuideLogger.WARN);
										int getsucc = RecallBonusObjectLogic.getInstance().increaseBonus(host, bonusid);
										if(getsucc == 1){//真中了 2
											int insertret = RecallBonusAwardLogic.getInstance().insert(host,bonusinfo.getBonus_id(),bonusinfo.getBonus_name());
											GuideLogger.printLog(" host:"+host.getId()+" SUCC!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" insert:"+insertret,GuideLogger.WARN);
											GuideXFiveRequestUtil.writeAjax(response, "5#您已经抽中了30元移动充值卡一张，我们将在五个工作日赠送奖品或联系你，我们核对信息以后将尽快给你发放奖品。:)"+"#"+(need*-1));
										}
										else{//没有了 4
											GuideLogger.printLog(" host:"+host.getId()+" NOLEFT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
											GuideXFiveRequestUtil.writeAjax(response, "5#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得抽奖豆，抽取别的奖品试试手气吧。#"+(need*-1));
										}
									}
									else{//没中 1
										GuideLogger.printLog(" host:"+host.getId()+" MISS:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+ratenumber+","+current+"]",GuideLogger.WARN);
										GuideXFiveRequestUtil.writeAjax(response, "1#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得抽奖豆，抽取别的奖品试试手气吧。#"+(need*-1));
									}
								}
								else{//没资格
									GuideLogger.printLog(" host:"+host.getId()+" NORIGHT:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+alreadyget+"]",GuideLogger.WARN);
									GuideXFiveRequestUtil.writeAjax(response, "5#对不起，你今天已经抽过奖了，该奖品每人当天只有一次抽取机会，敬请期待期待我们的后续活动吧。#"+0);
								}
								
							}
							else{//不是时间
								GuideLogger.printLog(" host:"+host.getId()+" NOTIME:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+isinbonustime+"]",GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "5#对不起，抽奖时间为2010年3月27日。#"+0);
							}
						}
					}
					else {
						int ratenumber = bonusinfo.getBonus_rate();
						if(bonusinfo.getBonus_id() == 10 && current >= 100) {
							ratenumber = ratenumber / current;
						}
						//先扣分
						RecallBonusAccountLogic.getInstance().doPayDecreament(host, need);
						if(lessThanPoint(ratenumber)){//中概率，判断有没有
							GuideLogger.printLog(" host:"+host.getId()+" HIT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+ratenumber+","+current+"]",GuideLogger.WARN);
							int getsucc = RecallBonusObjectLogic.getInstance().increaseBonus(host, bonusid);
							if(getsucc == 1){//真中了 2
								int insertret = RecallBonusAwardLogic.getInstance().insert(host,bonusinfo.getBonus_id(),bonusinfo.getBonus_name());
								GuideLogger.printLog(" host:"+host.getId()+" SUCC!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" insert:"+insertret,GuideLogger.WARN);
								if(bonusinfo.getBonus_id() == 10){
									GuideXFiveRequestUtil.writeAjax(response, "2#你已经抽中了10元移动充值卡一张，我们将在两个工作日内给您发送一封站内信，请注意查收并回复。我们核对信息以后将尽快给您发放奖品。:)"+"#"+(need*-1));
								}
								else{
									GuideXFiveRequestUtil.writeAjax(response, "2#"+bonusinfo.getBonus_name()+"#"+(need*-1));
								}
							}
							else{//没有了 4
								GuideLogger.printLog(" host:"+host.getId()+" NOLEFT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "4#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得150抽奖豆，每天都试试手气吧。#"+(need*-1));
							}
							
						}
						else{//没中 1
							GuideLogger.printLog(" host:"+host.getId()+" MISS:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+"["+ratenumber+","+current+"]",GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "1#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得150抽奖豆，每天都试试手气吧。#"+(need*-1));
						}
					}
				}
				
			}
			
			
		}*/
		
	}

	
	private int doQueryBonusRemain(int i) {
		int ret = 0;
		RecallBonusObjectModel bonus = RecallBonusObjectLogic.getInstance().selectById(11);
		if(bonus != null){
			ret = bonus.getBonus_max() - bonus.getBonus_count();
		}
		return ret;
	}


	@SuppressWarnings("unused")
	private void doGetBonus_old(HttpServletRequest request,	HttpServletResponse response, User host) {
		int bonusid = GuideXFiveRequestUtil.getParamInteger(request, "bid", host.getId());
		if(bonusid == 0 || bonusid > 9){ //id错了
			GuideLogger.printLog(" host:"+host.getId()+" code:"+5);
			GuideXFiveRequestUtil.writeAjax(response, "5#您选择的奖品号不存在。#"+0);
		}
		else{
			RecallBonusObjectModel bonusinfo = RecallBonusObjectLogic.getInstance().getBounusInfo_FromMem(host, bonusid);
			if(bonusinfo == null){
				GuideLogger.printLog(" host:"+host.getId()+" binfo null!");
			}
			else{
				int need = bonusinfo.getBonus_cost();
				int current = RecallBonusAccountLogic.getInstance().getAccountCurrent(host);
				if(current < need){//分不够  3
					GuideLogger.printLog(" host:"+host.getId()+" NEEDCURRENT:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" current:"+current,GuideLogger.WARN);
					GuideXFiveRequestUtil.writeAjax(response, "3#您的抽奖豆不够了哦~立即通过邀请好友的方式获得吧:)！#"+0);
				}
				else{
					//兑换的商品逻辑不能一样……
					if(bonusinfo.getBonus_id() == 1){//这是兑换的，要不同的处理逻辑
						int ratenumber = bonusinfo.getBonus_rate();
						if(lessThanPoint(ratenumber)){//中概率，判断有没有
							GuideLogger.printLog(" host:"+host.getId()+" HIT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
							int getsucc = RecallBonusObjectLogic.getInstance().increaseBonus(host, bonusid);
							if(getsucc == 1){//真中了 2
								int insertret = RecallBonusAwardLogic.getInstance().insert(host,bonusinfo.getBonus_id(),bonusinfo.getBonus_name());
								GuideLogger.printLog(" host:"+host.getId()+" SUCC!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" insert:"+insertret,GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "2#"+bonusinfo.getBonus_name()+"#"+(need*-1));
								//扣分
								RecallBonusAccountLogic.getInstance().doPayDecreament(host, need);
							}
							else{//没有了  4
								GuideLogger.printLog(" host:"+host.getId()+" NOLEFT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "4#抱歉~您要兑换的商品已经被抢购一空了！后续奖品请持续关注本活动哦:)#"+(0*-1));
							}
							
						}
						else{//没中 1  对于这个来说是不可能的
							GuideLogger.printLog(" host:"+host.getId()+" MISS:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "1#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得15个豆，每天都试试手气吧。#"+(need*-1));
						}
					}
					else{
						int ratenumber = bonusinfo.getBonus_rate();
						//先扣分
						RecallBonusAccountLogic.getInstance().doPayDecreament(host, need);
						if(lessThanPoint(ratenumber)){//中概率，判断有没有
							GuideLogger.printLog(" host:"+host.getId()+" HIT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
							int getsucc = RecallBonusObjectLogic.getInstance().increaseBonus(host, bonusid);
							if(getsucc == 1){//真中了 2
								int insertret = RecallBonusAwardLogic.getInstance().insert(host,bonusinfo.getBonus_id(),bonusinfo.getBonus_name());
								GuideLogger.printLog(" host:"+host.getId()+" SUCC!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" insert:"+insertret,GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "2#"+bonusinfo.getBonus_name()+"#"+(need*-1));
							}
							else{//没有了 4
								GuideLogger.printLog(" host:"+host.getId()+" NOLEFT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
								GuideXFiveRequestUtil.writeAjax(response, "4#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得15个豆，每天都试试手气吧~#"+(need*-1));
							}
							
						}
						else{//没中 1
							GuideLogger.printLog(" host:"+host.getId()+" MISS:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "1#很遗憾，只差一点点就中奖了。每天登录抽奖活动就能获得15个豆，每天都试试手气吧。#"+(need*-1));
						}
					}
				}
				
			}
			
			
		}
		
	}
	

	private void doInviteSubmit(HttpServletRequest request,HttpServletResponse response, User host) {
		if("1".equals("1")){
			String ret = "活动已结束。#"+0;
			GuideXFiveRequestUtil.writeAjax(response, ret);
			return;
		}
		/*if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
			return ;
		}
		String uids = GuideXFiveRequestUtil.getParamString(request, "uids", host.getId());
		GuideLogger.printLog(" host:"+host.getId()+" "+uids);
		String[] user_ids = { "" };
		if (uids != null && !"".equalsIgnoreCase(uids)) {
			user_ids = uids.split(",");
			if(user_ids != null){ //有人
				List<Integer> friends = FriendsHome.getInstance().getFriendsIds(host.getId());
				if(friends == null) {
					GuideLogger.printLog(" host:"+host.getId()+" friends null",GuideLogger.WARN);
					GuideXFiveRequestUtil.writeAjax(response, "请求超时，请重试。#0");
					return;
				}
				
				//找出今天已经发过的人
				List<RecallBonusInviteModel> todaylist = RecallBonusInviteLogic.getInstance().getByInviterIdAndToday(host);
				List<Integer> todaylist_int = new ArrayList<Integer>();
				if(todaylist != null){
					for (RecallBonusInviteModel m : todaylist) {
						todaylist_int.add(m.getInvitee_id());
					}
				}
				
				List<RecallBonusInviteModel> list = new ArrayList<RecallBonusInviteModel>();
				List<Integer> intlist = new ArrayList<Integer>();
				//做List
				for (String str : user_ids) {
					RecallBonusInviteModel m = new RecallBonusInviteModel();
					m.setInviter_id(host.getId());
					int invitee_id = 0;
					try {
						invitee_id = Integer.parseInt(str.trim());
					} catch (Exception e) {
						GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
						e.printStackTrace();
					}
					m.setInvitee_id(invitee_id);
					
					if(invitee_id!=0 && invitee_id!=host.getId() && !todaylist_int.contains(invitee_id) && friends.contains(m.getInvitee_id()) && !intlist.contains(invitee_id)){ //今天没发过并且是好友
						intlist.add(invitee_id);
						list.add(m);
					}
				}
				
				//发邀请数据
				int [] ret_invite = RecallBonusInviteLogic.getInstance().insertBatch(host, list);
				GuideLogger.printLog(" host:"+host.getId()+" invited:"+ret_invite.length);
				//加钱
				//int inc = (int) Math.ceil((double)list.size() / 3);
				//2010-3-24 16:38:26 不加钱了 陈周需求
				int inc = list.size(); 
				GuideLogger.printLog(" host:"+host.getId()+" inc:"+inc);
				if(inc > 0){
					RecallBonusAccountLogic.getInstance().doInviteIncreament(host, inc);
				}
				String ret_sub = inc == 0?"个。":"个,获得了抽奖豆"+inc+"个。";
				String ret = "你已经成功发送了好友邀请"+list.size()+ret_sub+"每发送1个好友申请就能获得抽奖豆1个。今天邀请过的好友，明天再来还能再邀请哦~:)#"+inc;
				GuideXFiveRequestUtil.writeAjax(response, ret);
				doNotifyThem(host,list);
				String ret = "你已经成功发送了好友邀请"+list.size()+"个。今天邀请过的好友，明天再来还能再邀请哦~:)#"+0;
				GuideXFiveRequestUtil.writeAjax(response, ret);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" user_ids null",GuideLogger.WARN);
			}
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" bonusid:"+uids,GuideLogger.WARN);
		}
		*/
	}


	@SuppressWarnings("unused")
	private void doNotifyThem(final User host, final List<RecallBonusInviteModel> list) {
		if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
			return ;
		}
		if(list == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
			return ;
		}
		GuideLogger.printLog(" host:"+host.getId()+" toids:"+list.size());
		final String urlguide = Globals.urlGuide.toString();
		Runnable taskSave_NET = new Runnable() {
			public void run() {
				for (RecallBonusInviteModel invitee : list) {
					if(invitee == null){
						GuideLogger.printLog(" host:"+host.getId()+" invitee null",GuideLogger.WARN);
						break;
					}
					String link = RecallBonusInviteLogic.getInstance().getInviteeLink(invitee.getInvitee_id(),urlguide);
					GuideLogger.printLog(" host:"+host.getId()+" link:"+link);
					if(link == null || link.endsWith("=")){
						continue;
					}
					//String notes = "Hi，"+"你的好友"+host.getName()+"邀请你去参加100%有奖，我为人人狂抽奖活动！"+"<a href=\""+link+"\">接受邀请</a>";
					
					Set<Integer> idset = new HashSet<Integer>();
					idset.add(invitee.getInvitee_id());
					//2010-3-23 18:21:20 先不发通知了
					/*try {
						AppNotificationMgr.sendNotification(93472, host.getId(), idset, notes, AppNotificationMgr.NOTIFICATION_TYPE_USER_TO_USER);
						//GuideLogger.printLog(" host:"+host.getId()+" fake_noteto:"+invitee.getInvitee_id());
					} 
					catch (AppNotificationException e) {
						GuideLogger.printLog(" host:"+host.getId()+" to:"+invitee.getInvitee_id()+" e:"+e.toString(),GuideLogger.ERROR);
						// TODO Auto-generated catch block
						//e.printStackTrace();
					}
					catch (Exception e){
						e.printStackTrace();
					}*/
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
		
		
	}


	private void doInit(Invocation inv, HttpServletRequest request, User host) {
		//查今天是不是加过钱，没加过就加，加过了就不加
		//然后INIT
		if(RecallBonusAccountLogic.getInstance().alreadyGetDailyIncreament(host)){
			GuideLogger.printLog(" host:"+host.getId()+" alreadyinctoday");
		}
		else{
			//2010-3-24 16:31:13
			/*int retcode = RecallBonusAccountLogic.getInstance().doDailyIncreament(host,IRecallBonusConst.AMOUNT_DAYLY_INCREMENT);
			GuideLogger.printLog(" host:"+host.getId()+" incdone:"+retcode);*/
			
			/*//改成先取天数，然后决定加多少豆
			int logindays  = GuideUtil.getKeepLoginDayFrom20100324(host);
			if(logindays <= 0){
				logindays = 1;
			}*/
			//int retcode = RecallBonusAccountLogic.getInstance().doDailyIncreament(host, IRecallBonusConst.AMOUNT_DAYLY_INCREMENT );
			//GuideLogger.printLog(" host:"+host.getId()+" incdone:"+retcode);
			GuideLogger.printLog(" host:"+host.getId()+" no incdone");
		}
		
		int current = RecallBonusAccountLogic.getInstance().getAccountCurrent(host);
		GuideLogger.printLog(" host:"+host.getId()+" current:"+current);
		
		addModel(inv,"rc_user_current",""+current);
		List<RecallBonusAwardModel> list = getAwardPersons(host);
		//List<Integer> ilist = new ArrayList<Integer>();
		/*if(list.size()<9){
			list.addAll(default_list);
		}*/
		int limit = 9;
		String rc_user_awarded = GuideUtil.getJSONForFriendSelectorForBounusResult(list,limit);
		addModel(inv,"rc_user_awarded",rc_user_awarded);
		
		//看剩余的11号商品
		int remain = doQueryBonusRemain(11);
		String sremain = "0";
		if(remain >= 1){
			sremain = "1";
		}
		else{
			sremain = "0";
		}
		addModel(inv,"rc_user_bonus_remain",sremain);
		
	}


	private final static List<Integer> default_list = new ArrayList<Integer>();
	static{
		default_list.add(223500512);
		default_list.add(39395980);
		default_list.add(221988163);
		default_list.add(232686677);
		default_list.add(200386284);
		default_list.add(221530589);
		default_list.add(242679045);
		default_list.add(248539544);
		default_list.add(230921637);
		
	}
	private List<RecallBonusAwardModel> getAwardPersons(User host) {
		//List<Integer> retlist = new ArrayList<Integer>();
		List<RecallBonusAwardModel> ulist = RecallBonusAwardLogic.getInstance().select10(host);
		/*List<RecallBonusAwardModel> retlist = new ArrayList<RecallBonusAwardModel>();
		if(ulist == null){
			
		}
		else{
			for (RecallBonusAwardModel m : ulist) {
				if (!retlist.contains(m.getUid())){
					retlist.add(m);
				}
			}
		}*/
		return ulist;
		
	}


	private void addModel(Invocation inv, String param, Object value) {
		inv.addModel(param,value);
	}


	public boolean lessThanPoint(int point){
		Random rand = new Random();
		int rint = rand.nextInt(10000000); //生成一个0到10000000的随机数 5000000 
		if(rint < point){ //如果随机数比Point小，就中了，所以中的概率是point卡在哪决定的
			return true;
		}
		return false;
		
	}
	
	public static void main(String[] args) {
		/*
		 * int truesum = 0; for (int i = 0; i < 100000; i++) { boolean ret =
		 * lessThanPoint(1);
		 * 
		 * if(ret){ truesum++; } }
		 * 
		 * System.out.println(truesum);
		 */
		// int inc = (int) Math.ceil((double)2 / 3);
		// System.out.println(inc);
		RecallBonusInitController c = new RecallBonusInitController();
		System.out.println(c.isInBonus50Time(null));

		Date currenttime = new Date();
		SimpleDateFormat lastf = new SimpleDateFormat("yyyy-MM-dd hh:00:00");
		long nowtime = currenttime.getTime();
		long lasttime = currenttime.getTime();
		try {
			lasttime = lastf.parse(lastf.format(currenttime)).getTime();

		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		long nexttimediff = 3600 - ((nowtime - lasttime) / (1000));
		System.out.println(currenttime + "|" + lastf.format(currenttime) + "|"+ nexttimediff);
	}

}
