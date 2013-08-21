/**
 * 
 */

package com.renren.xoa2.server.impl;

import java.lang.reflect.Method;
import java.util.AbstractMap;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.thrift.TProcessor;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TServerTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.backdoor.AbstractBackdoorService;

/**
 * XoaService对象表示一个运行的xoa服务，由<code>IServiceBuilder</code>构建而成
 * 
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-7
 */
public class XoaService implements Runnable {

    private Logger logger = LoggerFactory.getLogger(XoaService.class);

    private TProcessor processor;

    private TServerTransport serverTransport;

    private TServer server;
    
    private XoaServiceConfig config;
    
    private String confPath;
    
    private List<Thread> threadList;
    
    private List<XoaService> serverList;
    
    ScheduledThreadPoolExecutor timerExecutor = new ScheduledThreadPoolExecutor(2);
    
    ScheduledThreadPoolExecutor statisticsTimerExecutor = new ScheduledThreadPoolExecutor(2);
    
    private static final int SHIFTINDEX = 11;
    private static final int FIXED_1 = 1 << SHIFTINDEX;
    //private static final int LOAD_FREQ = 5;//每5秒钟采集一次
    private static final int EXP_1 = 1884;//公式所用到的常数2048/pow(exp, -5/60),1分钟统计一次
    private static final int EXP_5 = 2014;//公式所用到的常数2048/pow(exp, -5/300),5分钟统计一次
    private static final int EXP_15 = 2037; //公式所用到的常数2048/pow(exp, -5/900),15分钟统计一次
    
    
    //用来记录某个方法被调用了多少次
    private final ConcurrentHashMap<String, Long> serviceMethodCounts_ =
        new ConcurrentHashMap<String,  Long>();
    
    private final ConcurrentHashMap<String, String> serviceMethodOptions_ =
        new ConcurrentHashMap<String,  String>();   
    
    //用来记录某个函数在一天中的第N秒的调用花费最长的时间
    private final ConcurrentHashMap<String, int[]> serviceMethodTime_ = 
        new ConcurrentHashMap<String, int[]>();
        
    //用来记录某个函数在一天中的第N秒的时候被调用了M次 
    private final ConcurrentHashMap<String, long[]> serviceMethodCallTimes_ = 
        new ConcurrentHashMap<String, long[]>();
      
        
    //用来记录某个方法被调用的响应时间
    private final ConcurrentHashMap<String, Integer> serviceMethodCallTime_ =
        new ConcurrentHashMap<String,  Integer>();
    
    //1/5/15分钟平均调用时间
    private final ConcurrentHashMap<String, float[]> meanCallTime_ =
        new ConcurrentHashMap<String, float[]>();
       
    //1/5/15分钟平均调用次数 
    private final ConcurrentHashMap<String, float[]> callTimes_ =
        new ConcurrentHashMap<String, float[]>();   
        
    private static ConcurrentHashMap<String, Long> methodCallTimesbefore =
        new ConcurrentHashMap<String,  Long>(); 
    
    
    //construct by XoaServiceBuilder
    protected XoaService() {
    };

    @Override
    public void run() {
        try {
        	logger.info( config + " started.");
        	//只有对于继承了后门基类的才进行对结果进行统计和分析
        	if(Class.forName(this.config.getServiceClass()).newInstance() instanceof AbstractBackdoorService) {
        	    //测试阶段才开大数组计算极值，否则节省内存不予计算
        	    if(System.getProperty("test.phase") != null && System.getProperty("test.phase").equals("true")) {
        	        initStatisticsThread();
        	    }
        	    initStatisticsTimerExecutor();
        	}	
        	server.serve();
        	logger.info( config + " stoped.");
        } catch (Throwable t) {
        	logger.error("Exception occur: ", t);
        }
    }

    //============ getter and setter
    public TProcessor getProcessor() {
        return processor;
    }

    public TServerTransport getServerTransport() {
        return serverTransport;
    }

    public TServer getServer() {
        return server;
    }

    public void setProcessor(TProcessor processor) {
        this.processor = processor;
    }

    public void setServerTransport(TServerTransport serverTransport) {
        this.serverTransport = serverTransport;
    }

    public void setServer(TServer server) {
        this.server = server;
    }
    
    public XoaServiceConfig getConfig() {
        return config;
    }
    
    public void setConfig(XoaServiceConfig config) {
        this.config = config;
    }
    
    public void setConfPath(String confPath) {
        this.confPath = confPath;
    }
    
    public String getConfPath() {
        return confPath;
    }
    
    public void setThreadList(List<Thread> threadList) {
        this.threadList = threadList;
    }
    
    public List<Thread> getThreadList() {
        return threadList;
    }
    
