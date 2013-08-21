package com.xiaonei.reg.activity.manager.logic;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import com.xiaonei.reg.common.MD5Util;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.webservices.RegWebServicesLogic;


/*10——2个礼券（从领取之日起，月底到期）
11——5个礼券 （从领取之日起，月底到期）
12——10个礼券 （从领取之日起，月底到期）
13——7天vip体验
14——1个月vip会员
15——3个月vip会员*/

/*1 	15 	3236 	1000000 	礼券2个 	-1
2 	6 	297 	500000 	礼券5个 	-1
3 	9 	112 	100000 	礼券10个 	-1
4 	3 	8 	10000 	VIP7天免费体验 	-1
5 	6 	0 	1000 	VIP30天免费体验 	-1
6 	9 	1 	100 	VIP3个月免费体验 	-1
*/
public class SendAwardLogic {
	private static SendAwardLogic instance = new SendAwardLogic();

    public static SendAwardLogic getInstance() {
        return instance;
    }

    private final static String URL_001= "http://i.renren.com/api/score/reg?";//uid=xxx&rid=xxx&check=xxx";
    private final static String URL_002= "http://i.renren.com/api/score/consume?";//uid=xxx&vc=xxx&check=xxx";
    
    private final static Map<Integer,Integer> final_map =  new HashMap<Integer, Integer>();
    static{
    	final_map.put(1, 10);
    	final_map.put(2, 11);
    	final_map.put(3, 12);
    	final_map.put(4, 13);
    	final_map.put(5, 14);
    	final_map.put(6, 15);
    	final_map.put(7, 10);
    	final_map.put(8, 10); //怕抛
    	final_map.put(11, 0); //怕抛
    }
    
    public String mysend(int award_id,int to_uid){
    	int award_key = final_map.get(award_id);
    	if(award_key == 0){
    		System.out.println(000);
    		return "err";
    	}
    	else{
    		return send(award_key,to_uid);
    	}
    }
    
    public void update(){
    	
    }
    
    private String send(int award_key,int to_uid){
    	if(!Globals.DEBUG_ENV){
    		GuideLogger.printLog("to_uid:"+to_uid+" award_key:"+award_key+"  not debug",GuideLogger.WARN);
    	}
    	try {
    		String check1 = MD5Util.md5(to_uid+"_"+award_key+"_uLCU2j7kCbwTXEnV");
    		String url1 = URL_001+"uid="+to_uid+"&rid="+award_key+"&check="+check1;
			String ret_code = RegWebServicesLogic.wgetUrl(url1);
			
			String check2 = MD5Util.md5(to_uid+"_"+ret_code+"_"+"bho4xSgyV7sXRIK6");
			String url2 = URL_002+"uid="+to_uid+"&vc=" +ret_code+"&check="+check2;
			String ret = RegWebServicesLogic.wgetUrl(url2);
			GuideLogger.printLog("to_uid:"+to_uid+" award_key:"+award_key+" ret:"+ret,GuideLogger.WARN);
			return ret;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	return null;
    }
    
    public static void main(String[] args) {
    	//SendAwardLogic.getInstance().mysend(4, 81120);
	}
    
}
