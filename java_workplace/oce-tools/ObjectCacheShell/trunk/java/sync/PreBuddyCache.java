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

import xce.buddy.BuddyCacheManagerPrx;
import xce.buddy.BuddyCacheManagerPrxHelper;
import xce.util.channel.Channel;

import Ice.ObjectPrx;
import MyUtil.ObjectCachePrx;
import MyUtil.ObjectCachePrxHelper;

import com.xiaonei.xce.XceAdapter;

public class PreBuddyCache {
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
        ObjectPrx[] objects=new ObjectPrx[10];
        objects[0]=channel.getCommunicator().stringToProxy("M@BuddyCache0");
        objects[1]=channel.getCommunicator().stringToProxy("M@BuddyCache1");
        objects[2]=channel.getCommunicator().stringToProxy("M@BuddyCache2");
        objects[3]=channel.getCommunicator().stringToProxy("M@BuddyCache3");
        objects[4]=channel.getCommunicator().stringToProxy("M@BuddyCache4");
        objects[5]=channel.getCommunicator().stringToProxy("M@BuddyCache5");
        objects[6]=channel.getCommunicator().stringToProxy("M@BuddyCache6");
        objects[7]=channel.getCommunicator().stringToProxy("M@BuddyCache7");
        objects[8]=channel.getCommunicator().stringToProxy("M@BuddyCache8");
        objects[9]=channel.getCommunicator().stringToProxy("M@BuddyCache9");

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
                            String sql = "select id,userid from buddy_cache_log where id>" + id
                                            + " limit 10000";
                            ResultSet rs = s.executeQuery(sql);
                            Map<Integer,List<Integer>> map=new HashMap<Integer,List<Integer>>();
                            for(int i=0;i<10;i++){
                                map.put(new Integer(i), new ArrayList<Integer>());
                            }
                            while (rs.next()) {
                                    long tid=rs.getLong("id");
                                    int userid=rs.getInt("userid");
                                    max = Math.max(tid, max);
                                    map.get(new Integer(userid%10)).add(new Integer(userid));
                            }
                            rs.close();
                            s.close();
                            c.close();
                            if(max==0){
                            	Thread.sleep(10000);
                            	continue;
                            }
                            System.out.println("Load BuddyCache from "+id+" to "+max);
                            for(int i=0;i<10;i++){
                                System.out.println("map "+i+" size: "+map.get(new Integer(i)).size());
                            }
                            for(int i=0;i<10;i++){
                                List<ObjectPrx> proxies=locateAllProxy(objects[i]);
                                System.out.println("Size of proxies for "+i+": "+proxies.size());
                                for(ObjectPrx p:proxies){
                                        BuddyCacheManagerPrx ocp = BuddyCacheManagerPrxHelper.uncheckedCast(p);
                                        int[] ls=new int[map.get(new Integer(i)).size()];
                                        for(int j=0;j<ls.length;j++){
                                            ls[j]=map.get(new Integer(i)).get(j);
                                            
                                        }
                                        ocp.load(ls);
                                }
                                    
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