    public void setServerList(List<XoaService> serverList) {
        this.serverList = serverList;
    }
    
    public List<XoaService> getServerList() {
        return serverList;
    }
    
    public void setOption(String key, String value) {
        serviceMethodOptions_.put(key, value);
    }

    public String getOption(String key) {
        return serviceMethodOptions_.get(key);
    }

    public AbstractMap<String,String> getOptions() {
        return serviceMethodOptions_;
    }
    
    
    //每次在使用方法的时候调用这个函数，可以产生一些统计信息
    public void setCallMethodInfo(String methodName, int methodTime) {   
        if(!isMethodInClass(methodName)){
            return ;
        }
        synchronized(this) {
            if(serviceMethodCounts_.get(methodName) == null) { 
                serviceMethodsInit(methodName);
            }
            incrementCounter(methodName);
            if(System.getProperty("test.phase") != null && System.getProperty("test.phase").equals("true")) {
                int secondOfDay = calcSecond();
                serviceMethodCallTimes_.get(methodName)[secondOfDay] = serviceMethodCounts_.get(methodName);
                if(methodTime > serviceMethodTime_.get(methodName)[secondOfDay]) {
                    serviceMethodTime_.get(methodName)[secondOfDay] = methodTime;
                }
            }
            
            serviceMethodCallTime_.put(methodName, methodTime);
        }
    }
    
    //传入时间，得到最近一段时间内的调用次数
    public String getCallTimes(String methodName) {
        StringBuffer sbCallTimes = new StringBuffer();
        sbCallTimes.append(methodName).append(" method call times is ").append((int)callTimes_.get(methodName)[0]*12).append("  ")
        .append((int)callTimes_.get(methodName)[1]*60).append("  ").append((int)callTimes_.get(methodName)[2]*180)
        .append(" in 1/5/15 min");
        return sbCallTimes.toString();
    }
    
    //得到一段时间内某方法的平均响应时间
    public String getMeanTime(String methodName) {
        StringBuffer sbMeanCallTime = new StringBuffer();
        sbMeanCallTime.append(methodName).append(" method mean call time is ").append((int)meanCallTime_.get(methodName)[0]).append("  ")
        .append((int)meanCallTime_.get(methodName)[1]).append("  ").append((int)meanCallTime_.get(methodName)[2])
        .append(" in 1/5/15 min");
        return sbMeanCallTime.toString();
    }
    
    //得到一段时间内某方法最大并发数
    public String getMaxConcurrentTimes(String methodName, int second) {
        int currentTime = calcSecond();
        int maxConcurrentTimes = 0;
        for(int i = currentTime; i >= currentTime-second; --i) {
            if(maxConcurrentTimes < (serviceMethodCallTimes_.get(methodName)[getModule(i)] - serviceMethodCallTimes_.get(methodName)[getModule(i-1)])) {
                maxConcurrentTimes = (int)(serviceMethodCallTimes_.get(methodName)[getModule(i)] - serviceMethodCallTimes_.get(methodName)[getModule(i-1)]);
            }
        }
        StringBuffer sbMaxConcurrentTimes = new StringBuffer();
        sbMaxConcurrentTimes.append(methodName).append(" max concurrent times is ").append(maxConcurrentTimes).append(" in ")
        .append(second).append(" second ");
        return sbMaxConcurrentTimes.toString();
    }
    
    //得到一段时间内某个方法最长响应时间
    public String getMaxCallTime(String methodName, int second) {
        int currentTime = calcSecond();
        int maxCallTime = 0;
        for(int i = currentTime; i >= currentTime-second; --i) {
            if(maxCallTime < serviceMethodTime_.get(methodName)[getModule(i)]) {
                maxCallTime = serviceMethodTime_.get(methodName)[getModule(i)];
            }
        }
        StringBuffer sbMaxCallTime = new StringBuffer();
        sbMaxCallTime.append(methodName).append(" max call time is ").append(maxCallTime).append(" in ")
        .append(second).append("ms second ");
        return sbMaxCallTime.toString();
    }
    
    
    public long getCounter(String key) {
        synchronized(this) {
            Long val = serviceMethodCounts_.get(key);
            if (val == null) {
                return 0;
            }
            return val.longValue();
        }
    }
    

    public AbstractMap<String,Long> getCounters() {
        return serviceMethodCounts_;
    }
    
