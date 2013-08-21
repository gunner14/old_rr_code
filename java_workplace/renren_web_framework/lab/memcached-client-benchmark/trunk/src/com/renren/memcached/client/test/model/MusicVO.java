package com.renren.memcached.client.test.model;


import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;



public abstract class MusicVO<N extends Number & Comparable<N>> //
		implements Serializable, Comparable<MusicVO<N>> {

	public static final String COMMON_ID_FIELD_NAME = "id";

	//================================俩子类,懒得拆出去
	/**
	 * id为int型的子类
	 * 
	 * @author MaGicalL
	 */
	public static abstract class MusicVOInt extends MusicVO<Integer> {

		private static final long serialVersionUID = -4836564007759906530L;

		public void setId(final Integer id) {
			putId(id);
		}

		public Integer getId() {
			return id();
		}
	}

	/**
	 * id为long型的子类
	 * 
	 * @author MaGicalL
	 */
	public static abstract class MusicVOLong extends MusicVO<Long> {

		private static final long serialVersionUID = 2919995429479608131L;

		public void setId(final Long id) {
			putId(id);
		}

		public Long getId() {
			return id();
		}
	}

	//===================================================
	protected N id;

	//---------------------------------------------------
	private MusicVO() {
	}

	//---------------------------------------------------

	/**
	 * <pre>
	 * 如果子类不能根据id来比较两个BaseVO,比如说根本没有id这个字段,那么应该重写此方法,使用别的关键字段来比较 
	 * 此方法会被compareTo和equals调用
	 * </pre>
	 * 
	 * @return
	 */
	protected int compareFieldsWithoutId(final MusicVO<N> o) {
		return 0;
	}

	@Override
	public int compareTo(final MusicVO<N> o) {
		final N id = id();
		final N oid = o.id();
		if ((id == null) || (oid == null)) {
			return compareFieldsWithoutId(o);
		}
		return id.compareTo(oid);
	}

	public final N id() {
		return id;
	}

	public void putId(final N id) {
		this.id = id;
	}

	//该死的JavaBean规范,让我不能用泛型在父类上实现这个方法,只能下放到子类去
//	public void setId(N id) {
//		this.id = id;
//	}


	private transient Map<String, Object> calMap;

	private void checkCalMap() {
		if (calMap == null) {
			calMap = new HashMap<String, Object>();
		}
	}

	public void addCal(final String key, final int value) {
		checkCalMap();
		Integer o = (Integer) calMap.get(key);
		if (o == null) {
			o = 0;
		}
		calMap.put(key, o + value);
	}

	public void addCal(final String key, final long value) {
		checkCalMap();
		Long o = (Long) calMap.get(key);
		if (o == null) {
			o = 0L;
		}
		calMap.put(key, o + value);
	}

	public void addCal(final String key, final double value) {
		checkCalMap();
		Double o = (Double) calMap.get(key);
		if (o == null) {
			o = 0d;
		}
		calMap.put(key, o + value);
	}

	public Map<String, Object> calMap() {
		return calMap;
	}

	public void clearCalMap() {
		if (calMap != null) {
			calMap.clear();
		}
	}

	private static final long serialVersionUID = -8510059016854087454L;
}
