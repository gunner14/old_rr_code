package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * 指纹字典，用来快速查询内容是否重复。
 * <p>
 * 查询指纹是否存在时首先要给出查询的范围scope，相同的内容在不同的scope下重复判断的结果可能不一样。
 * 
 * @author yi.li@renren-inc.com since 2012-7-20
 * 
 */
public class FingerPrintDict<K, V> {

	private Map<K, Set<V>> dic;

	/**
	 * check if value exists in the scope
	 * 
	 * @param scope
	 * @param value
	 * @return true if exists the same value in the scope, otherwise false
	 */
	public boolean checkDuplicateAndSet(K scope, V value) {
		if (null == dic) {
			dic = new HashMap<K, Set<V>>();
		}

		Set<V> repository = dic.get(scope);
		if (null == repository) {
			repository = new HashSet<V>();
			dic.put(scope, repository);
		}

		if (repository.contains(value)) {
			return true;
		} else {
			repository.add(value);
		}
		return false;
	}
}
