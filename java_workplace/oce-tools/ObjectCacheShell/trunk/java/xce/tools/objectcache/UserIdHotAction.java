package xce.tools.objectcache;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;

import com.xiaonei.xce.XceAdapter;

public class UserIdHotAction extends Producer {

	protected UserIdHotAction(BlockingQueue<Long[]> queue, int batchSize, int limit) {
		super(queue);
		this.batchSize = batchSize;
		this.limit = limit;
	}

	private final int batchSize;
	private String minTimeStr = "2099-12-03 22:31:50";
	private int minTimeInt = Integer.MAX_VALUE;
	private int limit = -1;

	@Override
	protected boolean produce(List<Long> emptyData) {
		Connection conn = null;
		ResultSet rs = null;
		try {
			conn = XceAdapter.getInstance().getReadConnection(
					"user_time_new");
			PreparedStatement stmt = conn
					.prepareStatement( "SELECT id, lastlogintime, UNIX_TIMESTAMP(lastlogintime) as llt"
						+ " FROM user_time"
						+ " WHERE lastlogintime < \"" + minTimeStr + "\""
						+ " ORDER BY lastlogintime DESC"
						+ " LIMIT " + batchSize );
			rs = stmt.executeQuery();
			while (rs.next()) {
				if( --limit < 0 ){
					break;
				}
				Integer id = rs.getInt("id");
				String lastlogintime = rs.getString("lastlogintime");
				Integer llt = rs.getInt("llt");
				if( llt < minTimeInt ){
					minTimeStr = lastlogintime;
					minTimeInt = llt;
				}
				emptyData.add((long)id);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (conn != null) {
				try {
					conn.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		System.out.println("Produced " + emptyData.size() + " userIds, start at " + minTimeStr );
		return !emptyData.isEmpty();
	}

}
