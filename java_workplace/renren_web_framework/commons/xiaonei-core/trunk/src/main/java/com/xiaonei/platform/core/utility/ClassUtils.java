/*
 * Created on 2003-12-9
 */
package com.xiaonei.platform.core.utility;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.List;

/**
 * ���÷�����ơ��������л������⹹�췽�������Ķ���
 * ���ַ�ʽЧ�ʱȽϵͣ����ʺ�ͨ�����½��������
 * <br>
 * ���⹹�췽����(��
 * <li>������A(String)���췽�������������л�ΪXML�������ʽ����
 * �øù��췽���½�����ʵ��
 * <li>��Ĭ�Ϲ��췽���½�����ʵ���÷�����Ƹ����Ա��ֵ
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class ClassUtils {
	
	public static Object createInstance(String clsName) throws Exception {
		Class cls = Class.forName(clsName);
		
		return cls.newInstance();
	}
	
	public static Object createInstance(String clsName, Class[] paramTypes, Object[] params) throws Exception {
		Class cls = Class.forName(clsName);
		Constructor construct = cls.getConstructor(paramTypes);
		
		return construct.newInstance(params);
	}
	
	public static String serializeObject(Object obj) {
		return null;
	}
	
	public static Object constructObject(Class cls, String serial) {
		return null;
	}

	public static List getAllClassesAndInterfaces(Class startClass) {
		ArrayList classes = new ArrayList();		
		addClassesAndInterfaces(startClass, classes);
		
		return classes;
	}

	protected static void addClassesAndInterfaces(Class c, List classes) {
		if (c == null) {
			return;
		}
	
		Class superClass = c.getSuperclass();
		Class[] interfaces = c.getInterfaces();
		
		if (superClass != null && !classes.contains(superClass)) {
			//System.out.println("Adding " + superClass.getName() + " superclass of " + c.getName());
			classes.add(superClass);
		}
		
		for (int i = 0; i < interfaces.length; i++) {
			if(interfaces[i] != null && !classes.contains(interfaces[i])) {
				//System.out.println("Adding " + interfaces[i].getName() + " interface of " + c.getName());
				classes.add(interfaces[i]);
			}
		}
		
		addClassesAndInterfaces(superClass, classes);
		for (int i = 0; i < interfaces.length; i++) {
			addClassesAndInterfaces(interfaces[i], classes);
		}
	}

}
