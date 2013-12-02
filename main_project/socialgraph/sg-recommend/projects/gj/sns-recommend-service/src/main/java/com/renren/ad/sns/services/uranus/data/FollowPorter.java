package com.renren.ad.sns.services.uranus.data;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Scanner;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.jdbc.core.BatchPreparedStatementSetter;
import org.springframework.jdbc.core.JdbcTemplate;

import com.renren.ad.sns.util.db.PoolingDataSourceFactory;

public class FollowPorter {
	private static final Log LOG = LogFactory.getLog(FollowPorter.class);
	private JdbcTemplate jdbcTemplate;

	public FollowPorter(JdbcTemplate jdbcTemplate) {
		super();
		this.jdbcTemplate = jdbcTemplate;
	}

	public void init(String file) throws FileNotFoundException {
		Scanner cin = new Scanner(new FileInputStream(file));
		int cnt = 0 ;
		long t = System.currentTimeMillis();
		while (cin.hasNext()) {
			String line = cin.nextLine();
			String[] tmps = line.split(" ");
			if( tmps.length > 1 ) {
				long[] users = new long[tmps.length];
				for (int i = 0; i < tmps.length; i++) {
					users[i] = Long.parseLong(tmps[i]);
				}
				update(users);
			}
			if( ++ cnt % 10000 == 0) {
				long t2 = System.currentTimeMillis();
				LOG.info(cnt + " elapse " + ( t2-t) + " ms");
				t = t2;
			}
		}
		cin.close();
	}

	private void update(final long[] users) {
		String sql = "update u2u_actions set is_following = 1 where user_id = ? and owner_id = ?";
		BatchPreparedStatementSetter bss = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return users.length;
			}
			public void setValues(PreparedStatement ps, int i)
					throws SQLException {
				ps.setLong(1, users[0]);
				ps.setLong(2, users[i]);
			}
		};
		String sql2 = "update u2u_actions set is_follower = 1 where user_id = ? and owner_id = ?";
		BatchPreparedStatementSetter bss2 = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return users.length;
			}
			public void setValues(PreparedStatement ps, int i)
					throws SQLException {
				ps.setLong(1, users[i]);
				ps.setLong(2, users[0]);
			}
		};
		long t1 = System.currentTimeMillis();
		jdbcTemplate.batchUpdate(sql, bss);
		jdbcTemplate.batchUpdate(sql2, bss2);
		long t = System.currentTimeMillis() - t1;
		LOG.info("elapse " + t + " ms");
	}

	public static void main(String[] args) {
		DataSource dataSource = new PoolingDataSourceFactory()
				.createDataSource("com.mysql.jdbc.Driver",
						"jdbc:mysql://db.jie.d.xiaonei.com:3306/uranus?user=gj&password=mysql@gj");
		JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
		FollowPorter porter = new FollowPorter(jdbcTemplate);
		try {
			long t1 = System.currentTimeMillis();
			porter.init("/data/uranus/data/follow.dat");
			long t = System.currentTimeMillis() - t1;
			LOG.info("over , elapse " + t + " ms!");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
	}
}
