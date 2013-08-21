/**
 *
 */
package com.xiaonei.reg.register.logic.webservices;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * RegWebServicesLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-25 - 下午11:37:21
 */
public class RegWebServicesLogic {

	public final static int AUDIT_REG_STATUS_OK = 2;
	public final static int AUDIT_REG_STATUS_FAIL = 4;
	public final static int AUDIT_REG_STATUS_EMAIL_EXSIT = 6;
	public final static int AUDIT_REG_STATUS_ACTIVATE = 9;


	private static RegWebServicesLogic instance = new RegWebServicesLogic();

	private RegWebServicesLogic() {
		super();
	}

	public static RegWebServicesLogic getInstance() {
		return instance;
	}


	/**
	 * 调用某个url
	 * 
	 * @param sUrl
	 * @return
	 * @throws IOException
	 * @author wang-tai
	 */
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


	public boolean checkTsc(String tsc) {
		MemCacheManager mem = MemCacheFactory.getInstance()
				.getManagerByPoolName(MemCacheKeys.pool_wap);
		if (null != mem.get(tsc)) {
			mem.delete(tsc);
			return true;
		} else {
			return false;
		}
	}

}
