package com.renren.commons.tools.findunusedjar.classloadlog;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Apr 15, 2012 3:59:16 PM
 * @Who 通过加上jvm参数:-verbose:class查看class装载信息，从而找到未使用的jar包
 * @Modify
 */
public class FindUnusedJars {

    private final boolean deleteUnusedJar;

    private final File classUploadLog;

    private final File jars;

    private final List<String> usedJars;

    private final List<String> unusedJars;

    private final List<String> whiteList;

    private final List<String> whiteListJars;

    public FindUnusedJars(final String classUploadLogStr, final String jarsPath, final boolean deleteUnusedJar, final String whiteListPath) {
        classUploadLog = new File(classUploadLogStr);
        jars = new File(jarsPath);
        this.deleteUnusedJar = deleteUnusedJar;
        usedJars = new LinkedList<String>();
        unusedJars = new LinkedList<String>();
        whiteList = new ArrayList<String>();
        whiteListJars = new LinkedList<String>();
        this.initWhiteList(whiteListPath);
    }

    private void initWhiteList(final String whiteListPath) {
        final File whiteListF = new File(whiteListPath);
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(whiteListF));
            String tempString = null;
            while ((tempString = reader.readLine()) != null) {
                whiteList.add(tempString);
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
    }

    private void loadAllJars() {
        final File[] files = jars.listFiles();
        for (final File f : files) {
            unusedJars.add(f.getName());
            for (final String s : whiteList) {
                if (f.getName().startsWith(s)) {
                    whiteListJars.add(f.getName());
                }
            }
        }
        for (final String s : whiteListJars) {
            unusedJars.remove(s);
            usedJars.add(s);
        }
        System.out.println("扫描目录成功，一共有jar包：" + unusedJars.size() + "个");
        try {
            Thread.sleep(5000);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void analysisLog() {
        System.out.println("开始分析class uploading信息..");
        final long time1 = System.currentTimeMillis();
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(classUploadLog));
            String tempString = null;
            while ((tempString = reader.readLine()) != null) {
                //加一个try catch避免单次失败导致整个失败
                try {
                    if (tempString.length() == 0) {
                        continue;
                    }
                    if (!this.isClassUploadingLog(tempString)) {
                        continue;
                    }
                    final String jarName = this.getLoadJarName(tempString);
                    if (jarName == null) {
                        continue;
                    }
                    if (unusedJars.remove(jarName)) {
                        usedJars.add(jarName);
                        //System.out.println("this jar is used:" + jarName);
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
        System.out.println("log分析完毕，花费时间:" + (System.currentTimeMillis() - time1));
        try {
            Thread.sleep(5000);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    private boolean isClassUploadingLog(final String log) {
        if (log.startsWith("[Loaded")) {
            return true;
        }
        return false;
    }

    private String getLoadJarName(final String oneClassUploadingLog) {
        if (!oneClassUploadingLog.contains(" from ")) {
            //System.err.println(oneClassUploadingLog);
            return null;
        }
        final String[] tempStrs1 = oneClassUploadingLog.split(" from ");
        if (tempStrs1.length != 2) {
            //System.err.println(oneClassUploadingLog);
            return null;
        }
        final String temp1 = tempStrs1[1];
        if (!temp1.startsWith("file:") && !temp1.startsWith("jar:file:")) {
            //System.err.println(oneClassUploadingLog);
            return null;
        }
        final String[] tempStrs2 = temp1.split("file:");
        if (tempStrs2.length != 2) {
            //System.err.println(oneClassUploadingLog);
            return null;
        }
        final String temp2 = tempStrs2[1];
        final String[] tempStrs3 = temp2.split("/");
        for (final String s : tempStrs3) {
            if (s.contains(".jar")) {
                if (s.endsWith("!")) {
                    return s.substring(0, s.length() - 1);
                } else {
                    return s;
                }
            }
        }
        //System.err.println(oneClassUploadingLog);
        return null;
    }

    private void printUnusedJars() {
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
        System.out.println("白名单的jar包一共有：" + whiteListJars.size() + "个");
        for (final String s : whiteListJars) {
            System.out.println(s);
        }
    }

    private void deleteUnusedJars() {
        final File[] files = jars.listFiles();
        for (final File f : files) {
            final String jarName = f.getName();
            if (unusedJars.contains(jarName)) {
                f.delete();
            }
        }
    }

    public static void main(final String[] args) {
        if (args.length != 4) {
            throw new IllegalArgumentException("argument is wrong..");
        }
        try {
            System.out.println("为了debug方便，我先睡10秒哈..");
            Thread.sleep(10000);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        final String classUploadLogStr = args[0];
        final String jarsPath = args[1];
        final boolean deleteUnusedJar = Boolean.parseBoolean(args[2]);
        final String whiteList = args[3];
        final FindUnusedJars find = new FindUnusedJars(classUploadLogStr, jarsPath, deleteUnusedJar, whiteList);
        find.loadAllJars();
        find.analysisLog();
        find.printUnusedJars();
        if (find.deleteUnusedJar) {
            try {
                System.out.println("你选择了删除未使用的jar包，请保证你的容器已经停止运行，如果你没想好，你可以在10秒钟之内CTRL+C..");
                Thread.sleep(10000);
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
            find.deleteUnusedJars();
        }
    }
}
