/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import mop.hi.oce.adapter.AdapterFactory;

import com.xiaonei.sns.platform.core.opt.ice.IVisitFootprintAdapter;
import com.xiaonei.xce.footprintunite.VisitFootprintAdapter;
import com.xiaonei.xce.footprintunite.VisitFootprintResult;

/**
 * 脚印服务
 * 
 * @author tai.wang@opi-corp.com Sep 8, 2010 - 7:29:03 PM
 */
public class WVisitFootprintAdapter extends BaseUsersAdapter<VisitFootprintAdapter>
		implements IVisitFootprintAdapter {

	protected WVisitFootprintAdapter() {
	}

	@Override
	protected VisitFootprintAdapter makeObject() {
		return AdapterFactory.getVisitFootprintAdapter();
	}

	@Override
	protected String getServiceName() {
		return "visitfootprint";
	}

	@Override
	public VisitFootprintResult getAll(int owner, int begin, int limit) {
		return getAdapter().getAll(owner, begin, limit);
	}
}
