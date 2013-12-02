package com.renren.xcs;

import java.util.Arrays;

/**
 * 节点信息
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class Node {
	/**
	 * 节点的名字
	 */
	private String name;

	/**
	 * 节点的内容
	 */
	private byte[] data;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

	public Node(String name, byte[] data) {
		super();
		this.name = name;
		this.data = data;
	}

	@Override
	public String toString() {
		return "Node [name=" + name + ", data=" + Arrays.toString(data) + "]";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(data);
		result = prime * result + ((name == null) ? 0 : name.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Node other = (Node) obj;
		if (!Arrays.equals(data, other.data))
			return false;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

}
