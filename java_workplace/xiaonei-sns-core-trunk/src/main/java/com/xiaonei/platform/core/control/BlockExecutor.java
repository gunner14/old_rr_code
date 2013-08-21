/**
 * $Id: BlockExecutor.java 16542 2010-02-05 11:21:53Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;

import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

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
 * 
 * @author xylz(xylz@live.cn)
 * @since 2010-1-6
 */
public class BlockExecutor implements IBlockExecutor{

    private static final Log log = LogFactory.getLog(BlockExecutor.class);
    static final String performace_log = "Cutter.ThreadPool:TaskCount(%d),ActiveCount(%d),PoolSize(%d),CorePoolSize(%d),LargestPoolSize(%d),CompletedTaskCount(%d)";
    private static final ThreadPoolExecutor service = new ThreadPoolExecutor(200, Integer.MAX_VALUE,
            60L, TimeUnit.SECONDS,//
            new SynchronousQueue<Runnable>()//
            , new CutterThreadFactory());
    static final ScheduledExecutorService performaceService = Executors.newSingleThreadScheduledExecutor();
    static final ThreadCount threadCount = new ThreadCount();
    static {
        performaceService.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                log.info(threadCount.toString());
            }
        },  30, 30, TimeUnit.SECONDS);
    }

    @Override
    public  Object run(IBlockable blockable) throws BlockException {
        return run(blockable, BlockService.isEnabled(), BlockService.getTimeout());
    }

    protected boolean getDefaultEnable() {
        return ConfigCenter.getBoolean("SERVICE_CUTTER", false);
    }
    
    @Override
    public  Object run(IBlockable blockable, boolean enable, int timeout)
            throws BlockException {
        if(blockable==null)throw new NullPointerException("the 'blockable' is null");
        try {
            if (getDefaultEnable() && enable && timeout >= 0) {//block service is enabled.
                if(log.isDebugEnabled()) {
                    log.debug(String.format("execute service(%s,%s:%s ms) in block mode.", blockable.getName(),enable,timeout));
                }
                if (blockable instanceof ILogBlockable) {//记录超时？
                    ILogBlockable logBlockable = (ILogBlockable) blockable;
                    logBlockable.setTimeout(timeout);
                }
                Future<Object> task = service.submit(blockable);
                return task.get(timeout, TimeUnit.MILLISECONDS);
            }
            return blockable.call();
        } catch (InvocationTargetException ite) {
            Throwable t = ite.getCause();// throw the real exception
            if(t==null)new BlockException(ite);
            int c=0;
            while (t instanceof InvocationTargetException) {
                Throwable tmp = t.getCause();
                if(tmp==null)break;
                t=tmp;
                if(++c>5)
                    break;//5层都没完没了！！！
            }
            if(t instanceof BlockException)
                throw (BlockException)t;
            throw new BlockException("call real service error",t);
        } catch (TimeoutException te) {
            if(log.isDebugEnabled()) {
                log.debug(blockable.getName() + " ==> service timeout. ");
            }
            throw new ServiceTimeoutException(blockable.getName() + "@# service timeout. "
                    ,te);
        }catch(BlockException be) {
            throw be;
        }
        catch (Exception ex) {
            throw new BlockException(ex.getMessage(),ex);
        }
    }

    public static ThreadCount getThreadcount() {
        return threadCount;
    }
    
    public static class ThreadCount{
        private ThreadCount() {
        }
        
        public long getTaskCount() {
            return service.getTaskCount();
        }
        public long getActiveCount() {
            return service.getActiveCount();
        }
        public long getPoolSize() {
            return service.getPoolSize();
        }
        
        public long getCorePoolSize() {
            return service.getCorePoolSize();
        }
        
        public long getLargestPoolSize() {
            return service.getLargestPoolSize();
        }
        public long getCompletedTaskCount() {
            return service.getCompletedTaskCount();
        }
        @Override
        public String toString() {
            return String.format(performace_log,//
                    getTaskCount()//
                    ,getActiveCount()//
                    ,getPoolSize()//
                    ,getCorePoolSize()//
                    ,getLargestPoolSize()//
                    ,getCompletedTaskCount()//
                    );
        }
        
    }
}
