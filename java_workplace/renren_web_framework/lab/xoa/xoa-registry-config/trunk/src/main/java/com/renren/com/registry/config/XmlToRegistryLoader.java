package com.renren.com.registry.config;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.impl.zookeeper.RegistryHelper;
import com.renren.xoa.registry.impl.zookeeper.ZookeeperBasedRegistry;

/**
 * 将本地的/conf/xoa-services.xml配置文件写入到zookeeper中
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-25 下午06:38:43
 */
public class XmlToRegistryLoader {

    private static final String CONFIG_FILE_PATH = "conf/xoa-services.xml";

    private RegistryHelper helper = new RegistryHelper();

    private ZookeeperBasedRegistry registry;

    private BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

    public XmlToRegistryLoader() {
        registry = new ZookeeperBasedRegistry();
        registry.init();
    }

    public void run() {
        List<XoaService> services = loadFromXml(CONFIG_FILE_PATH);
        addNodes(filterToBeAdded(services));
        updateNodes(filterToBeUpdated(services));
        deleteNodes(filterToBeDeleted(services));
    }

    /**
     * 删除指定节点
     * @param toBeDeleted
     */
    private void deleteNodes(List<XoaServiceDescriptor> toBeDeleted) {

        if (toBeDeleted == null || toBeDeleted.size() == 0) {
            return;
        }
        for (XoaServiceDescriptor node : toBeDeleted) {

            StringBuilder sb = new StringBuilder();
            sb.append("Node '");
            sb.append(node);
            sb.append("' will be deleted. \r\nInput 'delete' to continue, 'no' to skip this step [delete/no]>");
            System.out.print(sb.toString());

            boolean done = false;
            while (!done) {
                String command = readlineFromConsole();
                command = command.trim();
                if (command.equals("delete")) {
                    boolean succ = helper.deleteServiceNode(node);
                    if (succ) {
                        System.out.println("Node successfully deleted: " + node);
                    } else {
                        System.err.println("Fail to delete node: " + node);
                    }
                    done = true;
                } else if (command.equals("no")) {
                    System.out.println("Delete operation is skipped.");
                    done = true;
                } else {
                    System.out.print("[delete/no]>");
                }
            }
        }
    }

    /**
     * 更新指定节点
     * 
     * @param toBeUpdated
     */
    private void updateNodes(List<XoaServiceDescriptor> toBeUpdated) {

        if (toBeUpdated == null || toBeUpdated.size() == 0) {
            return;
        }

        for (XoaServiceDescriptor node : toBeUpdated) {
            
            //判断当前的操作是enable还是disable
            String op = node.isDisabled() ? "disable" : "enable";
            
            StringBuilder sb = new StringBuilder();
            sb.append("Node '");
            sb.append(node);
            sb.append("' will be ");
            sb.append(op);
            sb.append("d. \r\nInput '");
            sb.append(op);
            sb.append("' to continue, 'no' to skip this step [");
            sb.append(op);
            sb.append("/no]>");
            System.out.print(sb.toString());

            boolean done = false;
            while (!done) {
                String command = readlineFromConsole();
                command = command.trim();
                if (command.equals(op)) {

                    boolean succ = helper.updateServiceNode(node);
                    if (succ) {
                        System.out.println("Node successfully " + op + "d: " + node);
                    } else {
                        System.err.println("Fail to " + op + " node: " + node);
                    }
                    done = true;
                } else if (command.equals("no")) {
                    System.out.println("Update operation is skipped.");
                    done = true;
                } else {
                    System.out.print("[" + op + "/no]>");
                }
            }
        }
    }

