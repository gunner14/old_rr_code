package com.xiaonei.login.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.login.logic.LoginConfigureImpl;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.validator.annotation.EncryptTimer;
import com.xiaonei.security.login.toolkit.encrypt.EncryptAdapter;

@Path(value = { "getEncryptKey" })
@Singleton
@Ajax
@EncryptTimer
public class EncryptKeyController {

	protected Log logger = LogFactory.getLog(this.getClass());
	
	/**
	 * 返回加密密钥串
	 * @param email 用户名，用于日志记录
	 * @return 包含是否加密，以及密钥串
	 */
	@Get
	public String getEncryptKey(@HtmlPure @Param("email") String email) {
		Map<String, String> config = LoginConfigureImpl.getInstance().getLoginConfigure();
		String isEncrypt = config.get("isEncrypt");
		if (isEncrypt != null && "true".equalsIgnoreCase(isEncrypt)) {
			// 加密开放的百分比
			int openRate = 5;
			try {
				openRate = Integer.parseInt(config.get("encryptRate"));
			} catch (Exception e) {
				logger.error("Get encryptRate error:", e);
			}
			// 按随机方式抽取
			if (Math.random() * 100 >= openRate) {
				return "@{\"isEncrypt\":false}";
			}
			try {
				EncryptAdapter ea = EncryptAdapter.getInstance();
				String rkeys = ea.generateJSONRSAComponent();
				return "@{\"isEncrypt\":true," + StringUtils.substring(rkeys, 1);
			} catch (Exception e) {
				logger.error("Get encryptKey error:", e);
				return "@{\"isEncrypt\":false}";
			}
		}
		return "@{\"isEncrypt\":false}";
	}
}
