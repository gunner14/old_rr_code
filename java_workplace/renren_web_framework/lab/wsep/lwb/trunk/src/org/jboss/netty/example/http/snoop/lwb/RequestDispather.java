package org.jboss.netty.example.http.snoop.lwb;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;

public class RequestDispather {

	private static RequestDispather instance = new RequestDispather();

	public static RequestDispather getInstance() {
		return instance;
	}
	
	private Map<String, IReqHandler> handlers = new HashMap<String, IReqHandler>();
	
	private RequestDispather() {
		loadConfig();
	}

	private void loadConfig() {
		
		InputStream is = null;
		try {
			is = RequestDispather.class.getClassLoader().getResourceAsStream("dispather.conf");
			BufferedReader in = new BufferedReader(new InputStreamReader(is));
			String line;
			while ((line = in.readLine()) != null) {
				parseLine(line);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	private void parseLine(String line) {

		String[] ss = line.split(" ");
		String path = ss[0];
		String klassName = ss[1];
		try {
			Class<?> klass = Class.forName(klassName);
			Constructor<?> con = klass.getConstructor();
			Object obj = con.newInstance();
			handlers.put(path, (IReqHandler)obj);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public IReqHandler dispatch(String path) {
		return handlers.get(path);
	}
	
	public static void main(String[] args) {
		RequestDispather.getInstance();
	}
	
}
