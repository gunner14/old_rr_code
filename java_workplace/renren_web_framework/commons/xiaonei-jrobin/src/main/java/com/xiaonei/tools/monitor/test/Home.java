package com.xiaonei.tools.monitor.test;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdDef;
import org.jrobin.core.RrdException;
import org.jrobin.core.Sample;
import org.jrobin.core.Util;


public class Home {
	public static void main(String[] args) throws IOException, RrdException{
		long start = Util.getTime()-3600*24;
		String rrdFile = "/home/wenwen/jrobin/home.rrd";
		File file = new File(rrdFile);
		RrdDb rrdDb = null;
		if(!file.exists()){
			// create
			RrdDef rrdDef = new RrdDef(rrdFile, start - 1, 1);
			rrdDef.addDatasource("home", "GAUGE", 600, Double.NaN, Double.NaN);
			rrdDef.addArchive("AVERAGE", 0.5, 10, 3600*4);
			rrdDef.addArchive("LAST",0.5,10,3600*4);
			rrdDef.addArchive("MIN", 0.5, 12, 300);
			rrdDef.addArchive("MAX", 0.5, 12, 300);
			rrdDb = new RrdDb(rrdDef);
		}else{
			rrdDb = new RrdDb(rrdFile);
		
		}
		
		
		File log = new File("/home/wenwen/jrobin/home.log");
		BufferedReader bufferReader = new BufferedReader(new FileReader(log));
		String line = null;
		long lastTime = 0,currTime = 0;
		while((line = bufferReader.readLine())!=null){
			String[] terms = line.split(" ");
			
			if(terms.length!=4||!terms[2].equals("/Home.do"))continue;
			
			currTime = Long.parseLong(terms[0]);
			if(currTime <= lastTime){
				continue;
			}
			lastTime = currTime;
			Sample sample = rrdDb.createSample(currTime);
			sample.setValue("home", Long.parseLong(terms[3]));
			//System.out.println(sample);
			sample.update();
		}
	}
}
