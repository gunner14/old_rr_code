package com.xiaonei.reg.guide.action.netbarflow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;




public class PageChooser {
	private static final Map <String,Integer> pagemap = new HashMap<String,Integer>();
	static {
		 pagemap.put("Michael Jackson"       ,600002101);
		 pagemap.put("变形金刚Ⅱ"            ,600002103);
		 pagemap.put("罗永浩"                ,600002131);
		 pagemap.put("火锅"                  ,600002132);
		 pagemap.put("DotA"                  ,600002133);
		 pagemap.put("Tank"                  ,600002139);
		 pagemap.put("德怀特·霍华德"        ,600002150);
		 pagemap.put("联合国环境规划署"      ,600002165);
		 pagemap.put("沧月"                  ,600002190);
		 pagemap.put("飞花"                  ,600002192);
		 pagemap.put("敷米浆"                ,600002193);
		 pagemap.put("君天"                  ,600002194);
		 pagemap.put("马好"                  ,600002196);
		 pagemap.put("那多"                  ,600002197);
		 pagemap.put("孙睿"                  ,600002198);
		 pagemap.put("张冠仁"                ,600002199);
		 pagemap.put("野生物贸易研究TRAFFIC" ,600002203);
		 pagemap.put("心灵阳光工程"          ,600002204);
		 pagemap.put("气候组织TCG"           ,600002205);
		 pagemap.put("道聼途説的阿涩"        ,600002206);
		 pagemap.put("旅游卫视-心旅程"       ,600002207);
		 pagemap.put("青艾工程"              ,600002208);
		 pagemap.put("1+1心联行动"           ,600002209);
		 pagemap.put("环球小姐-王静瑶"       ,600002210);
		 pagemap.put("苏素"                  ,600002212);
		 pagemap.put("安琥"                  ,600002213);
		 pagemap.put("绿色和平"              ,600002214);
		 pagemap.put("游鸿明"                ,600002216);
		 pagemap.put("最佳现场"              ,600002217);
		 pagemap.put("联盟影评"              ,600002218);
		 pagemap.put("自然之友"              ,600002219);
		 pagemap.put("周迅"                  ,600002221);
		 pagemap.put("王宝强"                ,600002223);
		 pagemap.put("李易峰"                ,600002225);
		 pagemap.put("山野"                  ,600002226);
		 pagemap.put("生态中国"              ,600002227);
		 pagemap.put("段奕宏"                ,600002228);
		 pagemap.put("宇桐非"                ,600002229);
		 pagemap.put("BOBO"                  ,600002231);
		 pagemap.put("09新生"                ,600002232);
		 pagemap.put("光良"                  ,600002233);
		 pagemap.put("李宇春"                ,600002235);
		 pagemap.put("南都公益基金会"        ,600002236);
		 pagemap.put("李渊"                  ,600002242);
		 pagemap.put("人人网"                ,600002246);
		 pagemap.put("张梓琳"                ,600002247);
		 pagemap.put("背后的故事"            ,600002248);
		 pagemap.put("兄弟联"                ,600002249);
		 pagemap.put("手机人人网"            ,600002250);
		 pagemap.put("湖南卫视"              ,600002251);
		 pagemap.put("蔡健雅"                ,600002252);
		 pagemap.put("徐佳莹"                ,600002253);
		 pagemap.put("新人电影节"            ,600002255);
		 pagemap.put("羽泉"                  ,600002256);
		 pagemap.put("开放平台"              ,600002257);
		 pagemap.put("七夕节"                ,600002258);
		 pagemap.put("环球时报"              ,600002260);
		 pagemap.put("阿狸"                  ,600002261);
		 pagemap.put("李昕岳"                ,600002263);
		 pagemap.put("每日文娱播报"          ,600002264);
		 pagemap.put("尚雯婕"                ,600002266);
		 pagemap.put("HIT-5"                 ,600002267);
		 pagemap.put("刘力扬"                ,600002269);
		 pagemap.put("联合国世界粮食计划署"  ,600002270);
		 pagemap.put("袁成杰"                ,600002271);
		 pagemap.put("联合国开发计划署"      ,600002272);
		 pagemap.put("戚薇"                  ,600002273);
		 pagemap.put("迪克牛仔"              ,600002274);
		 pagemap.put("汽车侠"                ,600002275);
		 pagemap.put("雷励中国"              ,600002276);
		 pagemap.put("曹格"                  ,600002277);
		 pagemap.put("朱梓骁"                ,600002279);
		 pagemap.put("张翰"                  ,600002281);
		 pagemap.put("俞灏明"                ,600002282);
		 pagemap.put("魏晨"                  ,600002283);
		 pagemap.put("友成企业家扶贫基金会"  ,600002284);
		 pagemap.put("光华基金会创新工程"    ,600002285);
		 pagemap.put("林园"                  ,600002286);
		 pagemap.put("金陵晚报"              ,600002287);
		 pagemap.put("行动援助（中国）"      ,600002288);
		 pagemap.put("2010上海世博会“魔方”",600002289);
		 pagemap.put("中国绿化基金会"        ,600002291);
		 pagemap.put("上海绿丝带"            ,600002292);
		 pagemap.put("教师节"                ,600002294);
		 pagemap.put("《商界》"              ,600002295);
		 pagemap.put("饶雪漫"                ,600002296);
		 pagemap.put("肖雅娴"                ,600002297);
		 pagemap.put("安意如"                ,600002298);
		 pagemap.put("茱莉"                  ,600002299);
		 pagemap.put("S.H.E"                 ,600002300);
		 pagemap.put("精舞门"                ,600002303);
		 pagemap.put("千橡校园招聘"          ,600002304);
		 pagemap.put("巫启贤"                ,600002306);
		 pagemap.put("江美琪"                ,600002311);
		 pagemap.put("鲁昕儿"                ,600002313);
		 pagemap.put("中国日报 China Daily"  ,600002333);
		 pagemap.put("NBA北京赛"             ,600002335);
		 pagemap.put("猫扑话题"              ,600002336);
		 pagemap.put("李冰冰"     ,600002337);
		 pagemap.put("《恋爱前规则》"        ,600002338);
		 pagemap.put("郭妮"                  ,600002339);
		 pagemap.put("国庆节"                ,600002341);
		 pagemap.put("陈奕迅"                ,600002342);
		 pagemap.put("中秋节"                ,600002343);
		 pagemap.put("林晓培"                ,600002346);
		 pagemap.put("人民网"                ,600002348);
		 pagemap.put("张信哲"                ,600002349);
		 pagemap.put("瑞士国家旅游局"        ,600002350);
		 pagemap.put("花儿乐队"              ,600002355);
		 pagemap.put("植物大战僵尸"          ,600002382);
		 pagemap.put("重阳节"                ,600002395);
		 pagemap.put("三国杀"                ,600002408);
		                                              

	}
	
