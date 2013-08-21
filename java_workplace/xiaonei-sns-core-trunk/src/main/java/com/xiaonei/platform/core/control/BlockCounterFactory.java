package com.xiaonei.platform.core.control;

import java.io.InputStream;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import org.apache.commons.lang.math.NumberUtils;

/**
 * a factory for <code>{@link BlockCounter}</code> <br>
 * The XCE service is blocked when the concurrence of access is large, and then
 * some service is timeout. This service provides an approach to avoid this
 * things happening.<br>
 * Like DataSource, it cuts all the access while the concurrence of access is
 * more then the given number. Althouse the application will not get the data,
 * but the service is still work fine.
 * <p>
 * Thers is an example:
 * 
 * <pre>
 * if (BlockCounterFactory.enter(&quot;key&quot;)) {
 * 	try {
 * 		doOurTask();
 * 	} finally {
 * 		BlockCounterFactory.outer(&quot;key&quot;);
 * 	}
 * } else {
 * 	throw new Throwable(&quot;&quot;);
 * }
 * 
 * </pre>
 * 
 * </p>
 * 
 * @author xylz(xylz@live.cn)
 * @since 20091022
 * @see {@link BlockCounter}
 * 
 */
public class BlockCounterFactory {

	private boolean used = false;
	private int defaultMaxBlocked = BlockCounter.UNLIMITED;
	private Map<String, BlockCounter> counters = new HashMap<String, BlockCounter>();

	private static class BlockCounterFactoryHolder {
		private static BlockCounterFactory instance = new BlockCounterFactory();
	}

	private BlockCounterFactory() {
		reload();
	}

	public static BlockCounterFactory getInstance() {
		return BlockCounterFactoryHolder.instance;
	}

	/**
	 * Load configuration from file and drop all
	 * <code>{@linkplain BlockCounter}</code> if there are.<br>
	 * The loading order of configuration file (in classpath):
	 * <ol>
	 * <li>/blockcounter.properties</li>
	 * <li>/com/xiaonei/platform/core/control/blockcounter.properties</li>
	 * </ol>
	 */
	public synchronized void reload() {
		long begin = TimeCostUtil.logBegin();
		try {
			Properties p = new Properties();
			InputStream in = null;
			try {
				for (URL url : new URL[] {//
						BlockCounterFactory.class
								.getResource("/blockcounter.properties")//
						,
						URLClassLoader
								.getSystemResource("/blockcounter.properties")//
						,
						BlockCounterFactory.class
								.getResource("blockcounter.properties")//
						,
						URLClassLoader
								.getSystemResource("blockcounter.properties") //
				}) {
					if (url != null) {
						in = url.openStream();
						break;
					}
				}
				if (in != null)
					p.load(in);
			} catch (Exception e) {
				TimeCostUtil.logError(e);
			} finally {
				if (in != null)
					in.close();
			}
			used = "true".equalsIgnoreCase(p.getProperty("used.counter",
					"false"));
			defaultMaxBlocked = NumberUtils.toInt(p
					.getProperty("maxblock.counter"), BlockCounter.UNLIMITED);
			// clear the old counters for reload again
			counters.clear();
			for (Object o : p.keySet()) {
				String k = (String) o;
				if (k.startsWith("counter.") && k.length() > 8) {
					int maxBlocked = NumberUtils.toInt(p.getProperty(k),
							defaultMaxBlocked);
					BlockCounter bc = new BlockCounter(k.substring(8),
							maxBlocked);
					counters.put(bc.getName(), bc);
				}
			}

		} catch (Exception e) {
			TimeCostUtil.logError(TimeCostUtil.logger, e,
					"loading resource error");
		} finally {
			TimeCostUtil.logEnd(begin, "BlockCounterFactory", "load_resource");
		}
	}

	/**
	 * get the statu of working
	 */
	public boolean isUsed() {
		return used;
	}

	/**
	 * set the BlockCountFactory workable<br>
	 * This method will trigger the action of the configuration reloading. <br>
	 * Using {@linkplain #reload()} for configuration reloading only.
	 * 
	 * @param used
	 *            the workable status
	 * @see {@linkplain #reload()}
	 */
	public synchronized void setUsed(boolean used) {
		reload();
		this.used = used;
	}

	/**
	 * check XCE access
	 * 
	 * @param name
	 *            the key for access
	 * @return false while the 'Block' is used but reaching the limited
	 * @see {@link BlockCounter#enter()}
	 */
	public boolean enter(String name) {
		if (name == null)
			throw new IllegalArgumentException("empty 'name' for BlockCounter");
		if (!used)
			return true;
		BlockCounter bc = counters.get(name);
		if (bc == null) {
			synchronized (counters) {
				bc = counters.get(name);
				if (bc == null) {
					bc = new BlockCounter(name, defaultMaxBlocked);
					counters.put(name, bc);
				}
			}
		}
		return bc.enter();
	}

	/**
	 * release the XCE access
	 * 
	 * @param name
	 *            the key for release
	 * @see {@link BlockCounter#outer()}
	 */
	public void outer(String name) {
		if (name == null)
			throw new IllegalArgumentException("empty 'name' for BlockCounter");
		if (!used)
			return;
		BlockCounter bc = counters.get(name);
		if (bc == null)
			new IllegalArgumentException(name).printStackTrace();
		else
			bc.outer();
	}

	/**
	 * read the {@link BlockCounter}
	 * 
	 * @param name
	 *            the name of the {@link BlockCounter}
	 * @return the {@link BlockCounter} with the name of null while nothing
	 *         found
	 */
	public BlockCounter getBlockCounter(String name) {
		if (name == null)
			throw new IllegalArgumentException("empty 'name' for BlockCounter");
		return counters.get(name);
	}

	/**
	 * get all {@link BlockCounter}
	 * 
	 * @return an unmodifiable map
	 */
	public Map<String, BlockCounter> getCounters() {
		return Collections.unmodifiableMap(counters);
	}

	public static void main(String[] args) {
		BlockCounterFactory bcf = new BlockCounterFactory();
		System.out.println(bcf.isUsed());
		System.out.println(bcf.enter("test"));
		System.out.println(bcf.getBlockCounter("test"));
		System.out.println(bcf.enter("test"));
		System.out.println(bcf.getBlockCounter("test"));
		bcf.outer("test");
		System.out.println(bcf.getBlockCounter("test"));
		System.out.println(bcf.getCounters());
	}
}
