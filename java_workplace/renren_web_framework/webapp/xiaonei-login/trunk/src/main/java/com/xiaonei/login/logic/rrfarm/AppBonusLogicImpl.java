/**
 * 
 */
package com.xiaonei.login.logic.rrfarm;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import com.xiaonei.login.logic.ObjectMaker;

/**
 * AppBonusLogicImpl <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 5:50:32 PM
 */
public class AppBonusLogicImpl implements AppBonusLogic {

	private static final AppBonusLogicImpl instance = ObjectMaker
			.getInstance(AppBonusLogicImpl.class);

	public static AppBonusLogicImpl getInstance() {
		return instance;
	}

	public static String wgetUrl(String sUrl) throws IOException {
		URL url = new URL(sUrl);
		HttpURLConnection uc = null;
		BufferedReader in = null;

		uc = (HttpURLConnection) url.openConnection();
		uc.connect();

		String msg = null;
		try {
			in = new BufferedReader(new InputStreamReader(uc.getInputStream()));
			msg = in.readLine();
		} catch (IOException e) {
			throw e;
		} finally {
			if (in != null)
				in.close();
		}
		return msg;
	}

	@Override
	public void rewardItem(int userId, int rewardItemId, int rewardItemNum) {
		StringBuffer s = new StringBuffer();
		s.append("?key=" + requestKey);
		s.append("&userId=" + userId);
		s.append("&rewardItemId=" + rewardItemId);
		s.append("&rewardItemNum=" + rewardItemNum);
		try {
			wgetUrl(rewardItemUrl + s.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void rewardMoney(int userId, int rewardMoney) {
		StringBuffer s = new StringBuffer();
		s.append("?key=" + requestKey);
		s.append("&userId=" + userId);
		s.append("&rewardMoney=" + rewardMoney);
		try {
			wgetUrl(rewardMoneyUrl + s.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void rewardProduct(int userId, int rewardItemId, int rewardItemNum) {
		StringBuffer s = new StringBuffer();
		s.append("?key=" + requestKey);
		s.append("&userId=" + userId);
		s.append("&rewardItemId=" + rewardItemId);
		s.append("&rewardItemNum=" + rewardItemNum);
		try {
			wgetUrl(rewardProductUrl + s.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
