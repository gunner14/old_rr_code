package com.xiaonei.platform.core.utility;

import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * Add description here
 * 
 * Created on 2004-3-723:03:25
 *
 * @auther Freebat Wangh<wanghuiwen@tsinghua.org.cn>
 */
public class CSV {
	public static ArrayList getAll(String path) throws Exception {
//		return readCSV(CSV.class.getClassLoader().getResourceAsStream("course.csv"));
		return readCSV(new FileInputStream(path));
	}
	
	
	/**
	 * Read CSV file, convert it to Contact.
	 * 
	 * @param stream Input file stream.
	 * @return ArrayList of Contact, if get nothing or errors occured return null
	 * @throws Exception
	 */
	public static ArrayList readCSV(InputStream stream) throws Exception{
		int[] index = new int[4];
		ArrayList<String[]> modr = new ArrayList<String[]>();
		ArrayList<String[]> result = new ArrayList<String[]>();
		CSVReader cr = new CSVReader(new InputStreamReader(stream,"GBK"),',','\"',true,false);
		while (true) {
			try {
				modr.add(cr.getAllFieldsInLine());
			} catch (EOFException e) {
				cr.close();
				break;
			} catch (IOException e) {
				cr.close();
				return null;
			}
		}
		
		String[] tile = modr.get(0);
		for (int i = 0; i < tile.length; i++) {
			if ("系".equalsIgnoreCase(tile[i])) {
				index[0] = i;
			}
			if ("课号".equalsIgnoreCase(tile[i])) {
				index[1] = i;
			}
			if ("课程".equalsIgnoreCase(tile[i])) {
				index[2] = i;
			}
			if ("老师".equalsIgnoreCase(tile[i])) {
				index[3] = i;
			}
		}
		
		for (int i = 1; i < modr.size(); i++) {
			String[] buf = modr.get(i);
			String[] puf = new String[4];
			for (int j = 0; j < 4; j++) {
				puf[j] = buf[index[j]];
			}
			result.add(puf);
		}
		
		return result;
	}
}