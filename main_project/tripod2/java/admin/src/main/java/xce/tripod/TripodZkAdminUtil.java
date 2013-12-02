package xce.tripod;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

public class TripodZkAdminUtil {

    public static void reloadZkConfig(String jsonPath, String backUpDir, String zkAddress)
            throws Exception {
        exportOnlineZkConfig(backUpDir, zkAddress);
        updateZkConfig(getConfigFromJson(jsonPath), zkAddress);
    }

    public static TripodConfig exportOnlineZkConfig(String backUpDir, String zkAddress)
            throws Exception {
        TripodConfig onlineConfig = getConfigFromZk(zkAddress);
        SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss");
        String dumpPath = backUpDir + "_" + df.format(new Date()) + ".json";
        dumpConfig(onlineConfig, dumpPath);
        IOStreamUtil.print("目前线上配置已备份,备份文件" + dumpPath);
        return onlineConfig;
    }

    public static void cutServerOff(List<String> toBeOffServers, String zkAddress,
            String backUpDir, String jsonPath) throws Exception {
        checkJsonAndOnlineSame(zkAddress, jsonPath);
        exportOnlineZkConfig(backUpDir, zkAddress);
        IOStreamUtil.makeSureWithPromt("希望切下：\n" + toBeOffServers);
        TripodConfig config = getConfigFromZk(zkAddress);
        config.cutServer(toBeOffServers);
        updateZkConfig(config, zkAddress);
        IOStreamUtil.print(toBeOffServers + " 已下线");
        dumpConfig(getConfigFromZk(zkAddress), jsonPath + "_off_" + toBeOffServers);
        IOStreamUtil.print("下线后配置已备份在" + jsonPath + "_off_" + toBeOffServers);
    }

    public static void addServerOn(String zkAddress, String namespace, String serverType,
            String toBeAddServer, String backUpDir, String jsonPath) throws Exception {
        checkJsonAndOnlineSame(zkAddress, jsonPath);
        exportOnlineZkConfig(backUpDir, zkAddress);
        TripodConfig config = getConfigFromZk(zkAddress);
        config.addServer(namespace, serverType, toBeAddServer);
        updateZkConfig(config, zkAddress);
        dumpConfig(getConfigFromZk(zkAddress), jsonPath + "_add_" + toBeAddServer);
        IOStreamUtil.print("下线后配置已备份在" + jsonPath + "_add_" + toBeAddServer);
    }

    public static void cutHalf(String zkAddress, String namespace, String serverType,
            String backUpDir, String jsonPath) throws Exception {
        checkJsonAndOnlineSame(zkAddress, jsonPath);
        exportOnlineZkConfig(backUpDir, zkAddress);
        TripodConfig config = getConfigFromZk(zkAddress);
        config.cutHalf(namespace, serverType);
        updateZkConfig(config, zkAddress);
        dumpConfig(getConfigFromZk(zkAddress), jsonPath + "_cut_half");
        IOStreamUtil.print("下线后配置已备份在" + jsonPath + "_cut_half");
    }

    public static void checkJsonAndOnlineSame(String zkAddress, String jsonPath) throws Exception {
        TripodConfig onlineConfig = getConfigFromZk(zkAddress);
        TripodConfig jsonConfig = getConfigFromJson(jsonPath);
        TripodConfig off = onlineConfig.removeAll(jsonConfig);
        TripodConfig add = jsonConfig.removeAll(onlineConfig);
        if (off.entryMap.size() > 0 || add.entryMap.size() > 0) {
            IOStreamUtil.print("当前配置文件 " + jsonPath + " 配置与线上不符，请查看!!!");
            IOStreamUtil.print("OFF:\n" + off);
            IOStreamUtil.print("ADD:\n" + add);
            System.exit(1);
        }
    }

    static TripodConfig getConfigFromJson(String jsonPath) throws Exception {
        return TripodConfig.praseFromJson(jsonPath);
    }

    static void dumpConfig(TripodConfig config, String dumpPath) throws Exception {
        config.dumpToJson(dumpPath);
    }

    //兼容之前的xml工具
    static void dumpToXml(String zkAddress, String xmlPath) throws Exception {
        ZkWrapper.dumpToXml(zkAddress, xmlPath);
        IOStreamUtil.print("目前线上xml配置文件 " + xmlPath + " 已更新为最新配置");
    }

    public static void backupToXml(String zkAddress, String backUpDir) throws Exception {
        SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss");
        String dumpPath = backUpDir + "__" + df.format(new Date()) + ".xml";
        ZkWrapper.dumpToXml(zkAddress, dumpPath);
        IOStreamUtil.print("目前线上配置已备份,备份文件" + dumpPath);
    }

