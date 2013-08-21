package com.renren.xcache.impl.memcachedclient;

import java.util.HashMap;
import java.util.Map;

/**
 * 将memcached返回的数据类型cast成期望的类型。
 * 
 * 由于counter用String来存储，所以在get的时候直接cast会出问题，需要转化一下
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-28 下午05:42:22
 */
public class ValueCaster {
	
	private static Map<Class<?>, Caster<?>> casters = new HashMap<Class<?>, Caster<?>>();
	static {
		//init casters
		
		//for Byte
		casters.put(Byte.class, new Caster<Byte>() {
			@Override
			public Byte cast(String value) {
				return Byte.valueOf(value.trim());
			}
		});
		
		//for Boolean
		casters.put(Boolean.class, new Caster<Boolean>() {
			@Override
			public Boolean cast(String value) {
				return Boolean.valueOf(value.trim());
			}
		});
		
		//for Integer
		casters.put(Integer.class, new Caster<Integer>(){
			@Override
			public Integer cast(String value) {
				return Integer.valueOf(value.trim());
			}
		});
		
		//for Long
		casters.put(Long.class, new Caster<Long>(){
			@Override
			public Long cast(String value) {
				return Long.valueOf(value.trim());
			}
		});
		
		//for Float
		casters.put(Float.class, new Caster<Float>(){
			@Override
			public Float cast(String value) {
				return Float.valueOf(value.trim());
			}
		});
		
		//for Double
		casters.put(Double.class, new Caster<Double>(){
			@Override
			public Double cast(String value) {
				return Double.valueOf(value.trim());
			}
		});
		
		//for Short
		casters.put(Short.class, new Caster<Short>(){
			@Override
			public Short cast(String value) {
				return Short.valueOf(value.trim());
			}
		});
		
		//for Character
		casters.put(Character.class, new Caster<Character>(){
			@Override
			public Character cast(String value) {
				return Character.valueOf(value.trim().charAt(0));
			}
		});
	}
	
	@SuppressWarnings("unchecked")
	public <T> T cast(Class<T> expectedValueClass, Object value) {
		//如果返回值类型为String但是期望的类型不是String，那么就要试图做转化
		if (value instanceof String && !expectedValueClass.equals(String.class)) {
			Caster<?> caster = casters.get(expectedValueClass);
			if (caster != null) {
				return (T)caster.cast((String) value);
			}
		}
		return (T)value;
	}

	public static interface Caster<T> {
		public T cast(String value);
	}
	
}
