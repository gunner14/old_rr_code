package com.renren.commons.tools.findunusedjar;

public class FindUnusedJarsMain {

    /**
     * @Author xuze(cantellow)
     * @Time Apr 17, 2012 12:40:37 PM
     * @param args
     * @Todo main参数第一个是要分析的类加载信息文件路径,第二个是要扫描的jar包文件夹路径,第三个是以逗号分隔开的白名单jar包，
     *       第四个是是否删除这些jar包
     * @Modify
     */
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
        //开始dumping class信息
        //开始分析这些class信息
        final ClassLoadInfoHelper helper = new ClassLoadInfoHelper(args[0], args[1], args[2], Boolean.parseBoolean(args[3]));
        helper.loadAllJars();
        helper.analysisLog();
        helper.printUnusedJars();
    }
}
