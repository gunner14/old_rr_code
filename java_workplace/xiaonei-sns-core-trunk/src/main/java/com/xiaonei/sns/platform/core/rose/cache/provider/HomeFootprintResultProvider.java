/**
 * 
 */
package com.xiaonei.sns.platform.core.rose.cache.provider;

import org.apache.commons.lang.ArrayUtils;

import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;

/**
 * HomeFootprintResultProvider <br>
 * 
 * @author tai.wang@opi-corp.com Aug 27, 2010 - 1:43:40 PM
 */
public class HomeFootprintResultProvider extends
		AbstractProvider<HomeFootprintResult> {

	private static class SingletonHoler {
		public static volatile HomeFootprintResultProvider singleton = new HomeFootprintResultProvider();
	}

	public static HomeFootprintResultProvider getInstance() {
		return SingletonHoler.singleton;
	}

	public static final String KEY_PREF = HomeFootprintResultProvider.class
			.getName();

	@Override
	public HomeFootprintResult get(CacheManager cm, Object... params) {
		if (ArrayUtils.isEmpty(params) || params.length < 3) {
			return null;
		}
		for (int i = 0; i < params.length; i++) {
			if (!(params[i] instanceof Integer)) {
				return null;
			}
		}
		final int id = ((Integer) params[0]).intValue();
		final int begin = ((Integer) params[1]).intValue();
		final int limit = ((Integer) params[2]).intValue();
		HomeFootprintResult ret = (HomeFootprintResult) cm.get(KEY_PREF + id);
		if (ret == null) {// cache里没有
			ret = SnsAdapterFactory.getFootprintAdapter().getAllFootprints(id,
					begin, limit);
			if (null == ret) {
				return null;
			} else {
				cm.put(KEY_PREF + id, ret);
			}
		}
		return ret;
	}

}
