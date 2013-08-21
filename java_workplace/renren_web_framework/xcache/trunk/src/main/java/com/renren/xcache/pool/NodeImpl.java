package com.renren.xcache.pool;

/**
 * {@link Node}的实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:18:06
 */
public class NodeImpl implements Node {

	private String hostname;
	
	private int port;
	
	/**
	 * 权重默认为1
	 */
	private int weight = 1;
	
	public NodeImpl setHostname(String hostname) {
		this.hostname = hostname;
		return this;
	}

	public NodeImpl setPort(int port) {
		this.port = port;
		return this;
	}

	public NodeImpl setWeight(int weight) {
		this.weight = weight;
		return this;
	}

	@Override
	public String getHostname() {
		return hostname;
	}

	@Override
	public int getPort() {
		return port;
	}

	@Override
	public int getWeight() {
		return weight;
	}

	public boolean equals(Object obj) {
		
		if (obj != null && obj instanceof Node) {
			Node anNode = (Node) obj;
			return this.hostname.equals(anNode.getHostname())
					&& this.port == anNode.getPort()
					&& this.weight == anNode.getWeight();
		}
		return false;
	}
	
	public String toString() {
		return hostname + ":" + port + "*" + weight;
	}
	
	public int hashCode() {
		throw new UnsupportedOperationException("hashCode not designed");
	}
	
	public static void main(String[] args) {
		System.out.println(new NodeImpl().hashCode());;
	}
}