	//0-15 	男：S.H.E,李宇春,光良,阿狸
	//女：BOBO,李宇春,S.H.E,阿狸
	//16-22	男：光良,李宇春,德怀特·霍华德,S.H.E
	//女：光良,李宇春,Tank,BOBO
	//23-30	男：光良,李宇春,羽泉,陈奕迅
	//女：光良,李宇春,蔡健雅,周迅
	//31-40	男：李冰冰,周迅,王宝强,陈奕迅
	//女：王宝强,游鸿明,李冰冰,周迅
	//41-无限	男：王宝强,陈奕迅,周迅,李冰冰
	//女：王宝强,李冰冰,周迅,段奕宏
	private static final String [] boy0015 = { "S.H.E","李宇春","光良","阿狸"};
	private static final String [] girl0015 = {"BOBO","李宇春","S.H.E","阿狸"};
	private static final String [] boy1622 = {"光良","李宇春","德怀特·霍华德","S.H.E"};
	private static final String [] girl1622 = {"光良","李宇春","Tank","BOBO"};
	private static final String [] boy2330 = {"光良","李宇春","羽泉","陈奕迅"};
	private static final String [] girl2330 = {"光良","李宇春","蔡健雅","周迅"};
	private static final String [] boy3140 = {"李冰冰","周迅","王宝强","陈奕迅"};
	private static final String [] girl3140 = {"王宝强","游鸿明","李冰冰","周迅"};
	private static final String [] boy4199 = {"王宝强","陈奕迅","周迅","李冰冰"};
	private static final String [] girl4199 = {"王宝强","李冰冰","周迅","段奕宏"};
	
	public static List<Integer> getPageIds(User host){
		List<Integer> list = new ArrayList<Integer>();
		int age = BirthdayLogic.getInstance().getAge(host);
		String gender = host.getGender();
		String [] names = {};
		GuideLogger.printLog("gender:"+gender+" age:"+age);
		if (gender.startsWith("男")) {
			if (age >= 0 && age <= 15) {
				names = boy0015;
			} 
			else if (age >= 16 && age <= 22) {
				names = boy1622;
			} 
			else if (age >= 23 && age <= 30) {
				names = boy2330;
			} 
			else if (age >= 31 && age <= 40) {
				names = boy3140;
			} 
			else if (age >= 41) {
				names = boy4199;
			}
		} else {
			if (age >= 0 && age <= 15) {
				names = girl0015;
			} 
			else if (age >= 16 && age <= 22) {
				names = girl1622;
			} 
			else if (age >= 23 && age <= 30) {
				names = girl2330;
			} 
			else if (age >= 31 && age <= 40) {
				names = girl3140;
			} 
			else if (age >= 41) {
				names = girl4199;
			}
		}
		for(String s : names){
			if(pagemap.get(s)==null){
				GuideLogger.printLog("name:"+s+" page null",GuideLogger.WARN);
				continue;
			} 
			list.add(pagemap.get(s));
		}
		return list;
	}
}
