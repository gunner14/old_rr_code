package com.renren.renrengame.utils.spread;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.renren.renrengame.utils.RRGameConstants;

public class SpreadFileAnalysis {

	private String getStringFromFile(String filePath) {
		URL resourceURL = this.getClass().getClassLoader()
				.getResource(filePath);
		String content = "";
		if (resourceURL == null) {
			return content;
		}
		String path = resourceURL.getPath();
		try {
			InputStream in = new FileInputStream(path);
			byte[] b = new byte[1024];
			int len = -1;
			while((len = in.read(b)) != -1) {
				content += new String(b, 0 , len);
			}
			in.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return content;
	}
	
	private List<Integer> getIDList() {
		String content = getStringFromFile(RRGameConstants.spreadFile);
		String str[] = content.split("\n");
		List<Integer> arr = new ArrayList<Integer>();
	    for(int i = 0; i < str.length; ++i) {
	    	try {
	    	    arr.add(Integer.parseInt(str[i].trim()));
	    	} catch(Exception e) {
	    		continue;
	    	}
	    }
		return arr;
	}
	
   public List<Integer> get9From20() {
	   List<Integer> list = getIDList();
	   Collections.shuffle(list);
	   int end = list.size();
	   if (9< end) {
		   end = 9;
	   }
	   return list.subList(0, end);
   }
  
}
