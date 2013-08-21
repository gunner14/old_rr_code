package com.xiaonei.commons.jade.impl.xceadapter.configurator;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceDescriptor;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceDescriptorLoader;

/**
 * 从 Properties 文件中读取散表属性的配置器实现。
 * 
 * @author han.liao
 */
public class PropertiesXceConfigurator implements XceConfigurator {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(PropertiesXceConfigurator.class);

    // 配置文件名称
    private static final String JADE_CONFIG_FILE = "jade-config.properties";

    // 缓存的配置项
    protected ConcurrentHashMap<String, XceDescriptor> cache = new ConcurrentHashMap<String, XceDescriptor>();

    // 配置信息
    protected ConcurrentHashMap<String, String> properties = new ConcurrentHashMap<String, String>();

    // 加锁保护配置信息
    protected ReadWriteLock rwLock = new ReentrantReadWriteLock();

    // 配置文件路径
    protected File file;

    // 配置文件路径
    protected URL url;

    // 是否成功加载
    protected boolean inited;

    /**
     * 配置: PropertiesXceConfigurator 对象。
     */
    public PropertiesXceConfigurator() {

        this(getDefault());
    }

    /**
     * 配置: PropertiesXceConfigurator 对象。
     * 
     * @param file - 配置文件
     */
    public PropertiesXceConfigurator(File file) {

        if (file == null) {
            throw new IllegalArgumentException("file is null");
        }

        if (logger.isInfoEnabled()) {
            logger.info("Loading config from: " + file);
        }

        loadConfiguration(file);

        this.file = file;

        inited = true;
    }

    /**
     * 配置: PropertiesXceConfigurator 对象。
     * 
     * @param url - 配置网络路径
     */
    public PropertiesXceConfigurator(URL url) {

        if (url == null) {
            throw new IllegalArgumentException("url is null");
        }

        if (logger.isInfoEnabled()) {
            logger.info("Loading config from: " + url);
        }

        loadConfiguration(url, null);

        this.url = url;

        inited = true;
    }

    /**
     * 配置: PropertiesXceConfigurator 对象。
     * 
     * @param url - 配置网络路径
     * 
     * @param failLoading - 如果从网络路径读取失败, 加载的默认路径, 可以是: <code>null</code>.
     */
    public PropertiesXceConfigurator(URL url, File failLoading) {

        if (url == null) {

            if (failLoading == null) {
                throw new IllegalArgumentException("url is null, and failLoading is null");
            }

            if (logger.isInfoEnabled()) {
                logger.info("Loading config from: " + failLoading);
            }

            loadConfiguration(failLoading);

            this.file = failLoading;

            inited = true;

        } else {

            if (logger.isInfoEnabled()) {
                logger.info("Loading config from: " + url);
            }

            boolean fromUrl = loadConfiguration(url, failLoading);

            if (!fromUrl) {
                this.file = failLoading;
            } else {
                this.url = url;
            }

            inited = true;
        }
    }

    /**
     * 返回配置文件。
     * 
     * @return 配置文件
     */
    public File getFile() {
        return file;
    }

    /**
     * 设置配置文件。
     * 
     * @param file - 配置文件
     */
    public void setFile(File file) {

        // 加载配置文件。
        loadConfiguration(file);

        this.file = file;

        url = null;

        inited = true;
    }

    /**
     * 设置配置文件的路径。
     * 
     * @param filePath - 配置文件的路径
     */
    public void setFile(String filePath) {

        File file = new File(filePath);

        // 加载配置文件。
        loadConfiguration(file);

        this.file = file;

        url = null;

        inited = true;
    }

    /**
     * 返回配置网络路径。
     * 
     * @return - 配置网络路径
     */
    public URL getURL() {
        return url;
    }

    /**
     * 设置配置网络路径。
     * 
     * @param url - 配置网络路径
     */
    public void setURL(URL url) {

        // 加载配置文件。
        loadConfiguration(url, null);

        this.url = url;

        file = null;

        inited = true;
    }

    /**
     * 设置配置网络路径。
     * 
     * @param url - 配置网络路径
     */
    public void setURL(String urlPath) {

        try {
            URL url = new URL(urlPath);

            // 加载配置文件。
            loadConfiguration(url, null);

            this.url = url;

            file = null;

            inited = true;

        } catch (MalformedURLException e) {

            // 输出日志
            if (logger.isWarnEnabled()) {
                logger.warn("Malformed url [" + urlPath + ']', e);
            }

            throw new IllegalArgumentException("Malformed url [" + urlPath + ']', e);
        }
    }

    @Override
    public XceDescriptor getCatalogDescriptor(String catalog) {

        if (!inited) {
            throw new IllegalStateException("XceConfigurator is not initialized.");
        }

        XceDescriptor descriptor = cache.get(catalog);

        if (descriptor == null) {

            // 加锁保护配置信息的完整性
            Lock lock = rwLock.readLock();

            try {
                lock.lock();

                // catalog = dbname
                String dbname = properties.get(catalog);

                if (dbname != null) {

                    descriptor = XceDescriptorLoader.create(dbname);

                    cache.putIfAbsent(catalog, descriptor);
                }

            } finally {

                lock.unlock();
            }
        }

        return descriptor;
    }

