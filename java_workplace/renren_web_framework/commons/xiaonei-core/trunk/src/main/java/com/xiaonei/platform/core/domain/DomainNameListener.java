package com.xiaonei.platform.core.domain;

import java.util.Map;

import com.xiaonei.service.util.descriptor.SvcDescriptorListener;

public class DomainNameListener extends SvcDescriptorListener {

	@Override
	protected boolean update(Map<String, String> arg0) {
		System.out.println("========domain name:======reciver update  event=============");
		DomainNameFactory.getInstance().initDomainName();
		return true;
	}

}