    /**
     * 添加指定节点
     * 
     * @param toBeAdded
     */
    private void addNodes(List<XoaServiceDescriptor> toBeAdded) {

        if (toBeAdded == null || toBeAdded.size() == 0) {
            return;
        }

        StringBuilder sb = new StringBuilder();
        sb.append("The following node will be added into registry:\r\n");
        for (XoaServiceDescriptor node : toBeAdded) {
            sb.append(node);
            sb.append("\r\n");
        }
        sb.append("Input 'yes' to continue, 'no' to skip this step [yes/no]>");
        System.out.print(sb.toString());

        boolean done = false;

        while (!done) {
            String command = readlineFromConsole();
            command = command.trim();
            if (command.equals("yes")) {

                for (XoaServiceDescriptor node : toBeAdded) {
                    boolean succ = helper.registerServiceNode(node);
                    if (succ) {
                        System.out.println("Node successfully added into registry: " + node);
                    } else {
                        System.err.println("Fail to add node into registry: " + node);
                    }
                }
                done = true;
            } else if (command.equals("no")) {
                System.out.println("Operation is skipped. No node was added into registry.");
                done = true;
            } else {
                System.out.print("[yes/no]>");
            }
        }
    }

    /**
     * 从控制台读一行输入
     * 
     * @return
     */
    private String readlineFromConsole() {
        try {
            return in.readLine();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private List<XoaServiceDescriptor> filterToBeDeleted(List<XoaService> services) {
        System.out.println("Seaching for node to be deleted...");
        List<XoaServiceDescriptor> ret = new ArrayList<XoaServiceDescriptor>();

        Map<String, List<XoaServiceDescriptor>> serviceMap = new HashMap<String, List<XoaServiceDescriptor>>();
        for (XoaService service : services) {
            serviceMap.put(service.getServiceId(), service.getNodes());
        }

        List<XoaService> servicesInRegistry = registry.getServices();
        for (XoaService serviceInRegistry : servicesInRegistry) {

            List<XoaServiceDescriptor> nodes = serviceMap.get(serviceInRegistry.getServiceId());
            List<XoaServiceDescriptor> nodesInRegistry = serviceInRegistry.getNodes();
            if (nodes == null) { //整个服务都删掉了

                //每个节点都得删除
                for (XoaServiceDescriptor nodeInReg : nodesInRegistry) {
                    ret.add(nodeInReg);
                }
            } else {
                for (XoaServiceDescriptor nodeInReg : nodesInRegistry) {
                    boolean found = false;
                    for (XoaServiceDescriptor node : nodes) { //n*n的遍历查找
                        if (nodeInReg.getIpAddress().equals(node.getIpAddress())
                                && nodeInReg.getPort() == node.getPort()) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        ret.add(nodeInReg);
                    }
                }
            }
        }
        return ret;
    }

    private List<XoaServiceDescriptor> filterToBeUpdated(List<XoaService> services) {
        System.out.println("Seaching for node to be updated...");
        List<XoaServiceDescriptor> ret = new ArrayList<XoaServiceDescriptor>();
        for (XoaService svc : services) {
            for (XoaServiceDescriptor node : svc.getNodes()) {
                XoaServiceDescriptor nodeInRegistry = helper.getNodeConfig(node.getServiceId(),
                        node.getIpAddress(), node.getPort());
                if (nodeInRegistry != null && nodeInRegistry.isDisabled() != node.isDisabled()) {
                    ret.add(node);
                }
            }
        }
        return ret;
    }

    /**
     * 
     * @param services
     * @return
     */
    private List<XoaServiceDescriptor> filterToBeAdded(List<XoaService> services) {
        System.out.println("Seaching for node to be added...");
        List<XoaServiceDescriptor> ret = new ArrayList<XoaServiceDescriptor>();
        for (XoaService svc : services) {
            for (XoaServiceDescriptor node : svc.getNodes()) {
                if (!helper.exists(node)) {
                    ret.add(node);
                }
            }
        }
        return ret;
    }

    private List<XoaService> loadFromXml(String filePath) {
        try {
            List<XoaService> services = ConfigUtil.readFromXml(new FileInputStream(filePath));
            System.out.println(services.size() + " services loaded from xml");
            return services;
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }

    }

    public void destroy() {
        helper.destroy();
        registry.destroy();
        try {
            in.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws Exception {

        XmlToRegistryLoader app = new XmlToRegistryLoader();
        app.run();

        System.out.println();
        System.out.println("done");
    }

}
