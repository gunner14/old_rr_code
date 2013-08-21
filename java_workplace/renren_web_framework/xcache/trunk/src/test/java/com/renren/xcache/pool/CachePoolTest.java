package com.renren.xcache.pool;


import static org.junit.Assert.*;
import org.junit.Test;

public class CachePoolTest {

	@Test
	public void testAddNode1() {
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName("haha");
		boolean expOccurs = false;
		try {
			pool.addNode(genNode("a.com", 11211));
			pool.addNode(genNode("a.com", 11211));
		} catch (IllegalArgumentException e) {
			expOccurs = true;
		}
		assertTrue(expOccurs);
	}
	
	@Test
	public void testAddNode2() {
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName("haha");
		boolean expOccurs = false;
		try {
			pool.addNode(genNode("a.com", 11211));
			pool.addNode(genNode("a.com", 11211, 2));
		} catch (IllegalArgumentException e) {
			expOccurs = true;
		}
		assertTrue(expOccurs);
	}
	
	@Test
	public void testEqual() {
		
		CachePool pool1 = getPool1();
		
		assertFalse(pool1.equals(null));
		assertFalse(pool1.equals(new Object()));
		
		CachePool pool2 = getPool1();
		assertTrue(pool1.equals(pool2));
		
		((CachePoolImpl)pool2).addNode(genNode("c.com", 11211, 2));
		assertFalse(pool1.equals(pool2));
		
		pool2 = getPool2();
		assertFalse(pool1.equals(pool2));
	}
	
	private CachePool getPool1() {
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName("haha");
		pool.addNode(genNode("a.com", 11211));
		pool.addNode(genNode("a.com", 11212));
		pool.addNode(genNode("b.com", 11212));
		pool.addNode(genNode("b.com", 11211));
		return pool;
	}
	
	private CachePool getPool2() {
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName("haha");
		pool.addNode(genNode("a.com", 11211));
		pool.addNode(genNode("a.com", 11212));
		pool.addNode(genNode("b.com", 11212));
		pool.addNode(genNode("c.com", 11211));
		return pool;
	}
	
	public Node genNode(String hostname, int port) {
		return genNode(hostname, port, 1);
	}
	
	public Node genNode(String hostname, int port, int weight) {
		NodeImpl node = new NodeImpl();
		node.setHostname(hostname);
		node.setPort(port);
		node.setWeight(weight);
		return node;
	}
	
}
