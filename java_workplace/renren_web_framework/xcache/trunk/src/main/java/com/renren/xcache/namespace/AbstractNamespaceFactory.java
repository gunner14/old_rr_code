package com.renren.xcache.namespace;

import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * {@link NamespaceFactory}的基类实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:40:15
 */
public abstract class AbstractNamespaceFactory implements NamespaceFactory {

	protected Log logger = LogFactory.getLog(this.getClass());
	
	protected Map<String, Namespace> namespaces = new ConcurrentHashMap<String, Namespace>();
	
	@Override
	public Namespace getNamespace(String name) {
		Namespace namespace = namespaces.get(name);
		if (namespace == null) {
			synchronized (name.intern()) {
				namespace = namespaces.get(name);
				if (namespace == null) {
					namespace = loadConfig(name);
				}
				if (namespace != null) {
					namespaces.put(name, namespace);
				}
			}
		}
		return namespace;
	}

	public abstract Namespace loadConfig(String name);
	
	protected void onNamespaceModified(Namespace newNamespace) {
		String name = newNamespace.getName();
		Namespace oldNamespace = namespaces.get(name);
		if (oldNamespace != null) {
			if (newNamespace.equals(oldNamespace)) {
				if (logger.isInfoEnabled()) {
					logger.info("Namespace config not modified:" + oldNamespace);
				}
			} else {
				namespaces.put(name, newNamespace);
				if (logger.isInfoEnabled()) {
					logger.info("Namespace config modified:\r\nold="
							+ oldNamespace + "\r\nnew=" + newNamespace);
				}
			}
		} else {
			logger.error("Original Namespace not existed, this should never happen: " + name);
		}
	}
	
	/**
	 * @return 当前本地配置缓存中所有的namespace的name
	 */
	protected Set<String> getNamespaceNames() {
		return namespaces.keySet();
	}
	
}
