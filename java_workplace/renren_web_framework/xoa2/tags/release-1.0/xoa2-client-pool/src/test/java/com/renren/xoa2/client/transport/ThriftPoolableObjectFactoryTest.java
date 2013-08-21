package com.renren.xoa2.client.transport;

import static org.junit.Assert.*;

import java.net.ServerSocket;

import org.apache.commons.pool.ObjectPool;
import org.apache.commons.pool.impl.GenericObjectPool;
import org.apache.thrift.transport.TTransport;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

/**
 * @date Mon Mar 19 12:24:14 2012
 *
 * @author yuan.liu1@renren-inc.com
 *
 */
public class ThriftPoolableObjectFactoryTest {

	final static String LOCAL_HOST = "127.0.0.1";
	final static int SERVER_PORT = 9000;

	static ServerSocket server = null;

	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		server = new ServerSocket(SERVER_PORT, 2);
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
		server.close();
	}

	@Before
	public void setUp() throws Exception {
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testMakeBorrowObject() throws Exception {
        ThriftPoolableObjectFactory poolObjectFactory =
            new ThriftPoolableObjectFactory(LOCAL_HOST, SERVER_PORT, 0);

        ObjectPool objectPool = new GenericObjectPool();
        objectPool.setFactory(poolObjectFactory);
        
        
        TTransport trans = (TTransport)objectPool.borrowObject();
        assertTrue(trans.peek());
        assertEquals(1, objectPool.getNumActive());
        
        TTransport trans2 = (TTransport)objectPool.borrowObject();
        assertTrue(trans2.peek());
        assertEquals(2, objectPool.getNumActive());

        objectPool.returnObject(trans);
        assertEquals(1, objectPool.getNumActive());
        
        objectPool.returnObject(trans);
        assertEquals(0, objectPool.getNumActive());

        // GenericObjectPool 的行为比较奇特，需要关注        
        objectPool.returnObject(trans2);
        assertEquals(-1, objectPool.getNumActive());
        
        objectPool.returnObject(trans2);
        assertEquals(-2, objectPool.getNumActive());
	}
}