    static TripodConfig getConfigFromZk(String zkAddress) throws Exception {
        return ZkWrapper.getTripodConfigFromZk(zkAddress);
    }

    static void updateZkConfig(TripodConfig newConfig, String zkAddress) throws Exception {
        ZkWrapper.updateZkConfig(newConfig, zkAddress);
    }

    public static void main(String[] args) {
        try {
            ZkWrapper.createZkMutex(Constants.DEFAULT_ZK_ADDRESS);
            IOStreamUtil.print("Usage:");
            IOStreamUtil.print("1.reload zk_conf.json");
            IOStreamUtil.print("2.备份线上配置");
            IOStreamUtil.print("3.切机器/进程下线");
            IOStreamUtil.print("4.新增进程上线");
            IOStreamUtil.print("5.检查json文本与线上配置是否相符");
            IOStreamUtil.print("6.CUT HALF");
            IOStreamUtil.print("输入选项，回车确认！！：");
            char choice = IOStreamUtil.getChar();
            switch (choice) {
                case '1':
                    backupToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf");
                    reloadZkConfig(Constants.CONFIG_DIR + "zk_conf.json", Constants.CONFIG_DIR
                            + "backup/zk_conf_bak", Constants.DEFAULT_ZK_ADDRESS);
                    dumpToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR + "zk_conf.xml");
                    break;
                case '2':
                    exportOnlineZkConfig(Constants.CONFIG_DIR + "backup/zk_conf_bak",
                            Constants.DEFAULT_ZK_ADDRESS);
                    backupToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf");
                    break;
                case '3':
                    backupToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf");
                    IOStreamUtil.print("输入需要下线的机器或进程");
                    System.out
                            .println("换行为分界符,示例：cc1/cc1.xce/cc1.xce.d.xioanei.com/cc1.xce.d.xiaonei.com:6600");
                    IOStreamUtil.print("空行结束");
                    List<String> offMachines = new LinkedList<String>();
                    String str = IOStreamUtil.getString();
                    while (str.length() != 0) {
                        offMachines.add(new String(str));
                        str = IOStreamUtil.getString();
                    }
                    cutServerOff(offMachines, Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf_bak", Constants.CONFIG_DIR + "zk_conf.json");
                    dumpToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR + "zk_conf.xml");
                    break;
                case '4':
                    backupToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf");
                    IOStreamUtil.print("输入namespace，示例：xce_user");
                    String namespace = IOStreamUtil.getString();
                    IOStreamUtil.print("输入server类型，示例：" + ConfigEntry.CC + "/" + ConfigEntry.CM
                            + "/" + ConfigEntry.PM);
                    String serverType = IOStreamUtil.getString();
                    ConfigEntry.checkValidType(serverType);
                    IOStreamUtil.print("输入server，示例：cc1:cc1.xce.d.xiaonei.com:6601");
                    String server = IOStreamUtil.getString();
                    addServerOn(Constants.DEFAULT_ZK_ADDRESS, namespace, serverType, server,
                            Constants.CONFIG_DIR + "backup/zk_conf_bak", Constants.CONFIG_DIR
                                    + "zk_conf.json");
                    dumpToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR + "zk_conf.xml");
                    break;
                case '5':
                    checkJsonAndOnlineSame(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "zk_conf.json");
                    IOStreamUtil.print("配置符合");
                    break;
                case '6':
                    backupToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR
                            + "backup/zk_conf");
                    IOStreamUtil.print("危险！！！请仔细注意提示说明！！！");
                    IOStreamUtil.print("输入namespace，示例：xce_user");
                    String ns = IOStreamUtil.getString();
                    IOStreamUtil.print("输入server类型，示例：" + ConfigEntry.CC + "/" + ConfigEntry.CM
                            + "/" + ConfigEntry.PM);
                    String type = IOStreamUtil.getString();
                    ConfigEntry.checkValidType(type);
                    cutHalf(Constants.DEFAULT_ZK_ADDRESS, ns, type, Constants.CONFIG_DIR
                            + "backup/zk_conf_bak", Constants.CONFIG_DIR + "zk_conf.json");
                    dumpToXml(Constants.DEFAULT_ZK_ADDRESS, Constants.CONFIG_DIR + "zk_conf.xml");
                    break;
                default:
                    IOStreamUtil.print("选择无效，程序退出");
                    System.exit(1);
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.exit(0);
    }
}
