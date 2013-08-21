package com.xiaonei.platform.core.cache;

import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;

public class MemUpdateMonitor extends Thread{

	private String lastUpdateTime = null;
	private int sleepTime = 1000*60*3;//two min
	//private int sleepTime = 1000;
	private long count;
	private volatile boolean stop = false;
	private static String status = "show table status like 'memconfig1'";
	
	public void stopMonitor(){
		stop = true;
	}
	
	/**
	 * 每2分钟检查一次Pool的初始化状况。每4分钟检查一下配置更新。
	 */
	public void run() {
		System.out.println("MemCached Monitor Start");
		while(!stop){
			count++;
			try{
				Thread.sleep(sleepTime);
				MemCacheFactory.getInstance().checkPoolInit();
				if(count%2==0){
					if(isUpdate()){
						System.out.println("Start Update MemCached Config");
						MemCacheFactory.getInstance().update(lastUpdateTime);
					}else{
						System.out.println("Config Not Update");
					}
				}
			}catch(Exception ex){
				ex.printStackTrace();
			}
		}
		System.out.println("MemConfig Monitor Stop");
	}
	
	private boolean isUpdate() throws SQLException{
		OpUniq op = new OpUniq(status,DataAccessMgr.BIZ_STAT){
			public Object parse(ResultSet rs) throws SQLException{
				return rs.getString("Update_time");
			}
		};
		
		String stamp = (String)DataAccessMgr.getInstance().queryUnique(op);
		
		if(lastUpdateTime == null){
			lastUpdateTime = stamp;
			return false;
		}else{
			if(stamp.equals(lastUpdateTime))return false;
			System.out.println("last="+lastUpdateTime+":now="+stamp);
			lastUpdateTime = stamp;
			return true;
		}
	}
	
}