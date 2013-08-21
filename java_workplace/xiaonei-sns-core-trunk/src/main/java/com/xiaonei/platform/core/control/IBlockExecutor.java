/**
 * $Id: IBlockExecutor.java 14202 2010-01-07 03:42:21Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;

/**
 * Execute a block service.
 * 
 * <pre>
 * <code>
 *         Object obj = new BlockExecutor().run(new IBlockable() {
 * 
 *             public Object call() throws Exception {
 *                 Thread.sleep(1000);//do your real work
 *                 return Boolean.TRUE;
 *             }
 * 
 *             public String getName() {
 *                 return "test";
 *             }
 *         });
 *         System.out.println(obj);
 * 
 * </code>
 * </pre>
 * 
 * In your 'Spring' environment, you can using
 * {@link org.springframework.beans.factory.annotation.Autowire} with this.
 * 
 * @author xylz(xylz@live.cn)
 * @since 2010-1-7
 */
public interface IBlockExecutor {

    /**
     * execute a service in 'Block' mode with given 'enable' state and
     * 'timeout'.
     * 
     * @param blockable the service
     * @param enable enable state of service
     * @param timeout the max time for service
     * @return the result of service
     * @throws BlockException if timeout or some error happended.
     */
    Object run(IBlockable blockable, boolean enable, int timeout) throws BlockException;

    /**
     * execute a service in 'Block' mode.<br />
     * The block enable and timeout depends the executing thread if
     * arguments exist.
     * 
     * @param blockable the service
     * @return the result of the service
     * @throws BlockException if timeout or some error happended.
     * @see #run(IBlockable, boolean, int)
     * @see {@link BlockService}
     * @see {@link ThreadLocal}
     */
    Object run(IBlockable blockable) throws BlockException;

}
