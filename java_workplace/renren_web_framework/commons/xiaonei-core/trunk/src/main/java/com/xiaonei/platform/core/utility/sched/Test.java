package com.xiaonei.platform.core.utility.sched;

public class Test {
    public static void main(String args[])throws Exception{
    	MySched ms = new MySched() ;
    	SchedManager.getInstance().addTask(ms) ;
    	
    	System.out.println("----------") ;
    	Thread.sleep(1000*4) ;
    }
}
