package xce.tools.objectcache;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.io.*;

import xce.util.channel.Channel;
import MyUtil.ObjectCachePrx;
import MyUtil.ObjectCachePrxHelper;

import com.xiaonei.xce.XceAdapter;

public class UserIdDescAction {

	public static void main(String[] args) {
		try {
			String endpoints = "SC@BuddyApplyCache";
			Ice.Communicator ic = Channel.newOceChannel(endpoints).getCommunicator();
			String pathIds = "/data/xce/ObjectCacheShell/xce/tools/objectcache/active_ids.txt";
			InputStream isIds = new FileInputStream(pathIds);
			BufferedReader readerIds = new BufferedReader(new InputStreamReader(isIds));
			String line = "";
			while ( line != null ) {				
				line = readerIds.readLine();
				if ( line != null ) {
					System.out.println(line);
					long id = Long.valueOf(line);
					ObjectCachePrx prx = ObjectCachePrxHelper.uncheckedCast(
						ic.stringToProxy("SC@BuddyApplyCache" + String.valueOf(id%10)));
					prx.removeObject(0, id);
					prx.locateObject(0, id);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} 	
		System.out.println("Locate userIds complete");		
	}

}
