package com.renren.sns.privacy.service.impl;

import mop.hi.oce.domain.model.Relation;
import net.sf.json.JSONObject;

import org.springframework.stereotype.Service;

import com.renren.sns.privacy.service.PrivacyService;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Service("privacyService")
public class PrivacyServiceImpl implements PrivacyService {

	@Override
	public int getProiflePrivacy(int uid, String key) {
		int result = Relation.uuSelf;
		UserConfigInfo uci = null;
		try {
			uci = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(uid);
		} catch (final Exception e) {
			e.printStackTrace();
		}
		if (uci == null || uci.getProfilePrivacy() == null || uci.getProfilePrivacy().trim().length() == 0) {
			return result;
		}
		try {
			String profilePrivacy = uci.getProfilePrivacy();
			JSONObject json = JSONObject.fromObject(profilePrivacy);
			if (json.get(key) == null) {
				result = -1;
			} else {
				result = Integer.parseInt(String.valueOf(json.get(key)));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

}
