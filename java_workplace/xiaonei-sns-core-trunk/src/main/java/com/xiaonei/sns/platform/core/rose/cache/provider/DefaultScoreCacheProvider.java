/**
 * 
 */
package com.xiaonei.sns.platform.core.rose.cache.provider;

import org.apache.commons.lang.ArrayUtils;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.xce.scorecache.DefaultScoreCache;
import com.xiaonei.xce.scorecache.ScoreCacheAdapter;

/**
 * DefaultScoreCacheProvider <br>
 * 
 * @see AbstractProvider
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 3:21:11 PM
 */
public class DefaultScoreCacheProvider extends
		AbstractProvider<DefaultScoreCache> {

	private static class SingletonHoler {
		public static final DefaultScoreCacheProvider singleton = new DefaultScoreCacheProvider();
	}

	public static DefaultScoreCacheProvider getInstance() {
		return SingletonHoler.singleton;
	}

	public static final String KEY_PREF = DefaultScoreCacheProvider.class
			.getName();

	@Override
	public DefaultScoreCache get(CacheManager cm, Object... params) {
		if (ArrayUtils.isEmpty(params)) {
			return null;
		}
		DefaultScoreCache ret = null;
		if (!(params[0] instanceof Integer)) {
			return null;
		}
		int id = ((Integer) params[0]).intValue();
		ret = (DefaultScoreCache) cm.get(KEY_PREF + id);
		if (ret == null) {// cache里没有
			ret = ScoreCacheAdapter.getInstance().getScoreData(id);
			if (null == ret) {
				ret = ScoreCacheAdapter.getInstance().getScoreData(id);
				if (null == ret) {
					return null;
				}
				cm.put(KEY_PREF + id, ret);
			} else {
				cm.put(KEY_PREF + id, ret);
			}
		}
		return ret;
	}

}
