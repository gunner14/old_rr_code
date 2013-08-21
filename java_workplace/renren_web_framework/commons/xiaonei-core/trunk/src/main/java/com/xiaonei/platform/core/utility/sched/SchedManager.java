/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

import org.apache.log4j.Logger;
import org.dom4j.Document;

import com.xiaonei.platform.core.utility.stat.StatsSchedTask;


/**
 * ������������
 * ϵͳ��ʼ��ʱװ�أ���(һ���̳߳غ�һ���������
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class SchedManager implements SchedThreadDeathListener {

	public static final String ROLE = SchedManager.class.getName();
	
	public static final int DEFAULT_THREADS = 10;
	
	private SchedTaskQueue taskQueue;
	
	private SchedThreadPool threadPool;
	
	private static SchedManager instance;
	private Logger logger = Logger.getLogger(this.getClass().getName()) ;
	private int threadCount;
	
	/**
	 * ȡ�õ��������������ʵ��
	 * 
	 * @return instance
	 */
	public static SchedManager getInstance() {
		if (instance == null) {
			synchronized (SchedManager.class) {
				if (instance == null)
					instance = new SchedManager();
			}

		}
		return instance;
	}
	public SchedManager(){
		this.init() ;
	}
	/**
	 * �������
	 * 
	 * @param sched
	 * @return true if success
	 */
	public boolean addTask(Schedulable sched) {
		SchedTask task = new SchedTask(taskQueue, sched);
		return taskQueue.addTask(task);
	}
	
	/**
	 * �������
	 * 
	 * @param sched
	 * @return true if success
	 */
	public boolean addTask(Schedulable sched, boolean first) {
		SchedTask task = new SchedTask(taskQueue, sched);
		task.setFirstExec(first);
		
		return taskQueue.addTask(task);
	}

	/**
	 * ���ÿ�ն���ִ������Ĭ��02:00:00
	 * 
	 * @param sched
	 * @return
	 */
	public boolean addDailyStatsTask(Schedulable sched) {
		StatsSchedTask task = new StatsSchedTask(taskQueue, sched);
		return taskQueue.addTask(task);
	}
	
	/**
	 * ���ÿ�ն���ִ������time��ʾ���ʱ�䣬��ʽ
	 * 
	 * @param sched
	 * @param time
	 * @return
	 */
	public boolean addDailyStatsTask(Schedulable sched, int time) {
		StatsSchedTask task = new StatsSchedTask(taskQueue, sched);
		task.setSchedTime(time);
		return taskQueue.addTask(task);
	}

	/**
	 * ���ÿ�ն���ִ������time��ʾ���ʱ�䣬��ʽΪHHmmss����010000
	 * ��һ�β���ִ��
	 * 
	 * @param sched
	 * @param time
	 * @return
	 */
	public boolean addDailyStatsTask(Schedulable sched, int time, boolean first) {
		StatsSchedTask task = new StatsSchedTask(taskQueue, sched);
		task.setSchedTime(time);
		task.setFirstExec(first);

		return taskQueue.addTask(task);
	}

	/**
	 * ȡ�õ�ǰ���еȴ��ȣ�����(����ִ�У�������
	 * 
	 * @return task array
	 */
	public SchedTask[] getTasks() {
		return this.taskQueue.getTasks();
	}
	
	/**
	 * ȡ�õ�ǰ��Ծ���߳���
	 * 
	 * @return
	 */
	public int getActiveThreadCount() {
		return this.threadPool.activeCount();
	}
	public void init(){
		Document doc = null ;
		/*
		Properties prop = ReadProperty.getProperties("config_path.properties");
		String config = prop.getProperty("schedu_config_path");
		
		try{
			doc =ReadProperty.read(config) ; 
		}catch(Exception e){
			e.printStackTrace(System.err) ;
		}*/
		this.configure(doc);
		this.initialize() ;
	}
	/* (non-Javadoc)
	 * @see org.apache.avalon.framework.activity.Initializable#initialize()
	 */
	public void initialize() {
		if (instance == null) {
			
			instance = this;
	
			for (int i=0; i<this.threadCount; i++) {
				SchedThread thrd = new SchedThread(this.taskQueue, this.threadPool, "Schedule-Thread" + (i+1));
				thrd.start();
	
				System.out.println("Schedule thread [" + thrd.getName() + "] started.");
			}
		}
	}

	/* (non-Javadoc)
	 * @see org.apache.avalon.framework.configuration.Configurable#configure(org.apache.avalon.framework.configuration.Configuration)
	 */
	public void configure(Document doc) {
		
		this.taskQueue = new SchedTaskQueue();
		this.threadPool = new SchedThreadPool("Schedule-ThreadPool", this);
         
		this.threadCount = DEFAULT_THREADS;
		/*
		String thrdsconf =null;
		if(doc !=null){
		    Element root = doc.getRootElement() ;
		    thrdsconf = root.elementText("threads");
		    System.out.println(thrdsconf) ;
		    this.threadCount = new Integer(thrdsconf).intValue() ;
		}
		
		if (thrdsconf == null) { 
			this.threadCount = (DEFAULT_THREADS);
		}
		
		List  <Element>tasks = null ;
		if(doc !=null){
		    tasks = doc.getRootElement().elements("sched-tasks");
		   
		}
		if ( tasks != null ) {
			for (int i=0; i<tasks.size(); i++) {
				try {
					System.out.println("tasks:"+tasks.get(i).elementText("sched-task")) ;
					Schedulable sched = (Schedulable) ClassUtils.createInstance(tasks.get(i).elementText("sched-task"));
					this.addTask(sched);
				
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		*/
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.SchedThreadDeathListener#notifyThreadEvent(com.dudu.sched.SchedThreadPool, java.lang.Thread)
	 */
	public void notifyThreadEvent(SchedThreadPool pool, Thread t, Throwable e) {
        this.logger.info("Uncaught Thread Exception " + e.getMessage(), e);
        this.logger.info("Restart Thread " + t.getName(), null);

		t.interrupt();

		try {
			Thread.sleep(5000);
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		}

		if (pool.activeCount() < this.threadCount) {
			SchedThread thrd = new SchedThread(this.taskQueue, this.threadPool, t.getName());
			thrd.start();
			this.logger.info("Already Restarted Thread " + t.getName(), null);
		}
	}

	/* (non-Javadoc)
	 * @see org.apache.avalon.framework.activity.Disposable#dispose()
	 */
	public void dispose() {
		this.logger.info("SchedManager disposed.", null);
		try {
			Thread[] thrds = new Thread[this.threadPool.activeCount()];
			this.threadPool.enumerate(thrds);
			for (int i=0; i<thrds.length; i++) {
				thrds[i].interrupt();
			}
			this.threadPool.destroy();
			this.taskQueue.clear();
		} catch ( Exception e ) {
			e.printStackTrace();
		}
	}
	public static void main(String args[]){
		SchedManager.getInstance() ;
		System.out.println(4&3);
		System.out.println(3/4);
	}

}
