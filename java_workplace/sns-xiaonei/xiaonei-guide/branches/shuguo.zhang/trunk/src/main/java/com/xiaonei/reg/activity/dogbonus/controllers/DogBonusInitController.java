package com.xiaonei.reg.activity.dogbonus.controllers;

import java.util.ArrayList;
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
import com.xiaonei.reg.activity.dogbonus.constant.IDogBonusConst;
import com.xiaonei.reg.activity.dogbonus.logic.DogBonusAccountLogic;
import com.xiaonei.reg.activity.dogbonus.logic.DogBonusAwardLogic;
import com.xiaonei.reg.activity.dogbonus.logic.DogBonusInviteLogic;
import com.xiaonei.reg.activity.dogbonus.logic.DogBonusObjectLogic;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAwardModel;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusInviteModel;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusObjectModel;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.webservices.RegWebServicesLogic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;


@LoginRequired
@Path("dogbonus")
public class DogBonusInitController {
	private static final String TO_INIT = "/views/activity/dogbonus/dogbonus.jsp";
	private static final String TO_DEBUG_MAIN = "/views/activity/dogbonus/d_dog_main.jsp";
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
				doInit(inv,request,host);
				
				return TO_INIT;
			}
			else if(MyStringUtils.equals("invite", action)){//invite
				doInvite(request,response,host);
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("invitefriends", action)){//invite
				//邀请好友
				
				//取参数列表
				//写邀请表
				doInviteSubmit(request,response,host);
				
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("get", action)){//get
				//抽奖
				doGetBonus(request,response,host);
				
				return AJAX_RETURN;
			}
			else if(MyStringUtils.equals("accept", action)){//accept
				//被邀请
				doAccept(request,response,host);
				
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
			List<DogBonusInviteModel> todaylist = DogBonusInviteLogic.getInstance().getByInviterIdAndToday(host);
			List<Integer> todaylist_int = new ArrayList<Integer>();
			if(todaylist != null){
				for (DogBonusInviteModel m : todaylist) {
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
			ret = GuideUtil.getJSONForFriendSelectorFromIdsForKaixinDog(friends);
		}
		GuideXFiveRequestUtil.writeAjax(response, ret);
		
	}


	private void doGetLink(HttpServletRequest request,	HttpServletResponse response, User host) {
		String urlguide = Globals.urlGuide.toString();
		String link = DogBonusInviteLogic.getInstance().getInviteeLink(host.getId(),urlguide);
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
					DogBonusInviteModel invitem = DogBonusInviteLogic.getInstance().getById(id);
					if(invitem == null){
						GuideLogger.printLog(" host:"+host.getId()+" inviter:"+id+" ",GuideLogger.WARN);
						continue;
					}
					else{
						if(invitem.getInvitee_id() == host.getId()){
							if(invitem.getAccept_flag() == 0){
								GuideLogger.printLog(" host:"+host.getId()+" inviter:"+invitem.getInviter_id()+" id:"+id,GuideLogger.WARN);
								DogBonusAccountLogic.getInstance().doAcceptedIncreament(host, invitem.getInviter_id(), IDogBonusConst.AMOUNT_ACCEPT_INCREMENT);
								donelist.add(id);
								User inviter = SnsAdapterFactory.getUserAdapter().get(invitem.getInviter_id());
								if(inviter == null){
									GuideLogger.printLog(" host:"+host.getId()+" invitee_accept:"+invitem.getInvitee_id()+" is not kaixin!",GuideLogger.WARN);
								}
								else{
									if(MyStringUtils.contains(inviter.getDomain(), "kaixin")){
										try {
											GuideLogger.printLog(" host:"+host.getId()+" invitee_accept:"+invitem.getInvitee_id()+" BONUSED!",GuideLogger.WARN);
                                            //UserLoginHomeEncourage.getInstance().loginEncourage(invitem.getInviter_id(), IDogBonusConst.AMOUNT_ACCEPT_INCREMENT);
											String url = "http://dog.kaixin.com/rewardbones.do?key=886971c7961950114e97b8f2963f1e10&method=1&encourage_bones="+IDogBonusConst.AMOUNT_ACCEPT_INCREMENT+"&uid="+invitem.getInviter_id();
											RegWebServicesLogic.wgetUrl(url);
                                        } catch (Exception e) {
											GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
                                            e.printStackTrace();
                                        } 
									}
								}
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
				DogBonusInviteLogic.getInstance().acceptBatch(host, donelist, host.getId());
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" ",GuideLogger.WARN);
			}
			
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" ",GuideLogger.WARN);
		}
		
	}
	

	private void doNotifyThem(final User host, final List<DogBonusInviteModel> list) {
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
				for (DogBonusInviteModel invitee : list) {
					if(invitee == null){
						GuideLogger.printLog(" host:"+host.getId()+" invitee null",GuideLogger.WARN);
						break;
					}
					String link = DogBonusInviteLogic.getInstance().getInviteeLink(invitee.getInvitee_id(),urlguide);
					GuideLogger.printLog(" host:"+host.getId()+" link:"+link);
					if(link == null || link.endsWith("=")){
						continue;
					}
					//String notes = "Hi，"+"你的好友"+host.getName()+"邀请你去免费领养最贵的狗狗！"+"<a href=\""+link+"\">接受邀请</a>";
					
					Set<Integer> idset = new HashSet<Integer>();
					idset.add(invitee.getInvitee_id());
					try {
						//AppNotificationMgr.sendNotification(1000038, host.getId(), idset, notes, AppNotificationMgr.NOTIFICATION_TYPE_USER_TO_USER);
						GuideLogger.printLog(" host:"+host.getId()+" fake_noteto:"+invitee.getInvitee_id());
					} 
					/*catch (AppNotificationException e) {
						GuideLogger.printLog(" host:"+host.getId()+" to:"+invitee.getInvitee_id()+" e:"+e.toString(),GuideLogger.ERROR);
						// TODO Auto-generated catch block
						//e.printStackTrace();
					}*/
					catch (Exception e){
						e.printStackTrace();
					}
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
		
		
	}

	private void doGetBonus(HttpServletRequest request,	HttpServletResponse response, User host) {
		int bonusid = GuideXFiveRequestUtil.getParamInteger(request, "bid", host.getId());
		if(bonusid == 0 || bonusid > 9){ //id错了
			GuideLogger.printLog(" host:"+host.getId()+" code:"+5);
			GuideXFiveRequestUtil.writeAjax(response, "5#您选择的奖品号不存在");
		}
		else{
			DogBonusObjectModel bonusinfo = DogBonusObjectLogic.getInstance().getBounusInfo_FromMem(host, bonusid);
			if(bonusinfo == null){
				GuideLogger.printLog(" host:"+host.getId()+" binfo null!");
			}
			else{
				int need = bonusinfo.getBonus_cost();
				int current = DogBonusAccountLogic.getInstance().getAccountCurrent(host);
				if(current < need){//分不够  3
					GuideLogger.printLog(" host:"+host.getId()+" NEEDCURRENT:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" current:"+current,GuideLogger.WARN);
					GuideXFiveRequestUtil.writeAjax(response, "3#您的抽奖豆不够了哦~立即通过邀请好友的方式获得吧:)！");
				}
				else{
					int ratenumber = bonusinfo.getBonus_rate();
					//先扣分
					DogBonusAccountLogic.getInstance().doPayDecreament(host, need);
					if(lessThanPoint(ratenumber)){//中概率，判断有没有
						GuideLogger.printLog(" host:"+host.getId()+" HIT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
						int getsucc = DogBonusObjectLogic.getInstance().increaseBonus(host, bonusid);
						if(getsucc == 1){//真中了 2
							int insertret = DogBonusAwardLogic.getInstance().insert(host,bonusinfo.getBonus_id(),bonusinfo.getBonus_name());
							GuideLogger.printLog(" host:"+host.getId()+" SUCC!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]"+" insert:"+insertret,GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "2#"+bonusinfo.getBonus_name());
						}
						else{//没有了 4
							GuideLogger.printLog(" host:"+host.getId()+" NOLEFT!:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
							GuideXFiveRequestUtil.writeAjax(response, "4#您要的商品已被抽取一空");
						}
						
					}
					else{//没中 1
						GuideLogger.printLog(" host:"+host.getId()+" MISS:["+bonusinfo.getBonus_id()+","+bonusinfo.getBonus_name()+"]",GuideLogger.WARN);
						GuideXFiveRequestUtil.writeAjax(response, "1#没中");
					}
				}
				
			}
			
			
		}
		
	}
	
	private void doInviteSubmit(HttpServletRequest request,HttpServletResponse response, User host) {
		if(host == null){
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
				List<DogBonusInviteModel> todaylist = DogBonusInviteLogic.getInstance().getByInviterIdAndToday(host);
				List<Integer> todaylist_int = new ArrayList<Integer>();
				if(todaylist != null){
					for (DogBonusInviteModel m : todaylist) {
						todaylist_int.add(m.getInvitee_id());
					}
				}
				
				List<DogBonusInviteModel> list = new ArrayList<DogBonusInviteModel>();
				List<Integer> intlist = new ArrayList<Integer>();
				//做List
				for (String str : user_ids) {
					DogBonusInviteModel m = new DogBonusInviteModel();
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
				int [] ret_invite = DogBonusInviteLogic.getInstance().insertBatch(host, list);
				GuideLogger.printLog(" host:"+host.getId()+" invited:"+ret_invite.length);
				//加钱
				//int inc = (int) Math.ceil((double)list.size() / 3);
				int inc = list.size(); 
				GuideLogger.printLog(" host:"+host.getId()+" inc:"+inc);
				if(inc > 0){
					DogBonusAccountLogic.getInstance().doInviteIncreament(host, inc);
				}
				//String ret_sub = inc == 0?"个。":"个,获得了抽奖豆"+inc+"个。";
				String ret = "你已经成功发送了好友邀请"+list.size()+"个。今天邀请过的好友，明天再来还能再邀请哦~:)#"+0;
				GuideXFiveRequestUtil.writeAjax(response, ret);
				doNotifyThem(host,list);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" user_ids null",GuideLogger.WARN);
			}
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" bonusid:"+uids,GuideLogger.WARN);
		}
		
	}

	private void doInit(Invocation inv, HttpServletRequest request, User host) {
		//查今天是不是加过钱，没加过就加，加过了就不加
		//然后INIT
		if(DogBonusAccountLogic.getInstance().alreadyGetDailyIncreament(host)){
			GuideLogger.printLog(" host:"+host.getId()+" alreadyinctoday");
		}
		else{
			int retcode = DogBonusAccountLogic.getInstance().doDailyIncreament(host,IDogBonusConst.AMOUNT_DAYLY_INCREMENT);
			GuideLogger.printLog(" host:"+host.getId()+" incdone:"+retcode);
		}
		
		int current = DogBonusAccountLogic.getInstance().getAccountCurrent(host);
		GuideLogger.printLog(" host:"+host.getId()+" current:"+current);
		
		addModel(inv,"rc_user_current",""+current);
		List<DogBonusAwardModel> list = getAwardPersons(host);
		addModel(inv,"rc_user_awarded",list);
		//List<Integer> ilist = new ArrayList<Integer>();
		/*if(list.size()<9){
			list.addAll(default_list);
		}*/
		//String rc_user_awarded = GuideUtil.getJSONForFriendSelectorForBounusResult(list);
		//addModel(inv,"rc_user_awarded",rc_user_awarded);
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
	private List<DogBonusAwardModel> getAwardPersons(User host) {
		//List<Integer> retlist = new ArrayList<Integer>();
		List<DogBonusAwardModel> ulist = DogBonusAwardLogic.getInstance().select10(host);
		/*List<DogBonusAwardModel> retlist = new ArrayList<DogBonusAwardModel>();
		if(ulist == null){
			
		}
		else{
			for (DogBonusAwardModel m : ulist) {
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
		int rint = rand.nextInt(10000000); //生成一个0到10000000的随机数
		if(rint < point){ //如果随机数比Point小，就中了，所以中的概率是point卡在哪决定的
			return true;
		}
		return false;
		
	}
	
	public static void main(String[] args) {
		/*int truesum = 0;
		for (int i = 0; i < 100000; i++) {
			boolean ret = lessThanPoint(1);
			
			if(ret){
				truesum++;
			}
		}
		
		System.out.println(truesum);*/
		int inc = (int) Math.ceil((double)2 / 3);
		System.out.println(inc);
	}

}
