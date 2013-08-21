package com.xiaonei.platform.storage;

import java.util.ArrayList;
import java.util.Random;

public class Group implements Comparable<Group> {
	
	private String name;

	private Random rand = new Random();

	private ArrayList<Node> nodes = new ArrayList<Node>();

	public int compareTo(Group group) {
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
		for (int i = start; ; ){
			node = nodes.get(i);
			if (node.isReadable()) {
				return node;
			}
			if (++i >= nodes.size()) {
				i = 0;
			}
			if (i == start) {
				break;
			}
		}
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
			if(node.getStatus()!=3){
				return false;
			}
/*			if (!node.isWritable()) {
				return false;
			}*/
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
		return (int)load;
	}
	
	public int rank() {
		double load = getLoad();
		if (load <= 10) {
			return Constants.RANK_LEVEL_IDLE;
		}
		if (load <= 30) {
			return Constants.RANK_LEVEL_NORMAL;
		}
		if (load <= 50) {
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
	public String toString(){
		return "[name:"+name+",rand:"+rand+",nodes:"+nodes+"]" ;
	}
}
