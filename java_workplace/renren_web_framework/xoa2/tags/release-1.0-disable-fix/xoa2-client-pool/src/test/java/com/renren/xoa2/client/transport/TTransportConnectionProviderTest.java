package com.renren.xoa2.client.transport;

import static org.junit.Assert.*;

import java.net.ServerSocket;

import org.apache.thrift.transport.TTransport;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.rules.ExpectedException;

import com.renren.xoa2.registry.impl.NodeImpl;

/**
 * @author yuan.liu1@renren-inc.com
 *
 */
public class TTransportConnectionProviderTest {

	final static String LOCAL_HOST = "127.0.0.1";
	final static int SERVER_PORT = 9000;

	static ServerSocket server = null;
	
	TTransportConnectionProvider con;
	NodeImpl node;
	
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
		con = new TTransportConnectionProvider();
		node = new NodeImpl();
		node.setHost(LOCAL_HOST);
		node.setPort(9000);
		con.createPool(node);
	}

	@After
	public void tearDown() throws Exception {
		con.clearConnections(node);
	}
	
	@Test
	public void testCreatePool() throws Exception {
		for (int i = 0; i < 100; i++) {
			TTransport trans = con.getConnection(node);
			assertNotNull(trans);
			XoaTransport xoaTrans = new XoaTransport();
			xoaTrans.setNode(node);
			xoaTrans.setTransport(trans);
			con.returnConnection(xoaTrans);
		}

		System.out.println(TTransportConnectionProvider.getConnStatus());
	}
	
	@Test
	public void testGetConnection() throws Exception {
		TTransport trans = con.getConnection(node);
		assertNotNull(trans);
		
		TTransport trans2 = con.getConnection(node);
		assertNotNull(trans2);

		TTransport trans3 = con.getConnection(node);
		assertNotNull(trans3);

		System.out.println(TTransportConnectionProvider.getConnStatus());
	}
	
	@Test
	public void testReturnConn() throws Exception {
		XoaTransport xoaTrans = new XoaTransport();

		xoaTrans.setNode(node);

		TTransport trans = con.getConnection(node);
		xoaTrans.setTransport(trans);
		
		con.returnConnection(xoaTrans);
		con.returnConnection(xoaTrans);
	}
}
