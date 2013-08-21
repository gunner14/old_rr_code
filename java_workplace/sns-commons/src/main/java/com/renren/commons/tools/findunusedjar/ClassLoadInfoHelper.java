package com.renren.commons.tools.findunusedjar;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class ClassLoadInfoHelper {

    private final boolean deleteUnusedJars;

    private final List<String> whiteList;

    private final File dumpClassLoadLog;

    private final File jarsParent;

    //被扫描下的所有jar包，第一次加载后就不会改变
    private final List<String> allJars;

    //使用的jar包
    private final List<String> usedJars;

    //未使用的jar包
    private final List<String> unusedJars;

    //有些jar的名字可能已经变了
    private final Map<String, String> nameChangeJars;

    public ClassLoadInfoHelper(final String dumpClassLoadLog, final String jarsParentPath, final String whiteListStr, final boolean deleteUnusedJars) {
        this.dumpClassLoadLog = new File(dumpClassLoadLog);
        jarsParent = new File(jarsParentPath);
        whiteList = new LinkedList<String>();
        for (final String s : whiteListStr.split(",")) {
            whiteList.add(s);
        }
        allJars = new LinkedList<String>();
        usedJars = new LinkedList<String>();
        unusedJars = new LinkedList<String>();
        nameChangeJars = new HashMap<String, String>();
        this.deleteUnusedJars = deleteUnusedJars;
    }

    public void loadAllJars() {
        final File[] files = jarsParent.listFiles();
        int i = 0;
        for (final File f : files) {
            allJars.add(f.getName());
            unusedJars.add(f.getName());
            for (final String s : whiteList) {
                if (f.getName().startsWith(s)) {
                    unusedJars.remove(f.getName());
                    usedJars.add(f.getName());
                }
            }
            i++;
        }
        System.out.println("扫描目录成功，一共有jar包：" + i + "个，白名单的jar包：" + usedJars.size() + "个");
        try {
            Thread.sleep(5000);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void analysisLog() {
        System.out.println("开始分析class uploading信息..");
        final long time1 = System.currentTimeMillis();
        final List<ClassLoadInfoModel> models = this.getClassLoadInfoModels();
        for (final ClassLoadInfoModel model : models) {
            //如果jar包被更新了
            if (!allJars.contains(model.getJarName()) && !nameChangeJars.keySet().contains(model.getJarName())) {
                this.findJarByClassName(model);
            }
            if (model.getJarName() == null) {
                continue;
            }
            if (unusedJars.remove(model.getJarName())) {
                usedJars.add(model.getJarName());
            }
        }
        System.out.println("log分析完毕，花费时间:" + (System.currentTimeMillis() - time1));
        try {
            Thread.sleep(5000);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * @Author xuze(cantellow)
     * @Time Apr 26, 2012 12:10:14 PM
     * @param model
     * @Todo 根据类名查找jar包
     * @Modify
     */
    private void findJarByClassName(final ClassLoadInfoModel model) {
        try {
            final File[] files = jarsParent.listFiles();
            for (final File f : files) {
                final List<JarEntry> entryList = Collections.list(new JarFile(f).entries());
                for (final JarEntry entry : entryList) {
                    String entryN = entry.getName();
                    if (entryN.endsWith(".class")) {
                        entryN = entryN.substring(0, entryN.length() - 6);
                        entryN = entryN.replace('/', '.');
                        if (model.getClassName().equalsIgnoreCase(entryN)) {
                            if (model.getJarName() != null) {
                                nameChangeJars.put(model.getJarName(), f.getName());
                            }
                            model.setJarName(f.getName());
                            return;
                        }
                    }
                }
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
    }

    private List<ClassLoadInfoModel> getClassLoadInfoModels() {
        final List<ClassLoadInfoModel> models = new ArrayList<ClassLoadInfoModel>();
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(dumpClassLoadLog));
            String tempString = null;
            while ((tempString = reader.readLine()) != null) {
                //加一个try catch避免单次失败导致整个失败
                try {
                    if (tempString.length() == 0) {
                        continue;
                    }
                    final ClassLoadInfoModel model = this.getModelByLog(tempString);
                    if (model != null) {
                        models.add(model);
                    }
                } catch (final Exception e) {
                    e.printStackTrace();
                    continue;
                }
            }
        } catch (final Exception e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (final IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return models;
    }

    private ClassLoadInfoModel getModelByLog(final String oneClassUploadingLog) {
        final String[] temps = oneClassUploadingLog.split(",");
        if (temps.length != 3) {
            throw new IllegalArgumentException("这个URL不符合规则：" + oneClassUploadingLog);
        }
        //        if (temps[0].startsWith("$Proxy")) {
        //            return null;
        //        }
        final String temp = temps[1];
        //为null一般有两种情况，一种是<bootstrap>，另外一种是使用asm等动态生成的类
        if ("null".equalsIgnoreCase(temp)) {
            return null;
        }
        String jarName = null;
        final String[] temps2 = temp.split("/");
        for (final String s : temps2) {
            if (s.contains(".jar")) {
                if (s.endsWith("!")) {
                    jarName = s.substring(0, s.length() - 1);
                    break;
                } else {
                    jarName = s;
                    break;
                }
            }
        }
        return new ClassLoadInfoModel(temps[0], jarName, temps[2]);
    }

    private void deleteUnusedJars() {
        final File[] files = jarsParent.listFiles();
        for (final File f : files) {
            final String jarName = f.getName();
            if (unusedJars.contains(jarName)) {
                f.delete();
            }
        }
    }

    public void printUnusedJars() {
        System.out.println();
        System.out.println("程序执行完毕，使用的jar包一共有：" + usedJars.size() + "个");
        for (final String s : usedJars) {
            System.out.println(s);
        }
        System.out.println();
        System.out.println("未使用的jar包一共有：" + unusedJars.size() + "个");
        for (final String s : unusedJars) {
            System.out.println(s);
        }
        System.out.println();
        System.out.println("被更新的jar一共有：" + nameChangeJars.size() + "个");
        for (final Entry<String, String> e : nameChangeJars.entrySet()) {
            System.out.println(e.getKey() + "更新为：" + e.getValue());
        }
        if (deleteUnusedJars) {
            try {
                System.out.println("你选择了删除未使用的jar包，请保证你的容器已经停止运行，如果你没想好，你可以在10秒钟之内CTRL+C..");
                Thread.sleep(10000);
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
            this.deleteUnusedJars();
        }
    }
}
