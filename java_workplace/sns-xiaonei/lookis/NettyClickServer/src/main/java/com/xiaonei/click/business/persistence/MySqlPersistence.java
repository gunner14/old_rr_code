/**
 * 
 */
package com.xiaonei.click.business.persistence;

import java.io.Serializable;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

import com.xiaonei.click.constants.Constants;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class MySqlPersistence implements Persistence {

    private static MySqlPersistence _instance = null;

    private static BlockingQueue<Connection> lbq = new LinkedBlockingQueue<Connection>(
            Constants.CONNECTION_SIZE);

    private MySqlPersistence() {

        try {
            Class.forName("com.mysql.jdbc.Driver");
        } catch (ClassNotFoundException e1) {
            throw new RuntimeException("Can't connect to data base", e1);
        }
        String url = "jdbc:mysql://10.3.19.59:3306/click_stat";
        String username = "log4js";
        String password = "nx$$%xdf.";
        for (int i = 0; i < Constants.CONNECTION_SIZE; i++) {

            Connection con = null;
            try {
                con = DriverManager.getConnection(url, username, password);
                con.setAutoCommit(true);
                lbq.add(con);
            } catch (Exception e) {
                throw new RuntimeException("Can't connect to data base", e);
            }
        }

    }

    public static MySqlPersistence getInstance() {
        if (_instance == null) {
            synchronized (MySqlPersistence.class) {
                if (_instance == null) {
                    _instance = new MySqlPersistence();
                }
            }
        }

        return _instance;
    }

    private static final String SELECT = "select value from click_kv where k=? limit 1";

    @Override
    public Object get(Object key) {
        PreparedStatement ps = null;
        Connection conn = null;
        try {
            conn = lbq.poll(1, TimeUnit.SECONDS);
        } catch (InterruptedException e1) {
            e1.printStackTrace();
            return null;
        }

        try {

            ps = conn.prepareStatement(SELECT);
            ps.setLong(1, Long.parseLong(key.toString()));
            return ps.executeQuery().getObject(1);
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        } finally {
            if (conn != null) lbq.add(conn);
            if (ps != null) {
                try {
                    ps.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static final String INSERT = "insert into click_kv(v) values(?)";

    @Override
    public void set(Serializable o) {
        PreparedStatement ps = null;
        Connection conn = null;
        try {
            conn = lbq.poll(1, TimeUnit.SECONDS);
        } catch (InterruptedException e1) {
            e1.printStackTrace();
            return;
        }

        try {
            ps = conn.prepareStatement(INSERT);
            ps.setString(1, o.toString());
            ps.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (conn != null) lbq.add(conn);
            if (ps != null) {
                try {
                    ps.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
