package com.xiaonei.commons.jade.impl.xceadapter.configurator;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.locks.Lock;

import org.apache.log4j.BasicConfigurator;

import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;

/**
 * 实现加载远程服务器的配置文件，并且可以监听文件改变。
 * 
 * @author han.liao
 */
public class RemoteXceConfigurator extends PropertiesXceConfigurator {

    // 订阅的唯一名称
    public static final String SUBSCRIBE_NAME = "JadeConfig";

    // 命令参数名称
    protected static final String COMMAND = "#COMMAND";

    // 订阅配置信息
    protected XceStormListener stormListener = new XceStormListener() {

        // serialVersionUID
        private static final long serialVersionUID = -7019155420108873791L;

        @Override
        public void handle(Map<String, String> context) {

            String command = context.remove(COMMAND);

            if ("RELOAD".equalsIgnoreCase(command)) {

                if (url != null) {

                    // 输出日志
                    if (logger.isInfoEnabled()) {
                        logger.info("Reloading url [" + url + ']');
                    }

                    // 重新从网址加载配置信息
                    loadConfiguration(url, null);

                    // 输出日志
                    if (logger.isInfoEnabled()) {
                        logger.info("Reload OK");
                    }

                } else if (file != null) {

                    // 输出日志
                    if (logger.isInfoEnabled()) {
                        logger.info("Reloading file [" + file.getPath() + ']');
                    }

                    // 重新从网址加载配置信息
                    loadConfiguration(file);

                    // 输出日志
                    if (logger.isInfoEnabled()) {
                        logger.info("Reload OK");
                    }

                } else {

                    // 输出日志
                    if (logger.isInfoEnabled()) {
                        logger.info("ERROR: No configurated url or file.");
                    }
                }

            } else if ("MERGE".equalsIgnoreCase(command)) {

                // 输出日志
                if (logger.isInfoEnabled()) {
                    logger.info("Merging entries: " + context.size());
                }

                // 加锁保护配置信息的完整性
                Lock lock = rwLock.writeLock();

                try {
                    lock.lock();

                    // 合并配置信息。
                    properties.putAll(context);

                    cache.clear();

                } finally {

                    lock.unlock();
                }

                // 输出日志
                if (logger.isInfoEnabled()) {
                    logger.info("Merge OK");
                }

            } else if ("LOAD".equalsIgnoreCase(command)) {

                // 输出日志
                if (logger.isInfoEnabled()) {
                    logger.info("Loading entries: " + context.size());
                }

                // 加锁保护配置信息的完整性
                Lock lock = rwLock.writeLock();

                try {
                    lock.lock();

                    // 使用发过来的配置信息。
                    properties.clear();
                    properties.putAll(context);

                    cache.clear();

                } finally {

                    lock.unlock();
                }

                // 输出日志
                if (logger.isInfoEnabled()) {
                    logger.info("Load OK");
                }
            }
        }
    };

    /**
     * 创建: RemoteXceConfigurator 配置。
     */
    public RemoteXceConfigurator() {

        super(getDefault());

        // 订阅配置信息
        XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 创建: RemoteXceConfigurator 配置。
     * 
     * @param url - 配置网络路径
     */
    public RemoteXceConfigurator(URL url) {

        super(url);

        // 订阅配置信息
        XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 配置: RemoteXceConfigurator 对象。
     * 
     * @param file - 配置本地路径
     */
    public RemoteXceConfigurator(File file) {

        super(file);

        // 订阅配置信息
        XceStormAdapter.getInstance().subscribe(SUBSCRIBE_NAME, stormListener);
    }

    /**
     * 从指定的文件路径或网址读取配置。
     * 
     * @param location - 指定的文件路径或网址
     */
    private static Properties loadProperties(String filePath) throws IOException {

        // 检查参数是路径还是网址
        if (filePath.indexOf(':') >= 2) {

            URL url = new URL(filePath);

            return loadProperties(url);

        } else {

            File file = new File(filePath);

            return loadProperties(file);
        }
    }

    /**
     * 返回默认加载的配置路径。
     * 
     * @return 默认的配置路径
     */
    private static URL getDefault() {

        // 从环境变量中获取配置信息
        String urlPath = System.getenv("JadeConfigUrl");

        if (urlPath == null) {
            urlPath = "http://jadeconfig.d.xiaonei.com/jade-config.properties";
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

            System.out.println("Usage: java -cp * XceConfigurator [reload | merge | load] (file)");
            System.out.println();
            System.out.println( // NL
                    "XceConfigurator reload      - Reload .properties configurations");
            System.out.println( // NL
                    "                merge file  - Merge {file} to current configurations");
            System.out.println( // NL
                    "                load file   - Replace current configurations with {file}");

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

        } else if ("MERGE".equalsIgnoreCase(command)) {

            // 必须指定文件路径
            if (args.length < 2) {

                System.out.println("ERROR: must specify file or url.");

            } else {

                String filePath = args[1];

                try {
                    System.out.println("Merging configuration ...");

                    // 重新从网址加载配置信息
                    HashMap<String, String> props = new HashMap<String, String>();

                    props.put(COMMAND, command);

                    putAllProps(props, loadProperties(filePath));

                    // 发布配置更新
                    XceStormAdapter.getInstance().notify(SUBSCRIBE_NAME, props);

                    System.out.println("Merge OK");

                } catch (MalformedURLException e) {

                    System.out.println("ERROR: malformed url - " + filePath);

                } catch (FileNotFoundException e) {

                    System.out.println("ERROR: file not found - " + filePath);

                } catch (IOException e) {

                    System.out.println("ERROR: can't load properties - " + e.getLocalizedMessage());
                }
            }

            System.exit(0);

        } else if ("LOAD".equalsIgnoreCase(command)) {

            // 必须指定文件路径
            if (args.length < 2) {

                System.out.println("ERROR: must specify file or url.");

            } else {

                String filePath = args[1];

                try {
                    System.out.println("Loading configuration ...");

                    // 重新从网址加载配置信息
                    HashMap<String, String> props = new HashMap<String, String>();

                    props.put(COMMAND, command);

                    putAllProps(props, loadProperties(filePath));

                    // 发布配置更新
                    XceStormAdapter.getInstance().notify(SUBSCRIBE_NAME, props);

                    System.out.println("Load OK");

                } catch (MalformedURLException e) {

                    System.out.println("ERROR: malformed url - " + filePath);

                } catch (FileNotFoundException e) {

                    System.out.println("ERROR: file not found - " + filePath);

                } catch (IOException e) {

                    System.out.println("ERROR: can't load properties - " + e.getLocalizedMessage());
                }
            }

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

                        new RemoteXceConfigurator(url);

                    } catch (MalformedURLException e) {

                        System.out.println("ERROR: malformed url [" + filePath + ']');
                    }

                } else {

                    File file = new File(filePath);

                    new RemoteXceConfigurator(file);
                }
            }

        } else {

            System.out.println("Usage: java -cp . XceConfigurator [reload | merge | load] (file)");
            System.out.println();
            System.out.println("XceConfigurator reload      - 重新加载配置信息");
            System.out.println("                merge file  - 合并配置文件到当前配置");
            System.out.println("                load file   - 加载配置文件取代当前配置");

            return;
        }
    }
}
