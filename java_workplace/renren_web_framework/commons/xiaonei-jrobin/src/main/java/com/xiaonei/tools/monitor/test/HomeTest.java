package com.xiaonei.tools.monitor.test;
import static org.junit.Assert.*;

import java.io.IOException;
import java.util.Date;

import org.jrobin.core.FetchData;
import org.jrobin.core.FetchPoint;
import org.jrobin.core.FetchRequest;
import org.jrobin.core.RrdDb;
import org.jrobin.core.RrdDbPool;
import org.jrobin.core.RrdException;
import org.jrobin.core.Util;
import org.junit.Test;


public class HomeTest {
	@Test
	public void test(){
		System.out.println(System.currentTimeMillis());
		java.util.Date date = new Date(1246517991L*1000);
		System.out.println(Util.getTime());
		System.out.println(date.toLocaleString());
		System.out.println(1246518843L-1246517991L);
	}
	@Test
	public void testFetch() throws IOException, RrdException{
		RrdDb rrd = new RrdDb("/home/wenwen/jrobin/home.rrd");

		FetchRequest request = rrd.createFetchRequest("AVERAGE", 1246517991L, 1246517991L+3600);

		// execute the request
		FetchData fetchData = request.fetchData();

		// From a logical point of view FetchData object is, simply, 
		// an array of FetchPoint objects. Each FetchPoint object 
		// represents all datasource values for the specific timestamp.
		// Here is how you can produce the same output to stdout 
		// as RRDTool's fetch command
		for(int i = 0; i < fetchData.getRowCount(); i++) {
		    FetchPoint point = fetchData.getRow(i);
		    System.out.println(point.dump());
		}

	}
	@Test
	public void testRrd() throws IOException, RrdException{
		//RrdDb  db = new RrdDb("/home/wenwen/jrobin/test.rrd");
		RrdDbPool pool = RrdDbPool.getInstance();
		RrdDb  db = pool.requestRrdDb("/home/wenwen/jrobin/test.rrd");
		assertTrue(db.getRrdDef()!=null);
	}
}