    //一个用于统计数据的线程,每秒钟保存一次数据
    private void initStatisticsThread() {
        timerExecutor.scheduleAtFixedRate(new Runnable() {
            public void run() {
                try {
                    synchronized(this) {
                        Iterator<String> ite=serviceMethodCallTimes_.keySet().iterator();
                        int secondOfDay = calcSecond();
                        while (ite.hasNext()) {  
                            String key = ite.next();
                          
                            long longValue = serviceMethodCallTimes_.get(key)[getModule(secondOfDay-1)];
                            serviceMethodCallTimes_.get(key)[secondOfDay] = longValue;
                        }
                    }  
                } catch (Throwable e) {
                    System.out.println("init statistics thread exception");
                }
            }
        }, 1, 1, TimeUnit.SECONDS);
    }
    
    //一个用于统计数据的线程,每五秒钟采集一次数据
    private void initStatisticsTimerExecutor() {
        statisticsTimerExecutor.scheduleAtFixedRate(new Runnable() {
            public void run() {
                try {
                    synchronized(this) {
                        Iterator<String> ite=serviceMethodCounts_.keySet().iterator();
                        while (ite.hasNext()) {  
                            String key = ite.next();       
                            calcStatisticsCallTime(key, serviceMethodCallTime_.get(key));
                            calcStatisticsCallTimes(key, (int)(serviceMethodCounts_.get(key)-methodCallTimesbefore.get(key)));
                            methodCallTimesbefore.put(key, serviceMethodCounts_.get(key));
                        }
                    }  
                } catch (Throwable e) {
                    System.out.println("init statistics timer executor exception");
                }
            }
        }, 1, 5, TimeUnit.SECONDS);
    }
    
    
    public void initService() {
        Iterator<String> it = serviceMethodCounts_.keySet().iterator();    
        while(it.hasNext()) {
            resetCounter(it.next());  
        }
        
        it = serviceMethodOptions_.keySet().iterator();
        while(it.hasNext()) {
            it.next();                           
            it.remove();
        }
    }
    
    private void resetCounter(String key) {
        synchronized(this) {
            serviceMethodCounts_.put(key, 0L);
        }
    }
    
    //用来计算当前时间为当天的第多少秒
    private int calcSecond() {
        int time = (int)(((System.currentTimeMillis()+(8*3600*1000))%(1000*24*60*60))/1000);
        return time;
    }
    
    //修正了取模方法(使取模后的结果非负)
    private int getModule(int time) {
        int index;
        if(time <= 0) {
            index = time + 86400;
        } else {
            index = time;
        }
        return index % 86400;
    }
    
    private long incrementCounter(String key) {
        long val = getCounter(key) + 1;
        serviceMethodCounts_.put(key, val);
        return val;
    }
    
    
    private void serviceMethodsInit(String name) {
        if(System.getProperty("test.phase") != null && System.getProperty("test.phase").equals("true")) {
            serviceMethodTime_.put(name, new int[86401]);
            serviceMethodCallTimes_.put(name, new long[86401]);
        }
        meanCallTime_.put(name, new float[3]);
        callTimes_.put(name, new float[3]);
        methodCallTimesbefore.put(name, 0L);
    }
    
    //用于判断是否为需要统计的方法
    private boolean isMethodInClass(String methodName) {
        try {
            Class<?> c=Class.forName(this.config.getServiceClass());  
            Method[] methodNames = c.getDeclaredMethods();
            for(int i = 0; i < methodNames.length; i++){
                if(methodNames[i].getName().equals(methodName)){
                    return true;
                }
            }
        } catch(ClassNotFoundException e) {
            if(logger.isWarnEnabled()) {
                logger.warn("Method In Class Not Found Exception");
            }
            return false;
        }
        return false;
    }
    
    //用来计算1分钟，5分钟和15分钟内的每次函数调用的平均响应时间
    private void calcStatisticsCallTime(String name, int callTime) {     
        meanCallTime_.get(name)[0] = calcBase(meanCallTime_.get(name)[0], EXP_1, callTime);
        meanCallTime_.get(name)[1] = calcBase(meanCallTime_.get(name)[1], EXP_5, callTime);
        meanCallTime_.get(name)[2] = calcBase(meanCallTime_.get(name)[2], EXP_15, callTime);
    }
    
    //用来计算1分钟，5分钟和15分钟内的每次函数调用的平均调用次数
    private void calcStatisticsCallTimes(String name, int callTimes) {     
        callTimes_.get(name)[0] = calcBase(callTimes_.get(name)[0], EXP_1, callTimes);
        callTimes_.get(name)[1] = calcBase(callTimes_.get(name)[1], EXP_5, callTimes);
        callTimes_.get(name)[2] = calcBase(callTimes_.get(name)[2], EXP_15, callTimes);
    }
    
    
    private float calcBase(float load, int exp, int n) {
        load *= exp;
        load += n*(FIXED_1-exp);
        load = load /(1 << SHIFTINDEX);
        return load;
    }
}
