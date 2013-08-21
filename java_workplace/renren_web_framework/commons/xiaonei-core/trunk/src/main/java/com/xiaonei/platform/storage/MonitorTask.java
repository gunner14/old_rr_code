package com.xiaonei.platform.storage;

import java.util.ArrayList;
import java.util.Date;
import java.util.Random;
import java.util.StringTokenizer;
import java.util.TimerTask;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;

public class MonitorTask extends TimerTask {
	private Domain domain;

	private Random rand = new Random();

	public MonitorTask(Domain domain) {
		this.domain = domain;
	}

	public void run() {
		try {
			System.err.println("#### Storage:MonitorTask check");
			long start = System.currentTimeMillis() ;
			Object[] groups = domain.getAllGroups();
			for (int i = 0; i < groups.length; ++i) {
				Group group = (Group) groups[i];
				 System.err.println("#### Storage:MonitorTask trapping group[" +
				 group.getName() + "]");
				ArrayList<Node> nodes = group.getNodes();
				for (int j = 0; j < nodes.size(); ++j) {
					Node node = nodes.get(j);
					if (node.getStatus() <= Constants.NODE_STATUS_DISABLE) {
						continue;
					}
					// System.err.println("#### Storage: trapping node[" +
					// node.getName() + "]");
					rsyncNodeState(node);
				}
			}
			domain.rankWritableGroups();
			System.err.println("### Storage MonitorTask .usetime :"+(System.currentTimeMillis()-start)) ;
		} catch (Exception e) {
			e.printStackTrace();
		}
		try {
			Thread.sleep(5 * 1000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void rsyncNodeState(Node node) {
		
		for (int count = 0; count < 3; ++count) {
			String state="" ;
			HttpClient httpClient = new HttpClient();
			
			GetMethod method = new GetMethod(getNodeStateUrl(node));
			method.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
					new DefaultHttpMethodRetryHandler());
			method.getParams().setSoTimeout(1000*5) ;
			try {
				if (httpClient.executeMethod(method) != HttpStatus.SC_OK) {
					continue;
				}
				
				state = new String(method.getResponseBody()).trim();
				if (state != null) {
					saveNodeState(node, state);
					return;
				}
			} catch (Exception e) {
				// do nothing
				e.printStackTrace() ;
			} finally {
				
				method.releaseConnection();
			}
			System.err.println("#### Storage:MonitorTask node[" + node.getName()
					+ "] inactive.count["+count+"].state["+state+"] !! ");
			try {
				Thread.sleep(10*1000);
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
		}
		
		node.setTime(new Date(0));
	}

	private String getNodeStateUrl(Node node) {
		// return "http://" + node.getDomain() + "/report.html?rand="
		// + rand.nextInt(100);
		return "http://" + node.getDomain() + "/report.html?rand="
				+ rand.nextInt(100);
	}

	private void saveNodeState(Node node, String state) throws Exception {
		try {
			StringTokenizer st = new StringTokenizer(state, "|");
			while (st.hasMoreTokens()) {
				StringTokenizer st1 = new StringTokenizer(st.nextToken(), "=");
				String key = st1.nextToken();
				String value = st1.nextToken();
				if (key.equals(Constants.STATE_SPACE.toString())) {
					node.setSpaceUsed(Double.valueOf(value));
				} else if (key.equals(Constants.STATE_LOAD.toString())) {
					node.setCpuLoad(Double.valueOf(value));
				} else if (key.equals(Constants.STATE_TIME.toString())) {
					node.setTime(new Date(Long.valueOf(value) * 1000));
				}
			}
		} catch (Exception e) {
			System.err.println("#### Storage: node[" + node.getName()
					+ "] parse error -> " + e);
			throw e;
		}
		//if (!node.isActive()) {
			System.err.println("#### Storage:MonitorTask node[" + node.getName()
					+ "]-> " + node);
		//}
		/*domain.saveNodeState(node);*/
	}
}
