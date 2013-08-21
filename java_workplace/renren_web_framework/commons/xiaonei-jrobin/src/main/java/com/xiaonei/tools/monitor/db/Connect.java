package com.xiaonei.tools.monitor.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class Connect {

    private final static String url = "jdbc:mysql://10.3.18.143:3306/webmonitor?autoReconnect=true";

    //        private final static String url = "jdbc:mysql://localhost:3306/xnmonitor?aut oReconnect=true";

    private final static String userName = "root";

    private final static String password = "";

    private static Connection con = null;
    static {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            con = DriverManager.getConnection(url, userName, password);

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }

    }

    private Connect() {

    }

    /**
     * 查询数据
     * 
     * @param sqlStr
     * @return
     * @throws SQLException
     */
    public static ResultSet querySet(String sqlStr) throws SQLException {
        Statement stmt = con.createStatement();
        ResultSet rs = stmt.executeQuery(sqlStr);
        return rs;
    }

    /**
     * 插入数据
     * 
     * @param sqlStr
     * @throws SQLException
     */
    public static void insert(String sqlStr) throws SQLException {
        Statement stmt = con.createStatement();
        stmt.execute(sqlStr);
    }

    public static void main(String... args) throws SQLException {
        System.out.println(con == null);
        System.out.println(con.createStatement() == null);

    }

}
