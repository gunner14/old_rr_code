package com.renren.ad.sns.services.uranus.data;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;

import com.renren.ad.sns.util.db.PoolingDataSourceFactory;

/**
 * a data exporter for u2u info from mysql
 * 
 * @author socialgraph
 */
public class U2uDataExporter {
    private static final Log LOG = LogFactory.getLog(U2uDataExporter.class);

    private NamedParameterJdbcTemplate namedParameterJdbcTemplate;

    public U2uDataExporter(NamedParameterJdbcTemplate namedParameterJdbcTemplate) {
        super();
        this.namedParameterJdbcTemplate = namedParameterJdbcTemplate;
    }

    public void load(String infile, String outfile) throws IOException {
		FileOutputStream cout = new FileOutputStream(outfile);
		Scanner cin = new Scanner(new FileInputStream(infile));
		long t_beg = System.currentTimeMillis();
		long[] users = new long[5000];
		int idx = 0;
		while (cin.hasNext()) {
			users[idx++] = cin.nextLong();
			if (idx == users.length) {
				dbLoader(users, cout);
				idx = 0;
				long t_t = System.currentTimeMillis() - t_beg;
				LOG.info(t_t + " ms elapsed!");
			}
		}
		if (idx > 0) {
			dbLoader(users, cout);
		}
		cin.close();
		cout.close();
	}

    private void dbLoader(long[] users, final FileOutputStream cout) {
        String sql = "select user_id , owner_id , visit_count,like_count,"
                + "click_count,comment_count,is_following,is_follower "
                + " from u2u_actions where user_id in (:fields)";
        List<Long> us = new ArrayList<Long>();
        for( long user : users){
            if( user != 0 ) us.add(user);
        }
        Map<String, List<Long>> params = (Map<String, List<Long>>) Collections.singletonMap("fields", us);  
        this.namedParameterJdbcTemplate.query(sql, params, 
            new RowCallbackHandler() {
            @Override
            public void processRow(ResultSet rs) throws SQLException {
                StringBuffer sb = new StringBuffer();
                for (int i = 1; i <= 8; i++) {
                    sb.append(rs.getInt(i));
                    if (i != 8) {
                        sb.append(",");
                    } else {
                        sb.append("\n");
                    }
                }
                try {
                    cout.write(sb.toString().getBytes());
                } catch (IOException e) {
                    LOG.error("write [" + sb.toString() + " ] to file error!",
                            e);
                }
            }
        });
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.exit(1);
        }
        DataSource dataSource = new PoolingDataSourceFactory()
                .createDataSource("com.mysql.jdbc.Driver",
                        "jdbc:mysql://db.jie.d.xiaonei.com:3306/uranus?user=gj&password=mysql@gj");
        NamedParameterJdbcTemplate namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(dataSource);
        U2uDataExporter exporter = new U2uDataExporter(namedParameterJdbcTemplate);
        try {
            long t1 = System.currentTimeMillis();
            LOG.info(args[0] + " ==========> " + args[1]);
            exporter.load(args[0], args[1]);
            long t = System.currentTimeMillis() - t1;
            LOG.info("over , elapse " + t + " ms!");
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
