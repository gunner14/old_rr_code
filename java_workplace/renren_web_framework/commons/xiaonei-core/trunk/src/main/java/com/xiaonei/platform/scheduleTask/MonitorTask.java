package com.xiaonei.platform.scheduleTask;

import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Random;
import java.util.StringTokenizer;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MonitorTask {

	private Domain domain;

	private final static int POOL_SIZE = 3;

	private Random rand = new Random();

	public MonitorTask(Domain domain) {
		System.out.println("Monitor Lauching .....................................");
		this.domain = domain;
		this.run();
	}

	@SuppressWarnings({ "unchecked", "unchecked" })
	public void run() {
		List<Node> nodes = domain.getLoadNodes();
		CompletionService<String> serv = new ExecutorCompletionService<String>(
				Executors.newFixedThreadPool(Runtime.getRuntime()
						.availableProcessors()
						* POOL_SIZE));
		for (final Node onenode : nodes) {
			serv.submit(new Callable() {
				public String call() throws Exception {
					getNodeState(onenode);
					return "OK!!!!!!!!!!!!";
				}
			});
		}
		try {
			for (int t = 0, n = nodes.size(); t < n; t++) {
				Future<String> f = serv.take();
				String result = f.get();
			}
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
		} catch (ExecutionException e) {
			Thread.currentThread().interrupt();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/*
	 * 根据node对象，生成获取上传服务器的状态信息的url地址
	 */

	private String getNodeStateUrl(Node node) {
		return "http://" + node.getDomain() + "/report.html?rand="
				+ rand.nextInt(100);
	}

	public void getNodeState(Node node) {
		String result = "";
		HttpclientUtil httputil;
		try {
			httputil = new HttpclientUtil(20000);
			result = httputil.getBodyToString(getNodeStateUrl(node), "gbk",
					new HashMap(), "get");
			saveNodeState(node, result);
		} catch (Exception e) {
			//http出问题了，将该节点置为临时只读
			try
			{
				saveNodeState(node, "");
			}catch(Exception e1){e1.printStackTrace();}
			
			e.printStackTrace();
		} finally {
			httputil = null;
		}
	}

	/*
	 * 分析当前上传服务器的状态 更新保存服务器的状态信息 Author@
	 */

	public void saveNodeState(Node node, String state) throws Exception {
		if (!state.equals("")) {
			try {
				StringTokenizer st = new StringTokenizer(state, "|");
				while (st.hasMoreTokens()) {
					StringTokenizer st1 = new StringTokenizer(st.nextToken(),
							"=");
					String key = st1.nextToken();
					String value = st1.nextToken();
					if (key.equals(Constants.STATE_SPACE.toString())) {
						node.setSpaceUsed(Double.valueOf(value));
					} else if (key.equals(Constants.STATE_LOAD.toString())) {
						node.setCpuLoad(Double.valueOf(value));
					} else if (key.equals(Constants.STATE_TIME.toString())) {// 生成的时间有换行符，做了一个替换操作
						value = value.replaceAll("\n", "");
						node.setTime(new Date(Long.valueOf(value) * 1000));
					}
				}
				Log4jConfiure.logger(this.getClass(), "Normal node:" + node);
			} catch (Exception e) {
				System.err.println("#### Storage: node[" + node.getName()
						+ "] parse error -> " + e);
				throw e;
			}
		} else
		{
			Log4jConfiure.logger(this.getClass(), "Http Get Error:" + node.getDomain());
			node.setStatus(Constants.NODE_STATUS_READABLE);// temp read
		}

		domain.saveNodeState(node);
	}
}
