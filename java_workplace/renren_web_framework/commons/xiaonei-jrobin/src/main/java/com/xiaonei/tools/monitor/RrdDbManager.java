package com.xiaonei.tools.monitor;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.log4j.Logger;
import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdDbPool;
import org.jrobin.core.RrdDef;
import org.jrobin.core.RrdException;

public class RrdDbManager {
	static Logger logger = Logger.getLogger(RrdDbManager.class);
	private static RrdDbManager instance = new RrdDbManager();
	private static Map<String,String> cache = new HashMap<String,String>();
	private static RrdDbPool pool =RrdDbPool.getInstance();
	private  RrdDbManager(){
		
	}
	public static RrdDbManager getInstance(){
		return instance;
	}
	public RrdDb requestRrd(String rrdPath,long startTime,String dsName,String dsType) throws IOException, RrdException{
		if(rrdPath==null)return null;
		try{
		if(cache.containsKey(rrdPath))return pool.requestRrdDb(rrdPath);
		}catch (RrdException rd){
		    System.out.println("RRDEXCEPTION:"+rrdPath);
		}
		
		File file = new File(rrdPath);
		if(file.exists()){
			cache.put(rrdPath, rrdPath);
			return pool.requestRrdDb(rrdPath);
		}
		File fileDir= new File(rrdPath.substring(0,rrdPath.lastIndexOf("/")));
		if(!fileDir.exists()){
			fileDir.mkdirs();
		}
		long start = startTime-300;
		
		logger.info("Create rrdDB ..path is "+rrdPath);
		RrdDef rrdDef = new RrdDef(rrdPath, start, 3);//step=60s,1m
		rrdDef.addDatasource(dsName, dsType, 20, Double.NaN, Double.NaN);
		rrdDef.addArchive("AVERAGE",0.5,5,244);
		rrdDef.addArchive("AVERAGE",0.5,120,244);
		rrdDef.addArchive("AVERAGE",0.5,840,244);
		rrdDef.addArchive("AVERAGE",0.5,3360,244);
		rrdDef.addArchive("AVERAGE",0.5,28800,374);
		
		rrdDef.addArchive("MIN",0.5,5,244);
		rrdDef.addArchive("MIN",0.5,120,244);
		rrdDef.addArchive("MIN",0.5,840,244);
		rrdDef.addArchive("MIN",0.5,3360,244);
		rrdDef.addArchive("MIN",0.5,28800,374);
		
		rrdDef.addArchive("MAX",0.5,5,244);
		rrdDef.addArchive("MAX",0.5,120,244);
		rrdDef.addArchive("MAX",0.5,840,244);
		rrdDef.addArchive("MAX",0.5,3360,244);
		rrdDef.addArchive("MAX",0.5,28800,374);
		cache.put(rrdPath, rrdPath);
		return pool.requestRrdDb(rrdDef);
	}
}
