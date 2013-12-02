package com.renren.xcs.optool;

import java.io.IOException;

import org.apache.log4j.PropertyConfigurator;
import org.apache.zookeeper.KeeperException;

import com.renren.ad.datacenter.find.optool.ShellMain;
import com.renren.xoa2.server.conf.Configuration;
import com.renren.xoa2.server.conf.ConfigurationFactory;
import com.renren.xoa2.server.impl.XoaServiceConfig;

/**
 * 对Find的optool的封装，用于自动化创建目录结构和删除目录结构 <br><br>
 * 1. 创建：<br>
 *      $command xcsCreate test.xcs.renren.com 1 2 <br>
 *      将会建立 <br>
 *      <i>/(root)/test.xcs.renren.com/1/0 <br>
 *      /(root)/test.xcs.renren.com/1/1</i><br>
 *      这里stat参数代表数量
 * <br><br>
 * 2. 删除：<br>
 *      $command xcsDelete test.xcs.renren.com 1 1 <br>
 *      将会删除 <br>
 *      <i>/(root)/test.xcs.renren.com/1/1</i><br>
 *      这里stat参数代表下标
 *      <br><br>
 *      $command xcsDelete test.xcs.renren.com 1 <br>
 *      将会删除
 *      <i>/(root)/test.xcs.renren.com/1</i><br>
 *      
 * 3. 通过 -D 传递 xoa.xml 和制定 xcs root<br>
 *      $commond xcsCreate -Dxoa.config="conf/xoa.xml" -Dxcs.root="sandbox"
 * 
 * <br>{@link http://jira.d.xiaonei.com/browse/XOASEC-103}<br>     
 *
 * TODO:
 * <dd> 1. 需要对optool的操作返回值进行判断
 * <dd> 2. 提供quota的设置接口
 * 
 * @author xun.dai / yuan.liu1
 */
public class CommandLineConsole extends ShellMain {

    public final static String XCS_CREATE_COMMAND = "xcsCreate";
    public final static String XCS_DELETE_COMMAND = "xcsDelete";

    static {
        commandSet.add(XCS_CREATE_COMMAND);
        commandSet.add(XCS_DELETE_COMMAND);
    }

    public CommandLineConsole(String[] args) throws IOException, InterruptedException {
        super(args);
    }

    private void xcsUsage() {
        System.out.println(" XCS OPERATIONS\n\n");
        System.out.println("\t " + XCS_CREATE_COMMAND + '/' + XCS_DELETE_COMMAND
                + " [service [version [state]]] -- XCS wrapper for create/delete\n");
    }

    @Override
    protected void processCmd(MyCommandOptions co) throws KeeperException, InterruptedException,
            IOException {
        String[] args = co.getArgArray();
        String cmd = co.getCommand();
        
        String service = null;
        
        String xcsRoot = "test";
        int    xcsVersion = 1;
        int    xcsState = 1;

        String rootProperty = System.getProperty("xcs.root");
        if (rootProperty != null) {
            xcsRoot = rootProperty;
        }

        String confProperty = System.getProperty("xoa.config");
        if (confProperty != null) {
            Configuration conf = ConfigurationFactory.newConfiguration(confProperty);
            XoaServiceConfig xoaConf = conf.listServiceConfig().get(0);
            service = xoaConf.getServiceId();
            xcsVersion = xoaConf.getServiceVersion();
            xcsState = xoaConf.getServiceShard() + 1;
        } else if (args.length > 1) {
            service = args[1];
            if (args.length > 2) {
                xcsVersion = Integer.parseInt(args[2]);
            }
            if (args.length > 3) {
                xcsState = Integer.parseInt(args[3]);
            }            
        } else if (args.length <= 1 || !commandSet.contains(cmd)) {
            usage();
            xcsUsage();
            return;
        }
        
        if (cmd.equals(XCS_CREATE_COMMAND)) {
            int index = 0;
            while (index < xcsState) {
                String path = new StringBuffer().append("/").append(xcsRoot)
                        .append("/").append(service)
                        .append("/").append(xcsVersion)
                        .append("/").append(index).toString();
                
                String returnStr = optool.create(super.genAbstractPath(path), null, true);
                if (returnStr != null) {
                    System.out.println("Created: " + returnStr);
                } else {
                    // 节点存在会被认为是合理行为，可以用于服务追加逻辑
                    System.err.println("Failed to create " + path + ". Node existed.");
                }
                index ++;
            }

        } else if (cmd.equals(XCS_DELETE_COMMAND)) {

            StringBuffer sb = new StringBuffer().append("/").append(xcsRoot)
                    .append("/").append(service);
            
            if (confProperty == null) {
                if (args.length > 2) 
                    sb.append("/").append(xcsVersion);
                if (args.length > 3) 
                    sb.append("/").append(xcsState);
            } else {
                sb.append("/").append(xcsVersion);
                sb.append("/").append(xcsState - 1);                
            }
            
            optool.delete(sb.toString(), true);
            
        } else {
            super.processCmd(co);
        }

    }

    /**
     * 执行XCS路径创建/删除操作
     * 
     * @throws IOException
     * @throws KeeperException
     * @throws InterruptedException
     */
    public void process() throws IOException, KeeperException, InterruptedException {
        run();
    }
    
    public static void main(String[] args) throws IOException, KeeperException,
            InterruptedException {
        CommandLineConsole main = new CommandLineConsole(args);
        PropertyConfigurator.configure("conf/log4jOP.properties");
        main.process();
    }
}
