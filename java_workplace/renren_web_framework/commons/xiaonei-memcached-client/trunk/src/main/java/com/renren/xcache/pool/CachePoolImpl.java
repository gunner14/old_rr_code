package com.renren.xcache.pool;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;



/**
 * {@link CachePool}的实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-10 下午05:42:55
 */
public class CachePoolImpl implements CachePool {

	private String name;
	
	private List<Node> nodes = new ArrayList<Node>();

	public void setName(String name) {
		this.name = name;
	}
	
	@Override
	public String getName() {
		return name;
	}

	@Override
	public List<Node> getNodes() {
		return nodes;
	}

	/**
	 * 向池中添加一个节点
	 * @param node
	 */
	public void addNode(Node node) {
		if (node == null) {
			throw new NullPointerException();
		}
		for (Node n : nodes) {
			//hostname和port都一样就不能添加进来
			//因为不考虑权重，所以这里不能用equals来判断
			if (n.getHostname().equals(node.getHostname())
					&& n.getPort() == node.getPort()) {
				throw new IllegalArgumentException("Duplicated Node: "
						+ node.toString() + ", currently in List is: "
						+ n.toString());
			}
		}
		nodes.add(node);
	}
	
	public boolean equals(Object anObject) {
		
		if (anObject != null && anObject instanceof CachePool) {
			CachePool anPool = (CachePool)anObject;
			if (this.name.equals(anPool.getName())) {
				return compareNodes(this.nodes, anPool.getNodes());
			}
		}
		return false;
	}
	
	/**
	 * 比较两组节点是否相同。只要数据相同就可以，与顺序无关。
	 * 
	 * @param nodes1
	 * @param nodes2
	 * @return
	 */
	private boolean compareNodes(List<Node> nodes1, List<Node> nodes2) {
		if (nodes1.size() == nodes2.size()) {
			
			Node[] type = new Node[]{};
			Node[] nodea1 = nodes1.toArray(type);
			Node[] nodea2 = nodes2.toArray(type);
			
			Arrays.sort(nodea1, nodeComparator);
			Arrays.sort(nodea2, nodeComparator);
			return Arrays.equals(nodea1, nodea2);
		} else {
			return false;
		}
	}
	
	/**
	 * 给@{link Node}排序用
	 */
	private static Comparator<Node> nodeComparator = new Comparator<Node>() {
		@Override
		public int compare(Node n1, Node n2) {
			
			//先比hostname
			int cmp = n1.getHostname().compareTo(n2.getHostname());
			if (cmp != 0) {
				return cmp;
			}
			
			//再比port
			if (n1.getPort() < n2.getPort()) {
				return -1;
			} else if (n1.getPort() > n2.getPort()) {
				return 1;
			} else {
				
				//最后比weight
				if (n1.getWeight() < n2.getWeight()) {
					return -1;
				} else if (n1.getWeight() > n2.getWeight()) {
					return 1;
				} else {
					return 0;
				}
			}
		}
	};
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(name);
		sb.append('[');
		for (Node node : nodes) {
			sb.append(node.toString());
			sb.append(",");
		}
		if (sb.charAt(sb.length() - 1) == ',') {
			sb.setLength(sb.length() - 1);
		}
		sb.append(']');
		return sb.toString();
	}
	
	public int hashCode() {
		throw new UnsupportedOperationException("hashCode not designed");
	}
	
}
