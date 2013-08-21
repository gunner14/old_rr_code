package com.xiaonei.passport.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.passport.service.LoginType;
import com.xiaonei.xce.ticket.TicketInfo;

public class TypeConvertUtil {
	private static Map<Integer, LoginType> types=new HashMap<Integer, LoginType>();
	private static Map<LoginType, Integer> loginTypes=new HashMap<LoginType, Integer>();
	//private static Map<Integer, Integer> valueTypes=new HashMap<Integer, Integer>();
	static {
		types.put(TicketInfo.Type.Web, LoginType.Web);
		types.put(TicketInfo.Type.Wap, LoginType.Wap);
		types.put(TicketInfo.Type.PlatForm, LoginType.PlatForm);
		types.put(TicketInfo.Type.Client, LoginType.Client);
		types.put(TicketInfo.Type.MobileClient, LoginType.MobileClient);
		loginTypes.put(LoginType.Web, TicketInfo.Type.Web);
		loginTypes.put(LoginType.Wap, TicketInfo.Type.Wap);
		loginTypes.put(LoginType.PlatForm, TicketInfo.Type.PlatForm);
		loginTypes.put(LoginType.Client, TicketInfo.Type.Client);
		loginTypes.put(LoginType.MobileClient, TicketInfo.Type.MobileClient);
//		valueTypes.put(1, TicketInfo.Type.Web);
//		valueTypes.put(2, TicketInfo.Type.Wap);
//		valueTypes.put(3, TicketInfo.Type.PlatForm);
//		valueTypes.put(4, TicketInfo.Type.Client);
//		valueTypes.put(5, TicketInfo.Type.MobileClient);
	}
	public static LoginType type2LoginType(int type){
		return types.get(type);
	}
	public static int loginType2Type(LoginType loginType){
		/**因为createTicket的时候写入的是新登录中的LoginType的值 
		  Web(1),
		  Wap(2),
		  PlatForm(3),
		  Client(4),
		  MobileClient(5);
		  而在verifyTicket的时候却要把这个值转换成TicketInfo.Type中对应的值，这显然是无法通过的
		*/
		return loginType.getValue();
		/*Integer result= loginTypes.get(loginType);
		if(result==null){
			return -1;
		}
		return result;*/
	}
	public static com.renren.passport.service.TicketInfo convertTicketInfo(TicketInfo info){
		 com.renren.passport.service.TicketInfo ticketInfo=new com.renren.passport.service.TicketInfo();
		 if(info==null){
			 return ticketInfo;
		 }
		 ticketInfo.setIp(info.get_ip());
		 ticketInfo.setLoginType(type2LoginType(info.get_type()));
		 ticketInfo.setUserId(Long.valueOf(info.get_id()).intValue());
		 ticketInfo.setUserId(Long.valueOf(info.get_realId()).intValue());
		 return ticketInfo;
	}
	
	public static List<Integer> loginType2Type(List<LoginType> loginTypes){
		List<Integer> types=new ArrayList<Integer>();
		for(LoginType loginType: loginTypes){
			Integer type=TypeConvertUtil.loginType2Type(loginType);
			if(type!=null){
				types.add(type);
			}
		}
		
		return types;
	}
	
}
