package com.renren.xcache.namespace;

/**
 * {@link Namespace}的实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:38:54
 */
public class NamespaceImpl implements Namespace {

	private String name;

	private String associatedPoolName;

	public void setName(String name) {
		this.name = name;
	}

	public void setAssociatedPoolName(String associatedPoolName) {
		this.associatedPoolName = associatedPoolName;
	}

	@Override
	public String getAssociatedPoolName() {
		return associatedPoolName;
	}

	@Override
	public String getName() {
		return name;
	}

	public boolean equals(Object anObj) {

		if (anObj != null && anObj instanceof Namespace) {
			Namespace anNamespace = (Namespace) anObj;
			return name.equals(anNamespace.getName())
					&& associatedPoolName.equals(anNamespace
							.getAssociatedPoolName());
		}

		return false;
	}
	
	public String toString() {
		return "name=" + name + ", pool=" + associatedPoolName;
	}

	public int hashCode() {
		throw new UnsupportedOperationException("hashCode not designed");
	}
	
}
