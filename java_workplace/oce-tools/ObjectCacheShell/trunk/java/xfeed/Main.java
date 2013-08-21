import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.FileReader;
import java.util.Vector;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;
import xce.xfeed.XFeedDispatcherPrx;
import xce.xfeed.XFeedDispatcherPrxHelper;
import com.xiaonei.xce.XceAdapter;

public class Main {
	public static void main(String[] args) {
		long minfeedId = 0;
		long maxfeedId = 0;
		int dispatchStep = 1000;
		int dispatchPeriod = 50;
		String filename = "min-dispatch-again-id";
		for (int i = 0; i < args.length; ++i) {
			if (i == 2) {
				minfeedId = Long.parseLong(args[i]);
			} else if (i == 3) {
				maxfeedId = Long.parseLong(args[i]);
			} else if (i == 0) {
				dispatchStep = Integer.parseInt(args[i]);
			} else if (i == 1) {
				dispatchPeriod = Integer.parseInt(args[i]);
			}
		}
		if (minfeedId == 0) {
			try {
				BufferedReader br = new BufferedReader(new FileReader(filename));
				minfeedId = Long.parseLong(br.readLine());
				br.close();
			} catch (Exception e) {
				e.printStackTrace();
				System.out.println("argument required: minfeedId [maxfeedId] [dispatchStep] [dispatchPeriod]");
				System.exit(0);
			}
		}
		if (maxfeedId == 0) {
			try {
				Connection c = XceAdapter.getInstance().getReadConnection(
						"xfeed_content");
				Statement s = c.createStatement();
				String sql = "select  max(id) as id from xfeed_content";
				ResultSet rs = s.executeQuery(sql);
				while (rs.next()) {
					maxfeedId = rs.getLong("id");
				}
				rs.close();
				s.close();
				c.close();
			} catch (Exception e) {
				e.printStackTrace();
				System.out.println("argument required: minfeedId [maxfeedId] [dispatchStep] [dispatchPeriod]");
				System.exit(0);
			}
		}
		System.out.println("####");
		System.out.println("#### dispatch again: from " + minfeedId + " to " + maxfeedId + " with " + dispatchStep + " count every " + dispatchPeriod + " ms");
		System.out.println("#### total: " + (maxfeedId - minfeedId) + "\n");
		for (long xfeedId = minfeedId; xfeedId < maxfeedId; xfeedId += (dispatchStep + 1)) {
			XFeedDispatcherAgainAdapter.getInstance().dispatchAgain(xfeedId, xfeedId+dispatchStep+1);
			System.out.println("dispatch xfeed again from " + xfeedId + " to " + (xfeedId + dispatchStep));
			try {
				BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
				bw.write(String.valueOf(xfeedId));
				bw.flush();
				bw.close();
				Thread.sleep(dispatchPeriod);
			} catch (Exception e) {
				e.printStackTrace();
			}				
		}
//		try {
//			BufferedReader br = new BufferedReader(new FileReader(args[0]));
//			maxfeedId = Long.parseLong(br.readLine());
//			br.close();
//		} catch (Exception e) {
//			e.printStackTrace();
//			System.exit(0);
//		}
//		while (true) {
//			long minfeedId = maxfeedId - 101;
//			try {
//				Connection c = XceAdapter.getInstance().getReadConnection(
//						"xfeed_content");
//				Statement s = c.createStatement();
//				String sql = "select if (max(id) > " + minfeedId + ", " + minfeedId
//						+ ", max(id)) as id from xfeed_content";
//				ResultSet rs = s.executeQuery(sql);
//				while (rs.next()) {
//					minfeedId = rs.getLong("id");
//				}
//				rs.close();
//				s.close();
//				c.close();
//				if (minfeedId > maxfeedId) {
//					System.out.println("Dispatcher again from " + maxfeedId + " to "
//							+ (minfeedId - 1));
//					XFeedDispatcherAgainAdapter.getInstance().dispatchAgain(maxfeedId, minfeedId);
//					maxfeedId = minfeedId;
//					try {
//						BufferedWriter bw = new BufferedWriter(new FileWriter(args[0]));
//						bw.write(String.valueOf(minfeedId));
//						bw.flush();
//						bw.close();
//					} catch (Exception e) {
//						e.printStackTrace();
//					}		
//					continue;
//				}
//			} catch (Exception e) {
//				e.printStackTrace();
//			}
//			try {
//				Thread.sleep(100);
//			} catch (Exception e) {
//				e.printStackTrace();
//
//			}
//		}
	}	
}

