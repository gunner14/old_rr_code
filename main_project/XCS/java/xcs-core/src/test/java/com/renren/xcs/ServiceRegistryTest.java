package com.renren.xcs;

import junit.framework.Assert;
import junit.framework.TestCase;

import org.junit.Before;
import org.junit.Test;

public class ServiceRegistryTest extends TestCase {

    ServiceRegistry reg = null;
    String zkStr = "10.3.18.186:2181";
    String serverId = "test2.xcs.renren.com";
    String [] args = new String[4];

    /* (non-Javadoc)
     * @see junit.framework.TestCase#setUp()
     */
    @Override
    protected void setUp() throws Exception {
        super.setUp();
        args[0] = new String("-server");
        args[1] = zkStr;
        args[2] = new String("xcsCreate");
        args[3] = new String("test2.xcs.renren.com");
// TODO 编译不过去，临时注释掉
//        CommandLineConsole console = new CommandLineConsole(args);
//        console.process();
    }
    
    /* (non-Javadoc)
     * @see junit.framework.TestCase#tearDown()
     */
    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        Thread.sleep(500);
        
        // args[2] = new String("xcsDelete");
        // CommandLineConsole consoleD = new CommandLineConsole(args);
        // consoleD.process();
    }
    
    @Test
    @Before
    public void testRegistry() {
        try {
            reg = new FindServiceRegistry(new TestConfiguration());
        } catch (Exception e) {
            e.printStackTrace();
            Assert.assertFalse(true);
        }
        Assert.assertNotNull(reg);

        
        try {
            reg.publishService(serverId, 1, "0", new Node("127.0.0.1:1234",
                    "state=0\ndisabled=false\nhealthy=true\n".getBytes()), true);

            System.out.println("Done!");
        } catch (XcsException e) {
            e.printStackTrace();
            Assert.fail();
        } catch (Exception e) {
            Assert.fail();
        }
    }
}
