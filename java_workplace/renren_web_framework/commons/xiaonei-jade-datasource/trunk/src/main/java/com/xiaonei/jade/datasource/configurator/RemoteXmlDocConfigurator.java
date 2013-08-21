package com.xiaonei.jade.datasource.configurator;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;

import org.apache.log4j.BasicConfigurator;

import com.xiaonei.xce.storm.XceStormAdapter;

/**
 * 实现加载远程服务器的配置文件，并且可以监听文件改变。
 * 
 * @author han.liao
 */
public class RemoteXmlDocConfigurator extends XmlDocConfigurator {

    // 本地配置文件 - Linux
    public static final String FAILLOADING_PATH_LINUX = "/etc/jade-config/";

    // 本地配置文件 - Windows
    public static final String FAILLOADING_PATH_WINDOWS = "C:\\jade-config\\";

    // 标记文件名称
    public static final String MARKUP_FILE = "use-local-config";

    // 配置文件名称
    public static final String CONFIG_FILE = "jade-config.xml";

    // 订阅的唯一名称
    public static final String SUBSCRIBE_NAME = "XmlJadeConfig";

    // 命令参数名称
    protected static final String COMMAND = "#COMMAND";

//    // 订阅配置信息
//    protected XceStormListener stormListener = new XceStormListener() {
//
//        // serialVersionUID
//        private static final long serialVersionUID = -7019155420108873791L;
//
//        @Override
//        public void handle(Map<String, String> context) {
//
//            String command = context.remove(COMMAND);
//
//            if ("RELOAD".equalsIgnoreCase(command)) {
//
//                boolean loaded = false;
//
//                if (url != null) {
//
//                    // 输出日志
//                    if (logger.isInfoEnabled()) {
//                        logger.info("Reloading url [" + url + ']');
//                    }
//
//                    // 重新从网址加载配置信息
//                    loaded = loadXmlDoc(url, getLoadingFile(), true);
//
//                } else if (file != null) {
//
//                    // 输出日志
//                    if (logger.isInfoEnabled()) {
//                        logger.info("Reloading file [" + file.getPath() + ']');
//                    }
//
//                    // 重新从网址加载配置信息
//                    loaded = loadXmlDoc(getLoadingURL(), file, false);
//
//                } else {
//
//                    // 输出日志
//                    if (logger.isInfoEnabled()) {
//                        logger.info("ERROR: No configurated url or file.");
//                    }
//                }
//
//                // 输出日志
//                if (logger.isInfoEnabled()) {
//                    logger.info(loaded ? "Reload OK" : "ERROR: Reload FAILED!");
//                }
//            }
//        }
//    };

    /**
     * 创建: RemoteXmlDocXceConfigurator 配置。
     */
    public RemoteXmlDocConfigurator() {

        super(getLoadingURL(), getLoadingFile(), getPriority());

        // 订阅配置信息
        // XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 创建: RemoteXmlDocXceConfigurator 配置。
     * 
     * @param url - 配置网络路径
     */
    public RemoteXmlDocConfigurator(URL url) {

        super(url);

//        // 订阅配置信息
//        XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 配置: RemoteXmlDocXceConfigurator 对象。
     * 
     * @param file - 配置本地路径
     */
    public RemoteXmlDocConfigurator(File file) {

        super(file);

//        // 订阅配置信息
//        XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 返回从网络加载的配置路径。
     * 
     * @return 从网络加载的配置路径
     */
    private static URL getLoadingURL() {

        // 从环境变量中获取配置信息
        String urlPath = System.getenv("JadeConfigUrl");

        if (urlPath == null) {
            urlPath = "http://jadeconfig.d.xiaonei.com/jade-config.xml";
        }

        try {
            return new URL(urlPath);

        } catch (MalformedURLException e) {

            if (logger.isErrorEnabled()) {
                logger.error("Malformed URL [" + urlPath + ']', e);
            }
        }

        return null;
    }

    /**
     * 返回从本地加载的文件路径。
     * 
     * @return 从本地加载的文件路径
     */
    private static File getLoadingFile() {

        String filePath;

        if (File.separatorChar == '\\') {

            filePath = FAILLOADING_PATH_WINDOWS + CONFIG_FILE;

        } else {

            filePath = FAILLOADING_PATH_LINUX + CONFIG_FILE;
        }

        return new File(filePath);
    }

    /**
     * 返回优先加载选项: <code>true</code> 优先从网络加载, <code>false</code> 优先从本地加载。
     * 
     * @return 优先加载选项
     */
    private static boolean getPriority() {

        // 获得目录标记文件
        String filePath;

        if (File.separatorChar == '\\') {

            filePath = FAILLOADING_PATH_WINDOWS + MARKUP_FILE;

        } else {

            filePath = FAILLOADING_PATH_LINUX + MARKUP_FILE;
        }

        File markup = new File(filePath);

        // 检查目录标记：是否从服务器获取配置信息
        return !markup.exists();
    }

    /**
     * 实现命令行调用工具。用法：
     * 
     * <PRE>
     * XceConfigurator reload      - 重新加载配置信息
     *                 merge file  - 合并配置项到内容
     *                 load file   - 加载配置项
     * </PRE>
     * 
     * @param args - 命令参数, 参考上文说明。
     */
    public static void main(String... args) {

        if (args.length < 1) {

            System.out.println("Usage: java -cp * XmlDocXceConfigurator [reload]");
            System.out.println();
            System.out.println("XmlDocXceConfigurator reload      - Reload XML configurations");

            return;
        }

        String command = args[0];

        if ("RELOAD".equalsIgnoreCase(command)) {

            System.out.println("Reloading configuration ...");

            // 重新从网址加载配置信息
            HashMap<String, String> props = new HashMap<String, String>();

            props.put(COMMAND, command);

            // 发布配置更新
            XceStormAdapter.getInstance().notify(SUBSCRIBE_NAME, props);

            System.out.println("Reload OK");

            System.exit(0);

        } else if ("TEST".equalsIgnoreCase(command)) {

            // 配置  Log4j
            BasicConfigurator.configure();

            // 必须指定文件路径
            if (args.length < 2) {

                System.out.println("ERROR: must specify file or url.");

            } else {

                String filePath = args[1];

                // 检查参数是路径还是网址
                if (filePath.indexOf(':') >= 2) {

                    try {
                        URL url = new URL(filePath);

                        new RemoteXmlDocConfigurator(url);

                    } catch (MalformedURLException e) {

                        System.out.println("ERROR: malformed url [" + filePath + ']');
                    }

                } else {

                    File file = new File(filePath);

                    new RemoteXmlDocConfigurator(file);
                }
            }

        } else {

            System.out.println("Usage: java -cp . XceConfigurator [reload | merge | load] (file)");
            System.out.println();
            System.out.println("XmlDocXceConfigurator reload      - 重新加载配置信息");

            return;
        }
    }
}
