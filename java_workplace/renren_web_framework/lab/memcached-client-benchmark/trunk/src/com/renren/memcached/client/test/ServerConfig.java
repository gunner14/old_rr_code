package com.renren.memcached.client.test;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ServerConfig {

	private static ServerConfig instance = new ServerConfig();

	public static ServerConfig getInstance() {
		return instance;
	}

	private List<Server> servers;

	private ServerConfig() {
		loadConfig();
	}

	private void loadConfig() {

		BufferedReader reader = null;
		List<Server> servers = new ArrayList<Server>();
		try {
			reader = new BufferedReader(new InputStreamReader(this
					.getClass().getClassLoader()
					.getResourceAsStream("servers.conf")));
			String line;
			while ((line = reader.readLine()) != null) {
				servers.add(new Server(line));
			}
			this.servers = Collections.unmodifiableList(servers);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public List<Server> getServers() {
		return this.servers;
	}
	
	public String[] getServersArray() {
		String[] ret = new String[servers.size()];
		int offset = 0;
		for (Server s : servers) {
			ret[offset++] = s.getHost() + ":" + s.getPort();
		}
		return ret;
	}
	
	public static void main(String[] args) {
		List<Server> ss = ServerConfig.getInstance().getServers();
		System.out.println(ss.size());
	}
	
}
