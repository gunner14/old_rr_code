package com.dodoyo.opt;

import java.lang.reflect.Field;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.MissingResourceException;
import java.util.ResourceBundle;
import java.util.concurrent.ConcurrentMap;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;

public class DodoyoResource {
	private static final String DEFAULT_RESOURCE_BASE_NAME = "application";

	private static final int INDEX_CACHE_KEY = 0;
	private static final int INDEX_RESOURCE_BUNDLE = 1;
	private volatile static Map<String, List<Object>> RESOURCE_BUNDLE_MAP = new HashMap<String, List<Object>>();

	private static RegLogger log = RegLogger
			.getLoggerWithType(DodoyoResource.class);

	public static ResourceBundle getResourceBundle(String basename) {
		ResourceBundle bundle = null;
		try {
			bundle = (ResourceBundle) RESOURCE_BUNDLE_MAP.get(basename).get(
					INDEX_RESOURCE_BUNDLE);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bundle;
	}

	public static void init(String resourceBaseName) {
		Object cacheKey = null;
		ResourceBundle resourceBundle = ResourceBundle
				.getBundle(resourceBaseName);
		try {
			Field fi = ResourceBundle.class.getDeclaredField("cacheKey");
			fi.setAccessible(true);
			cacheKey = fi.get(resourceBundle);
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		}
		List<Object> li = new ArrayList<Object>();
		li.add(INDEX_CACHE_KEY, cacheKey);
		li.add(INDEX_RESOURCE_BUNDLE, resourceBundle);
		RESOURCE_BUNDLE_MAP.put(resourceBaseName, li);
	}

	public static void close(String baseName) {
		if (RESOURCE_BUNDLE_MAP.get(baseName) == null) {
			return;
		}
		ResourceBundle resourceBundle = (ResourceBundle) RESOURCE_BUNDLE_MAP
				.get(baseName).get(INDEX_RESOURCE_BUNDLE);
		Object cacheKey = RESOURCE_BUNDLE_MAP.get(baseName)
				.get(INDEX_CACHE_KEY);
		try {
			Field fi = ResourceBundle.class.getDeclaredField("cacheList");
			fi.setAccessible(true);
			java.util.concurrent.ConcurrentMap<?, ?> cacheList = (ConcurrentMap<?, ?>) fi
					.get(resourceBundle);
			fi.setAccessible(true);
			cacheList.remove(cacheKey);
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		}
		RESOURCE_BUNDLE_MAP.remove(baseName);
	}

	public static String getString(String key, String basename) {
		try {
			ResourceBundle resourceBundle = (ResourceBundle) RESOURCE_BUNDLE_MAP
					.get(basename).get(INDEX_RESOURCE_BUNDLE);
			String value = resourceBundle.getString(key);
			log.debug("key: " + key + ",value: " + value);
			return value;
		} catch (MissingResourceException e) {
			RegLogger.getLoggerWithType(DodoyoResource.class).debug(
					"key: " + key + ",value: " + key);
			return key;
		} catch (NullPointerException e) {
			log.debug("key: " + key + ",value: " + "!" + key + "!");
			return "!" + key + "!";
		}
	}

	public static boolean isNotValue(String key, String value) {
		return StringUtils.equals(key, value)
				|| StringUtils.equals("!" + key + "!", value);
	}

	public static void reloadResource(String baseName) {
		synchronized (RESOURCE_BUNDLE_MAP) {
			DodoyoResource.close(baseName);
			DodoyoResource.init(baseName);
		}
	}

	/**
	 * 
	 */
	public static void init() {
		DodoyoResource.init(DEFAULT_RESOURCE_BASE_NAME);
	}

	/**
	 * @deprecated
	 */
	public static void close() {
		close(DEFAULT_RESOURCE_BASE_NAME);
	}

	/**
	 * 从资源文件中返回字符串 我们不希望程序崩溃，所以如果没有找到Key，就直接返回Key
	 */
	public static String getString(String key) {
		return getString(key, DEFAULT_RESOURCE_BASE_NAME);

	}

	/**
	 * 从资源文件中返回字符串 我们不希望程序崩溃，所以如果没有找到Key，就直接返回Key
	 */
	public static String getString(String key, Object... args) {

		try {
			return MessageFormat.format(getString(key), args);
		} catch (MissingResourceException e) {
			log.error(e.toString());
			return key;
		} catch (NullPointerException e) {
			log.error(e.toString());
			return "!" + key + "!";
		}
	}

	public static void main(String args[]) throws InterruptedException {
		DodoyoResource.init();
		System.out.println(DodoyoResource.getString("errors.cancel"));
		DodoyoResource.close();
		System.out.println(DodoyoResource.getString("errors.cancel"));
		// Thread.sleep(10000l);
		// DodoyoResource.init();
		// System.out.println(DodoyoResource.getString("errors.cancel"));
		// DodoyoResource.close();
		// System.out.println(DodoyoResource.getString("errors.cancel"));
	}
}
