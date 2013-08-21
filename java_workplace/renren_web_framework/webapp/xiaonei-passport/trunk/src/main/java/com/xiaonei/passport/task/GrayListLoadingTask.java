package com.xiaonei.passport.task;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Properties;
import java.util.Map.Entry;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.passport.constants.Globals;


public class GrayListLoadingTask {

	private static final HashMap<String, Boolean> WARNING_USERNAME = Globals.WARNING_USERNAME;

	public static final Runnable loadGrayList = new Runnable() {

		@Override
		public void run() {
			System.out.println("LoadGrayList");
			InputStream in = null;
			try {
				in = GrayListLoadingTask.class.getClassLoader()
						.getResourceAsStream("gray-name-list.properties");
				Properties p = new Properties();
				p.load(in);
				for (Entry<Object, Object> entry : p.entrySet()) {
					String ul = StringUtils.removeStart(entry.getKey()
							.toString(), "gray.names.list.")
							+ "\t";
					if (null != entry.getValue()
							&& !"".equals(entry.getValue().toString())) {
						ul += entry.getValue().toString();
					}
					WARNING_USERNAME.put(ul, true);
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					if (null != in) {
						in.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

			System.out.println("LoadGrayEnd:" + WARNING_USERNAME.size());
		}
	};

	static {
		new Thread(loadGrayList).start();
	}

	public static void main(String[] args) throws IOException {
		// InputStream in =
		// GrayListLoadingTask.class.getClassLoader().getResourceAsStream(
		// "gray-name-list.properties");
		// Properties p = new Properties();
		// p.load(in);
		// System.out.println(p.get("gray.names.list.00wait00@163.com"));
		// System.out.println(p.get("gray.names.list.10210329"));
		// System.out.println(p.get("gray.names.list.0sky890111"));
		System.out.println("aaa");
	}

}
