package com.renren.fmwidget.utils;

import java.lang.reflect.Method;
import java.util.Iterator;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.renren.fmwidget.model.Pos;
import com.renren.fmwidget.model.StickyModel;
import com.renren.fmwidget.model.StickyModel.StickyCustomDatas;
import com.renren.fmwidget.model.StickyModel.StickySystemDatas;

public class JSONCover {

	/**
	 * fromObject<br>
	 * 
	 * @param object
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Sep 2, 2010 - 9:37:21 PM
	 */
	public static JSONObject fromObject(Object object) {
		if (null == object) {
			return new JSONObject();
		}
		Method[] methods = object.getClass().getMethods();
		JSONObject jo = new JSONObject();
		for (Method method : methods) {
			try {
				if (method.getName().startsWith("get")
						&& !method.getName().equals("getClass")) {
					String mName = m2kName(method.getName());
					Object o = method.invoke(object, new Object[] {});
					if (null == o) {
						jo.put(mName, new JSONObject());
					} else if (o instanceof Byte) {
						jo.put(mName, o);
					} else if (o instanceof Integer) {
						jo.put(mName, o);
					} else if (o instanceof Long) {
						jo.put(mName, o);
					} else if (o instanceof Float) {
						jo.put(mName, o);
					} else if (o instanceof Double) {
						jo.put(mName, o);
					} else if (o instanceof Character) {
						jo.put(mName, o);
					} else if (o instanceof String) {
						jo.put(mName, o);
					} else if (o instanceof Boolean) {
						jo.put(mName, o);
					} else if (o instanceof Iterable<?>){
						JSONArray ja = new JSONArray();
						for(Object oi : (Iterable<?>)o){
							JSONObject subJo = fromObject(oi);
							if (!subJo.isEmpty() && !subJo.isNullObject()) {
								ja.add(subJo);
							}
						}
						jo.put(mName, ja);
					} else {
						JSONObject subJo = fromObject(o);
						if (!subJo.isEmpty() && !subJo.isNullObject()) {
							jo.put(mName, subJo);
						}
					}

				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return jo;
	}

	/**
	 * toObject<br>
	 * 
	 * @param <T>
	 * @param jo
	 * @param obj
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Sep 2, 2010 - 11:42:03 PM
	 */
	public static <T> T toObject(JSONObject jo, final Class<T> clazz) {
		if (jo.isArray() || jo.isEmpty() || jo.isNullObject()) {
			return null;
		}
		T obj;
		try {
			obj = (T) clazz.newInstance();
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		for (Iterator<?> iterator = jo.keys(); iterator.hasNext();) {
			String jkey = (String) iterator.next();
			Object jvalue = null;
			try {
				Class<?> subClazz = fromKeyGetType(jkey, obj);
				if (subClazz == Boolean.class || subClazz == boolean.class) {
					jvalue = jo.getBoolean(jkey);
				} else if (subClazz == Double.class || subClazz == double.class) {
					jvalue = jo.getDouble(jkey);
				} else if (subClazz == Integer.class || subClazz == int.class) {
					jvalue = jo.getInt(jkey);
				} else if (subClazz == Long.class || subClazz == long.class) {
					jvalue = jo.getLong(jkey);
				} else if (subClazz == String.class) {
					jvalue = jo.getString(jkey);
				} else if (subClazz == null){
					continue;
				} else {
					Class<?> realSubClazz = subClazz;
					for (Class<?> c : clazz.getClasses()) {
						if (c.getSuperclass().equals(subClazz)) {
							realSubClazz = c;
						}
					}
					JSONObject jovalue = jo.getJSONObject(jkey);
					jvalue = toObject(jovalue, realSubClazz);
				}
				String mName = k2mName(jkey);

				if (null != jvalue) {
					Method setMethod = null;
					for (Method m : clazz.getMethods()) {
						if (m.getName().equals(mName)) {
							setMethod = m;
						}
					}
					if (null != setMethod) {
						setMethod.invoke(obj, jvalue);
					}
				}
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
				continue;
			} catch (Exception e) {
				e.printStackTrace();
				continue;
			}
		}
		return obj;
	}

	private static String k2mName(String key) {
		char[] name = key.toCharArray();
		name[0] = Character.toUpperCase(name[0]);
		return "set" + String.valueOf(name);
	}

	private static Class<?> fromKeyGetType(String key, Object obj)
			throws NoSuchMethodException, SecurityException {
		char[] name = key.toCharArray();
		name[0] = Character.toUpperCase(name[0]);
		String gM = "set" + String.valueOf(name);
		Method method = null;
		for (Method m : obj.getClass().getMethods()) {
			if (gM.equals(m.getName())) {
				method = m;
			}
		}
		if (null != method) {
			return method.getParameterTypes()[0];
		} else {
			return null;
		}
	}

	private static String m2kName(String mname) {
		char[] get = { 'g', 'e', 't' };
		char[] name = mname.toCharArray();
		char[] rtname = new char[name.length - 3];
		for (int i = 0; i < name.length; i++) {
			char c = name[i];
			if (i < 3) {
				if (c != get[i]) {
					return "";
				} else {
					continue;
				}
			}
			if (i == 3) {
				rtname[i - 3] = Character.toLowerCase(c);
			} else {
				rtname[i - 3] = c;
			}

		}
		return String.valueOf(rtname);
	}

	public static void main(String[] args) {
		StickyModel m = new StickyModel();
		StickySystemDatas config = new StickySystemDatas();
		m.setSystemDatas(config);
		StickyCustomDatas content = new StickyCustomDatas();
		content.setContent("hello world");
		m.setCustomDatas(content);
		Pos position = new Pos();
		m.getSystemDatas().setPos(position);
		StickyModel m2 = toObject(fromObject(m), StickyModel.class);
		System.out.println(fromObject(m2));

	}
}
