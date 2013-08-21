package com.xiaonei.platform.scheduleTask;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;

import com.xiaonei.platform.storage.Node;

public class Group implements Comparable {

	private String name;

	private Random rand = new Random();

	private ArrayList<Node> nodes = new ArrayList<Node>();

	public int compareTo(Object o) {
		Group group = (Group) o;
		return this.getLoad() - group.getLoad();
	}

	public void putNode(Node node) {
		nodes.add(node);
	}

	public Node getReadableNode(String fileName) {
		Node node = nodes.get(Math.abs(fileName.hashCode() % nodes.size()));
		if (node.isReadable()) {
			return node;
		}
		int start = rand.nextInt(nodes.size());
		for (int i = start;;) {
			node = nodes.get(i);
			if (node.isReadable()) {
				return node;
			}
			if (++i >= nodes.size()) {
				i = 0;
			}
			// circle back
			if (i == start) {
				break;
			}
		}
		// for (int i = 0; i < nodes.size(); ++i) {
		// node = nodes.get(rand.nextInt(nodes.size()));
		// if (node.isReadable()) {
		// return node;
		// }
		// }

		return nodes.get(rand.nextInt(nodes.size()));
	}

	public ArrayList<Node> getNodes() {
		return nodes;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public boolean isWritable() {
		for (int i = 0; i < nodes.size(); ++i) {
			Node node = nodes.get(i);
			if (!node.isWritable()) {
				return false;
			}
		}
		return true;
	}

	public int getLoad() {
		double load = 0.0;
		for (int i = 0; i < nodes.size(); ++i) {
			Node node = nodes.get(i);
			if (node.getCpuLoad() > load) {
				load = node.getCpuLoad();
			}
		}
		return (int) load;
	}

	public int rank() {
		double load = getLoad();
		if (load <= 5) {
			return Constants.RANK_LEVEL_IDLE;
		}
		if (load <= 8) {
			return Constants.RANK_LEVEL_NORMAL;
		}
		if (load <= 12) {
			return Constants.RANK_LEVEL_WARN;
		}

		return Constants.RANK_LEVEL_FATAL;
	}

	public int getType() {
		if (nodes.isEmpty()) {
			return -1;
		}
		return nodes.get(0).getType();
	}

	public static Map getGroupMap(List<Node> allnodes) {
		// 按照键值排序
		// 注意：每次对TreeMap进行put()时，TreeMap都会自动调用它的compare(key,Entry.key)【TreeMap
		// api:463行】
		// 按照key进行排序
		Map sortCpuNodeMap = Collections.synchronizedMap(new TreeMap());
		if (allnodes.size() != 0) {
			for (Node node : allnodes) {
				sortCpuNodeMap.put(node.getCpuLoad(), node);
			}
			return sortCpuNodeMap;
		} else
			return null;

		/*
		 * Collection col = sortCpuNodeMap.keySet(); Iterator it =
		 * col.iterator(); while (it.hasNext()) { System.out.println(it.next()); }
		 */
	}

	public static void main(String args[]) {
		Node one = new Node();
		one.setCpuLoad(11.0);
		one.setDomain("sohu");

		Node two = new Node();
		two.setCpuLoad(8.0);
		two.setDomain("sina");

		Node three = new Node();
		three.setCpuLoad(5.2);
		three.setDomain("163");

		Node fore = new Node();
		fore.setCpuLoad(12.2);
		fore.setDomain("qq");

		List<Node> aa = new ArrayList<Node>();
		aa.add(one);
		aa.add(two);
		aa.add(three);
		aa.add(fore);

		Map onemap = getGroupMap(aa);
		Iterator it=onemap.entrySet().iterator();
		   //使用entrySet方法将hashMap转化为Set视图,返回的Set中的每个元素都是一个Map.Entry
		while(it.hasNext()){
		    Map.Entry entry=(Map.Entry)it.next();
		    Double key=(Double)entry.getKey();
		    Node value=(Node)entry.getValue();
		    System.out.println("key:"+key+"   value:"+value.getDomain());
		}
	}
}
