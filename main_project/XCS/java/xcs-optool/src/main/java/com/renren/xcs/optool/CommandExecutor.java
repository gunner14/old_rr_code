/**
 * @(#)CommandExecutor.java, 2012-10-31. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.zookeeper.KeeperException;

import com.renren.ad.datacenter.find.optool.OPTool;
import com.renren.ad.datacenter.find.optool.ZKAuth;
import com.renren.xcs.NodeData;
import com.renren.xoa2.server.conf.Configuration;
import com.renren.xoa2.server.conf.ConfigurationFactory;
import com.renren.xoa2.server.impl.XoaServiceConfig;

/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public class CommandExecutor {

    /**
     * 
     */
    private static final String ZK_CLUSTER_TEST = "10.11.18.185";

    private OPTool optool;

    private String root = "test";

    private Map<String, Command> commands = new HashMap<String, Command>();

    public final static String CREATE_COMMAND = "create";

    public final static String DELETE_COMMAND = "delete";

    public final static String ENABLE_COMMAND = "enable";

    public final static String DISABLE_COMMAND = "disable";

    public final static int ZK_ERROR = 1;

    public final static int INVALID_COMMAND = 2;

    public final static int INVALID_PARAMETER = 3;

    public final static int INVALID_AUTH_STORE = 4;

    {
        commands.put(ENABLE_COMMAND, new AbstractCommand(optool) {

            public final static int INVALID_NODE_DATA = 10;

            @Override
            public void execute(String[] args) throws CommandException {
                if (args.length < 4) {
                    throw new CommandException(INVALID_PARAMETER, "Invalid Parameter. Useage: "
                            + ENABLE_COMMAND + " ${service} ${version} ${shard} ${node}");
                }
                String fullPath = "/" + root + "/"
                        + StringUtils.join(Arrays.copyOfRange(args, 0, 4), "/");
                try {
                    String data = optool.getData(fullPath);
                    NodeData nodeData;
                    try {
                        nodeData = NodeData.valueOf(data);
                    } catch (Exception e) {
                        optool.setData(fullPath,
                                new String(new NodeData(args[2], true, true).toBytes()));
                        throw new CommandException(INVALID_NODE_DATA, "Invalid node data: " + data
                                + ". Optool have repared it. Please try again.");
                    }
                    nodeData.setDisabled(false);
                    optool.setData(fullPath, new String(nodeData.toBytes()));
                } catch (KeeperException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                } catch (InterruptedException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                }
                System.out.println("Enable: " + fullPath);
            }
        });

        commands.put(DISABLE_COMMAND, new AbstractCommand(optool) {

            public final static int INVALID_NODE_DATA = 10;

            @Override
            public void execute(String[] args) throws CommandException {
                if (args.length < 4) {
                    throw new CommandException(INVALID_PARAMETER, "Invalid Parameter. Useage: "
                            + ENABLE_COMMAND + " ${service} ${version} ${shard} ${node}");
                }
                String fullPath = "/" + root + "/"
                        + StringUtils.join(Arrays.copyOfRange(args, 0, 4), "/");
                try {
                    String data = optool.getData(fullPath);
                    NodeData nodeData;
                    try {
                        nodeData = NodeData.valueOf(data);
                    } catch (Exception e) {
                        optool.setData(fullPath,
                                new String(new NodeData(args[2], true, true).toBytes()));
                        throw new CommandException(INVALID_NODE_DATA, "Invalid node data: " + data
                                + ". Optool have repared it. Please try again.");
                    }
                    nodeData.setDisabled(true);
                    optool.setData(fullPath, new String(nodeData.toBytes()));
                } catch (KeeperException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                } catch (InterruptedException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                }
                System.out.println("Disable: " + fullPath);
            }
        });

        commands.put(CREATE_COMMAND, new AbstractCommand(optool) {

            @Override
            public void execute(String[] args) throws CommandException {
                String service;
                int version;
                int shard;

                String confProperty = System.getProperty("xoa.config");

                if (confProperty != null) {
                    Configuration conf = ConfigurationFactory.newConfiguration(confProperty);
                    XoaServiceConfig xoaConf = conf.listServiceConfig().get(0);
                    service = xoaConf.getServiceId();
                    version = xoaConf.getServiceVersion();
                    shard = xoaConf.getServiceShard() + 1;
                } else if (args.length == 3) {
                    service = args[0];
                    try {
                        version = Integer.parseInt(args[1]);
                        shard = Integer.parseInt(args[2]);
                    } catch (NumberFormatException e) {
                        throw new CommandException(INVALID_PARAMETER, "Invalid version or shard.");
                    }
                } else {
                    throw new CommandException(INVALID_PARAMETER, "Invalid Parameter. Useage: "
                            + CREATE_COMMAND
                            + " ${service} ${version} ${shard}; Or -Dxoa.config=path/to/xoa.xml "
                            + CREATE_COMMAND);
                }

                String fullPath = new StringBuffer().append("/").append(root).append("/")
                        .append(service).append("/").append(version).append("/").append(shard)
                        .toString();
                try {
                    optool.create(fullPath, null, true);
                } catch (KeeperException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                } catch (InterruptedException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                }
                System.out.println("Created: " + fullPath);
            }

        });

        commands.put(DELETE_COMMAND, new AbstractCommand(optool) {

            @Override
            public void execute(String[] args) throws CommandException {

                StringBuffer pathBuffer = new StringBuffer().append("/").append(root);

                if (args.length > 0) {
                    pathBuffer.append("/").append(args[0]);
                    try {
                        if (args.length > 1) {
                            pathBuffer.append("/").append(Integer.parseInt(args[1]));
                        }
                        if (args.length > 2) {
                            pathBuffer.append("/").append(Integer.parseInt(args[2]));
                        }
                    } catch (NumberFormatException e) {
                        throw new CommandException(INVALID_PARAMETER, "Invalid version or shard.");
                    }
                } else {
                    throw new CommandException(
                            INVALID_PARAMETER,
                            "Invalid Parameter. Useage: "
                                    + DELETE_COMMAND
                                    + " ${service} [${version}] [${shard}]; Or -Dxoa.config=path/to/xoa.xml "
                                    + DELETE_COMMAND);
                }

                try {
                    optool.delete(pathBuffer.toString(), true);
                } catch (KeeperException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                } catch (InterruptedException e) {
                    throw new CommandException(ZK_ERROR, e.getMessage());
                }
                System.out.println("Delete: " + pathBuffer.toString());
            }

        });
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        if (args == null || args.length == 0) {
            System.err.println(INVALID_COMMAND + ": Invalid command.");
            System.exit(INVALID_COMMAND);
        }
        String command = args[0];
        String[] commandArgs = Arrays.copyOfRange(args, 1, args.length);

        try {
            CommandExecutor executor = new CommandExecutor();
            executor.initOptool();
            String rootProperty = System.getProperty("xcs.root");
            if (rootProperty != null) {
                executor.root = rootProperty;
            }

            Command c = executor.commands.get(command);
            c.execute(commandArgs);
        } catch (CommandException e) {
            System.err.println(e.getCode() + ": " + e.getMessage());
            System.exit(e.getCode());
        }
        System.exit(0);

    }

    private void initOptool() throws CommandException {
        String host = ZK_CLUSTER_TEST;
        String hostProperty = System.getProperty("xcs.zkserver");
        if (hostProperty != null) {
            host = hostProperty;
        }

        try {
            String authStore = System.getProperty("xcs.authstore");
            if(authStore == null){
                host = ZK_CLUSTER_TEST;
            }
            optool = new OPTool(host, 5000);
            if (authStore != null) {
                // 线上环境
                initAuth(authStore);
            } else {
                // 测试环境
                optool.addAuthInfo("test:test");
                optool.authOpDigest = ZKAuth.generateDigest("test:test");
            }
        } catch (Exception e) {
            System.err.println("Init Optool failed: " + e.getMessage());
        }

    }

    private void initAuth(String authFile) throws CommandException {
        FileReader fr = null;
        BufferedReader br = null;
        try {
            fr = new FileReader(authFile);
            br = new BufferedReader(fr);
            String line = br.readLine();
            while (line != null) {
                String[] strs = line.split(":");
                if (line.startsWith("server")) {
                    optool.authServerDigests.add(optool.new AuthList(strs[1], strs[2], strs[3]));
                } else if (line.startsWith("client")) {
                    optool.authClientDigests.add(optool.new AuthList(strs[1], strs[2], strs[3]));
                } else if (line.startsWith("operator")) {
                    optool.addAuthInfo(strs[1] + ":" + strs[2]);
                    try {
                        optool.authOpDigest = ZKAuth.generateDigest(strs[1] + ":" + strs[2]);
                    } catch (NoSuchAlgorithmException e) {

                    }
                }
                line = br.readLine();
            }
        } catch (Exception e) {
            throw new CommandException(INVALID_AUTH_STORE, "Invalid Auth file.", e);
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
                if (fr != null) {
                    fr.close();
                }
            } catch (IOException e) {
                System.err.println(e.getMessage());
            }
        }
    }

}
