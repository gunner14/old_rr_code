package com.renren.commons.tools.findunusedjar;

public class ClassLoadInfoModel {

    private String className;

    private String jarName;

    private String classLoaderName;

    public ClassLoadInfoModel(final String className, final String jarName, final String classLoaderName) {
        this.className = className;
        this.jarName = jarName;
        this.classLoaderName = classLoaderName;
    }

    public String getClassName() {
        return className;
    }

    public void setClassName(final String className) {
        this.className = className;
    }

    public String getJarName() {
        return jarName;
    }

    public void setJarName(final String jarName) {
        this.jarName = jarName;
    }

    public String getClassLoaderName() {
        return classLoaderName;
    }

    public void setClassLoaderName(final String classLoaderName) {
        this.classLoaderName = classLoaderName;
    }

}
