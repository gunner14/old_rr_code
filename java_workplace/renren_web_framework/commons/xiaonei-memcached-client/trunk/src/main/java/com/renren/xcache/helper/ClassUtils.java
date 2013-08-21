package com.renren.xcache.helper;

public class ClassUtils {

    public static Object newInstanceOfClass(String className) {
        try {
            return Class.forName(className).newInstance();
        } catch (Exception e) {
            throw new NoClassDefFoundError(className);
        }
    }
}
