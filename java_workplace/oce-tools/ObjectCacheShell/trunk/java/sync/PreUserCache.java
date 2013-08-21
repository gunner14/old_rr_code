import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import xce.usercache.UserCacheManagerPrx;
import xce.usercache.UserCacheManagerPrxHelper;
import xce.util.channel.Channel;

import Ice.ObjectPrx;

import com.xiaonei.xce.XceAdapter;

public class PreUserCache {
	private static Timer _timer = new Timer(true);
	private static Channel channel=Channel.newCoreChannel("oce");
	private static IceGrid.QueryPrx query = IceGrid.QueryPrxHelper.uncheckedCast(channel.getCommunicator().stringToProxy("CoreCache/Query"));
	private static Map<ObjectPrx, List<ObjectPrx>> replicatedProxy = new ConcurrentHashMap<ObjectPrx, List<ObjectPrx>>();
	private static List<ObjectPrx> locateAllProxy(Ice.ObjectPrx proxy) {
		if (!replicatedProxy.containsKey(proxy)) {
			replicatedProxy.put(proxy, getReplicatedProxy(proxy));
		}
		return replicatedProxy.get(proxy);
	}
	private static List<ObjectPrx> getReplicatedProxy(
			Ice.ObjectPrx proxy) {
		List<Ice.ObjectPrx> result = new ArrayList<ObjectPrx>();
		Ice.ObjectPrx[] replica = query.findAllReplicas(proxy);
		for (Ice.ObjectPrx one : replica) {
			result.add(one);
		}
		return result;
	}
	private static class ValidateTask extends TimerTask {
		@Override
		public void run() {
			try {
				for (Entry<ObjectPrx, List<ObjectPrx>> entry : replicatedProxy
						.entrySet()) {
					ObjectPrx key = entry.getKey();
					entry.setValue(getReplicatedProxy(key));
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	public static void main(String[] args) {
		ObjectPrx[] objects=new ObjectPrx[2];		
		objects[0]=channel.getCommunicator().stringToProxy("M@UserCache0");
		objects[1]=channel.getCommunicator().stringToProxy("M@UserCache1");
		_timer.schedule(new ValidateTask(), 10000, 10000);
		try {			
			while (true) {
				try {
					BufferedReader br = new BufferedReader(new FileReader(args[0]));
					Long id = Long.parseLong(br.readLine());
					br.close();
					long max=0;
					Connection c = XceAdapter.getInstance().getReadConnection(
							"user_buddy_log");
					Statement s = c.createStatement();
					String sql = "select id,userid from user_cache_log where id>" + id
							+ " limit 10000";
					ResultSet rs = s.executeQuery(sql);
					Map<Integer,List<Integer>> map=new HashMap<Integer,List<Integer>>();
					map.put(new Integer(0), new ArrayList<Integer>());
					map.put(new Integer(1), new ArrayList<Integer>());
					while (rs.next()) {
						long tid=rs.getLong("id");
						int userid=rs.getInt("userid");
						max = Math.max(tid, max);
						map.get(new Integer(userid%2)).add(new Integer(userid));
					}
					rs.close();
					s.close();
					c.close();
					if(max==0){
						Thread.sleep(10000);
						continue;
					}
					System.out.println("Load UserCache from "+id+" to "+max);
					System.out.println("map 0 size: "+map.get(new Integer(0)).size());
					System.out.println("map 1 size: "+map.get(new Integer(1)).size());
					List<ObjectPrx> proxies=locateAllProxy(objects[0]);
					System.out.println("Size of proxies for 0: "+proxies.size());
					for(ObjectPrx p:proxies){
						UserCacheManagerPrx ocp=UserCacheManagerPrxHelper.uncheckedCast(p);
						int[] ls=new int[map.get(new Integer(0)).size()];
						for(int i=0;i<ls.length;i++){
							ls[i]=map.get(new Integer(0)).get(i);
						}
						ocp.load(ls);
					}
					proxies=locateAllProxy(objects[1]);
					System.out.println("Size of proxies for 1: "+proxies.size());
					for(ObjectPrx p:proxies){
						UserCacheManagerPrx ocp=UserCacheManagerPrxHelper.uncheckedCast(p);
						int[] ls=new int[map.get(new Integer(1)).size()];
						for(int i=0;i<ls.length;i++){
							ls[i]=map.get(new Integer(1)).get(i);
						}
						ocp.load(ls);
					}
					
					BufferedWriter bw = new BufferedWriter(new FileWriter(
							args[0]));
					bw.write(String.valueOf(max));
					bw.flush();
					bw.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

