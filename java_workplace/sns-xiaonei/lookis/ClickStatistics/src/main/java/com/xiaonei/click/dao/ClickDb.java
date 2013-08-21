/**
 * 
 */
package com.xiaonei.click.dao;

import java.net.MalformedURLException;
import java.net.URL;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.click.module.Click;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class ClickDb {

    private static volatile ClickDb _instance = null;

    Connection con = null;

    private ClickDb() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            String url = "jdbc:mysql://127.0.0.1:3306/click_stat";
            String username = "log4js";
            String password = "nx$$%xdf.";
            con = DriverManager.getConnection(url, username, password);
            con.setAutoCommit(true);
        } catch (Exception e) {
            throw new RuntimeException("Can't connect to data base", e);
        }
    }

    public static ClickDb getInstance() {
        if (_instance == null) {
            synchronized (ClickDb.class) {
                if (_instance == null) {
                    _instance = new ClickDb();
                }
            }
        }
        return _instance;
    }

    public List<Click> getClick(int id, String url) throws SQLException, MalformedURLException {
        ResultSet rs = null;
        PreparedStatement ps = null;
        try {
            String sql = "select * from click where user_id=? and r_domain=? and r_uri=?";
            ps = con.prepareStatement(sql);
            ps.setInt(1, id);
            URL u = new URL(url);
            ps.setString(1, u.getHost());
            ps.setString(2, u.getPath());
            rs = ps.executeQuery();
            List<Click> clickList = new ArrayList<Click>();
            while (rs.next()) {
                Click c = parseClick(rs);
                if (c != null) clickList.add(c);
            }
            return clickList;
        } finally {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
        }
    }

    public List<Click> getClick(String url) throws SQLException, MalformedURLException {
        ResultSet rs = null;
        PreparedStatement ps = null;
        try {
            String sql = "select * from click where r_domain=? and r_uri=? order by id desc limit 1000";
            ps = con.prepareStatement(sql);
            URL u = new URL(url);
            ps.setString(1, u.getHost());
            ps.setString(2, u.getPath());
            rs = ps.executeQuery();
            List<Click> clickList = new ArrayList<Click>();
            while (rs.next()) {
                Click c = parseClick(rs);
                if (c != null) clickList.add(c);
            }
            return clickList;
        } finally {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
        }
    }

    private Click parseClick(ResultSet rs) throws SQLException {
        Click c = new Click();
        c.setTime(rs.getLong("time"));
        c.setUrl("http://" + rs.getString("r_domain") + rs.getString("r_uri")
                + rs.getString("r_querystring"));
        c.setUserId(rs.getInt("user_id"));
        c.setX(rs.getInt("x"));
        c.setY(rs.getInt("y"));
        return c;
    }
}