class XFeedDispatcherAgainAdapter extends ServiceAdapter {
	private static XFeedDispatcherAgainAdapter instance;
	private static final String endpoints = "@XFeedDispatcherAgain";
	private Vector<XFeedDispatcherPrx> dispatchersOneway = new Vector<XFeedDispatcherPrx>();
	private Vector<XFeedDispatcherPrx> dispatchers = new Vector<XFeedDispatcherPrx>();

	/**
	 * Constructs an singleton instance of this class.
	 * 
	 */
	public static XFeedDispatcherAgainAdapter getInstance() {
		if (instance == null) {
			instance = new XFeedDispatcherAgainAdapter();
		}
		return instance;
	}

	private XFeedDispatcherAgainAdapter() {
		super(endpoints, 10, Channel.newFeedChannel(endpoints));
	}
	
	/**
	 * Send xfeedXml and xfeedProp to the background service
	 * 
	 * @param xfeedXml
	 *            the xml data of feed
	 * @param xfeedProp
	 *            the properties for dispatching, it should contain the 
	 *            following keys:<br/> 
	 *            
	 *            actor -> who performs this action.<br/>
	 *            type -> which is the first level type of this action.<br/>
	 *            resource -> which is the unique key for merging.<br/>
	 *            expr -> which is an expression or id that represent feed targets.<br/>
	 *            	example: f(xx)+u(xx)+u(xx)-u(xx)&n(xx)&a(xx)-a(xx)|c(xx).<br/>
	 *            	+ plus rhs to lhs.<br/>
	 *            	- sub the rhs from lhs.<br/>
	 *            	| plus rhs to lhs, like +.<br/>
	 *            	& keep the common targets between lhs and rhs.<br/>
	 *            	f(xx) evaluate all friends of xx.<br/>
	 *            	n(xx) only be used in & operator, evaluate the common  
	 *            		network members with another collection.<br/>
	 *            	a(xx) only be used in & operator, evaluate the common
	 *            		app members with another collection.<br/>
	 *            	c(xx) evaluate all class members of xx.<br/>
	 *            	u(xx) xx only.<br/>            	
	 *            
	 *            weight -> which is the weight of this action when feeding.<br/>
	 *            merge -> which is policy when merging.<br/>
	 *            	-1 remove,<br/>
	 *            	0 none ,<br/>
	 *            	1 replace,<br/>
	 *            	2 append.<br/>
	 *            renew -> renew the feed's time when merging if it is 1.<br/>
	 *            unread -> reset the feed's unread when merging if it is 1.<br/>
	 */
	public void dispatch(String xfeedXml, java.util.Map xfeedProp) {
		try {
			getXFeedDispatcher(
					Integer.parseInt(xfeedProp.get("actor").toString()))
					.dispatch(xfeedXml, xfeedProp);			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Dispatch the original feeds again between startXFeedId and endXFeedId
	 * 
	 * @param startXFeedId
	 *            the start feed id
	 * @param endXFeedId
	 *            the end feed id
	 */
	public void dispatchAgain(long startXFeedId, long endXFeedId) {
		int mod=(int)(startXFeedId%cluster);
		getXFeedDispatcherOneway(mod).dispatchAgain(startXFeedId,
				endXFeedId);
	}

	/**
	 * Increase feed's comment count
	 * 
	 * @param feedId
	 *            the feed id to increase
	 * @param step
	 *            the count to increase on the feed
	 */
	public void incCommentCount(int feedId, int step) {
		getXFeedDispatcherOneway(feedId).incCommentCount(feedId, step);
	}

	/**
	 * Decrease feed's comment count
	 * 
	 * @param feedId
	 *            the feed id to increase
	 * @param step
	 *            the count to increase on the feed
	 */
	public void decCommentCount(int feedId, int step) {
		getXFeedDispatcherOneway(feedId).decCommentCount(feedId, step);
	}

	private XFeedDispatcherPrx getXFeedDispatcherOneway(int userId) {
		return locateProxy(dispatchersOneway, "XFD", userId,
				Channel.Invoke.Oneway, XFeedDispatcherPrxHelper.class, 300);
	}

	private XFeedDispatcherPrx getXFeedDispatcher(int userId) {
		return locateProxy(dispatchers, "XFD", userId, Channel.Invoke.Twoway,
				XFeedDispatcherPrxHelper.class, 300);
	}
}