    @Override
    public XceDescriptor getDescriptor(String catalog, String name) {

        if (!inited) {
            throw new IllegalStateException("XceConfigurator is not initialized.");
        }

        String keyword = catalog + '.' + name;

        XceDescriptor descriptor = cache.get(keyword);

        if (descriptor == null) {

            // 加锁保护配置信息的完整性
            Lock lock = rwLock.readLock();

            try {
                lock.lock();

                // catalog = dbname
                String dbname = properties.get(catalog);

                descriptor = XceDescriptorLoader.fromProps(keyword, properties, dbname);

                if (descriptor != null) {
                    cache.putIfAbsent(keyword, descriptor);
                }

            } finally {

                lock.unlock();
            }
        }

        return descriptor;
    }

    /**
     * 从指定的网络路径读取配置。
     * 
     * @param url - 指定的网络路径
     * 
     * @param failLoading - 如果从网络路径读取失败, 加载的默认路径, 可以是: <code>null</code>.
     * 
     * @return 从网络路径加载成功返回 <code>true</code>, 否则返回 <code>false</code>.
     */
    protected boolean loadConfiguration(URL url, File failLoading) {

        Lock lock = null;

        try {
            // 加载配置信息
            Properties props = loadProperties(url);

            // 加锁保护配置信息的完整性
            lock = rwLock.writeLock();

            lock.lock();

            // 替换原有配置信息
            properties.clear();

            putAllProps(properties, props);

            cache.clear();

            return true;

        } catch (IOException e) {

            // 输出日志
            if (logger.isWarnEnabled()) {
                logger.warn("Can't load [" + url + ']', e);
            }

            if (failLoading == null) {
                throw new IllegalArgumentException("Can't load properties: " + url, e);
            }

            // 输出日志
            if (logger.isInfoEnabled()) {
                logger.info("Loading config from: " + failLoading.getPath());
            }

            // 从网络路径读取失败, 加载默认路径
            loadConfiguration(failLoading);

        } finally {

            if (lock != null) {
                lock.unlock();
            }
        }

        return false;
    }

    /**
     * 从指定的文件读取配置。
     * 
     * @param file - 指定的文件
     */
    protected void loadConfiguration(File file) {

        Lock lock = null;

        try {
            // 加载配置信息
            Properties props = loadProperties(file);

            // 加锁保护配置信息的完整性
            lock = rwLock.writeLock();

            lock.lock();

            // 替换原有配置信息
            properties.clear();

            putAllProps(properties, props);

            cache.clear();

        } catch (IOException e) {

            // 输出日志
            if (logger.isWarnEnabled()) {
                logger.warn("Can't load [" + url + ']', e);
            }

            throw new IllegalArgumentException("Can't load properties: " + url, e);

        } finally {

            if (lock != null) {
                lock.unlock();
            }
        }
    }

    /**
     * 将: Properties 集合并入: Map<String, String> 集合。
     * 
     * @param props - Properties 集合
     * 
     * @return Map<String, String> 集合
     */
    protected static void putAllProps(Map<String, String> map, Properties props) {

        for (Entry<Object, Object> entry : props.entrySet()) {

            // 如果有类型不符合的对象，请抛出异常。
            map.put((String) entry.getKey(), (String) entry.getValue());
        }
    }

    /**
     * 从指定的网络路径读取配置。
     * 
     * @param url - 指定的网络路径
     */
    protected static Properties loadProperties(URL url) throws IOException {

        InputStreamReader in = null;

        try {
            // 打开网络连接
            URLConnection connect = url.openConnection();

            // 打开文件流
            in = new InputStreamReader(connect.getInputStream(), "UTF-8");

            // 加载配置信息
            Properties props = new Properties();

            props.load(in);

            return props;

        } finally {

            // 关闭文件流
            if (in != null) {

                try {
                    in.close();

                } catch (IOException e) {

                    // 输出日志
                    if (logger.isWarnEnabled()) {
                        logger.warn("Can't close stream", e);
                    }
                }
            }
        }
    }

    /**
     * 从指定的文件读取配置。
     * 
     * @param file - 指定的文件
     */
    protected static Properties loadProperties(File file) throws FileNotFoundException, IOException {

        FileReader fin = null;

        try {
            // 打开文件流
            fin = new FileReader(file);

            // 加载配置信息
            Properties props = new Properties();

            props.load(fin);

            return props;

        } finally {

            // 关闭文件流
            if (fin != null) {

                try {
                    fin.close();

                } catch (IOException e) {

                    // 输出日志
                    if (logger.isWarnEnabled()) {
                        logger.warn("Can't close stream", e);
                    }
                }
            }
        }
    }

    /**
     * 返回默认加载的配置路径。
     * 
     * @return 默认的配置路径
     */
    private static URL getDefault() {

        ClassLoader classLoader = PropertiesXceConfigurator.class.getClassLoader();

        URL url = classLoader.getResource(JADE_CONFIG_FILE);

        if (url == null) {

            if (logger.isWarnEnabled()) {
                logger.warn( // NL
                        "Can't load [" + JADE_CONFIG_FILE + "] from [" + classLoader + ']');
            }
        }

        return url;
    }
}
