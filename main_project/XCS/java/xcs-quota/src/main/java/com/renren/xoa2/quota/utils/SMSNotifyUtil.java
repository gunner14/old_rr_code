package com.renren.xoa2.quota.utils;

import java.util.List;

import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.sms.LanTaoSmsService;

/**
 * 短信报警工具类（Quota内部使用）
 * @author Xue Wenxin Nov 19, 2012 6:50:23 PM
 */
public class SMSNotifyUtil {

	public final static String EXCEED_QUOTA = "EXCEED_QUOTA";

	public final static String GET_QUOTA_CACHE_FAILURE = "GET_QUOTA_CACHE_FAILURE";

	private static LanTaoSmsService smsService = new LanTaoSmsService();

	public static void smsSend(String message, String state) {
		String clientId = XcsConfigurations.getConfiguration().getClientUsername();
		List<String> servicePhones = XcsConfigurations.getConfiguration().getClientAlarmPhones();
		String ip = IPAddrUtil.getLocalhostIp();

		if (servicePhones != null) {
			for (String phone : servicePhones) {
				smsService.sendSms(phone, ip, clientId, state, message);
			}
		}
	}

}
