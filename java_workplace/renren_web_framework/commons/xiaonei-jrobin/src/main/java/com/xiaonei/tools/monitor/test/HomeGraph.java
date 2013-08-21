package com.xiaonei.tools.monitor.test;
import java.awt.Color;
import java.io.IOException;

import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdException;
import org.jrobin.graph.RrdGraph;
import org.jrobin.graph.RrdGraphDef;
import org.jrobin.graph.TimeAxisUnit;

import com.xiaonei.tools.monitor.RrdDbManager;


public class HomeGraph {
	public static void main(String[] args) throws IOException, RrdException{
		String rrdFile = "/home/wenwen/jrobin/xiaonei.com.rrd", pngFile = "/home/wenwen/jrobin/www3.png";
//		RrdDb db = RrdDbManager.getInstance().requestRrd(rrdFile, 0);
//		
//		long start = db.getLastUpdateTime() - 3600;
//		RrdGraphDef gDef = new RrdGraphDef();
//		gDef.setTimePeriod(start, start + 3600);
//		gDef.setTitle("xiaonei.com");
//		
//		//gDef.datasource("access", rrdFile, "access", "LAST");
//		gDef.datasource("access_avg", rrdFile,"access","AVERAGE");
//		gDef.setFrameColor(Color.WHITE);
//		//gDef.line("access", Color.GREEN, "last",2);
//		gDef.area("access_avg", Color.green, "avg");
//		gDef.setMinorGridColor(Color.LIGHT_GRAY);
//		gDef.setMajorGridColor(Color.pink);
//		gDef.setAxisColor(Color.BLACK);
//		gDef.setTimeAxis(TimeAxisUnit.MINUTE, 5, TimeAxisUnit.MINUTE, 20, "HH:mm", false);
//		gDef.hrule(300, Color.RED, null);
//		RrdGraph graph = new RrdGraph(gDef);
//		
//		graph.saveAsPNG(pngFile, 297, 0);
	}
}
