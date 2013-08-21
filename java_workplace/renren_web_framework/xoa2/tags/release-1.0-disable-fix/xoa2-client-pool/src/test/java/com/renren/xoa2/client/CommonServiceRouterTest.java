package com.renren.xoa2.client;

import static org.junit.Assert.*;

import java.net.ServerSocket;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.xoa2.client.transport.XoaTransport;
import com.renren.xoa2.registry.Node;

public class CommonServiceRouterTest {

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
	public void testRouteService() throws Exception {
        CommonServiceRouter router = CommonServiceRouter.getInstance();
        LoadBalancer lb = new FakeBalancer();
        router.setLoadBalancer(lb);
        
        XoaTransport xoa = router.routeService("9000", "");
        Node node = xoa.getNode();
        
        assertEquals(LOCAL_HOST, node.getHost());
        assertEquals(SERVER_PORT, node.getPort());
        
        xoa = router.routeService("9000", "");
        node = xoa.getNode();

        assertEquals(LOCAL_HOST, node.getHost());
        assertEquals(SERVER_PORT, node.getPort());

        router.returnConn(xoa);
    }
}
