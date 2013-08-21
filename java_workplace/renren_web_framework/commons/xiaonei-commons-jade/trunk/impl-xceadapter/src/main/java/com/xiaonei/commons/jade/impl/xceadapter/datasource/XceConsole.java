package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.sql.DataSource;

import jline.ConsoleReader;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;

import com.xiaonei.commons.jade.impl.util.JdbcUtils;
import com.xiaonei.commons.jade.impl.xceadapter.configurator.RemoteXmlDocXceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.configurator.XmlDocXceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.connector.TestXceConnector;

/**
 * 实现一个简易的: Xce 连接控制台, 可以通过它选择虚拟数据库执行语句。
 * 
 * @author han.liao
 */
public class XceConsole {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(XceConsole.class);

    // 匹配  WHITESAPCE
    private static final Pattern EMPTY_PATTERN = Pattern.compile("^\\s*$");

    // 匹配命令行
    private static final Pattern LINE_PATTERN = Pattern
            .compile("(\'([^\\\\\']|\\\\\'|\'\')*\'|\"([^\\\\\"]|\\\\\"|\"\")*\"|[^\'\"])*;\\s*$");

    // 匹配  EXIT / QUIT 命令
    private static final Pattern EXIT_PATTERN = Pattern.compile("^(EXIT|QUIT);?$",
            Pattern.CASE_INSENSITIVE);

    // 匹配  USE {catalog} 命令
    private static final Pattern USE_PATTERN = Pattern.compile("^\\s*USE\\s+([^;]+);?\\s*$",
            Pattern.CASE_INSENSITIVE);

    // 匹配  SET {master|slave} 命令
    private static final Pattern SET_PATTERN = Pattern.compile("^\\s*SET\\s+([^;]+);?\\s*$",
            Pattern.CASE_INSENSITIVE);

    // 匹配  ReadOnly {on | off} 命令, 注意该命令是大小写敏感的！
    private static final Pattern READONLY_PATTERN = Pattern.compile( // NL
            "^\\s*ReadOnly\\s+([^;]+);?\\s*$");

    // 匹配  USE {catalog} 命令
    private static final Pattern DEBUG_PATTERN = Pattern.compile("^\\s*DEBUG\\s+([^;]+);?\\s*$",
            Pattern.CASE_INSENSITIVE);

    public static void main(String... args) throws IOException {

        // 防止  Log4j 报错
        BasicConfigurator.configure();

        Logger.getRootLogger().setLevel(Level.INFO);

        // 检查参数
        if ((args.length < 1) || "--help".equalsIgnoreCase(args[0])) {

            System.out.println( // NL
                    "Usage: java -cp * XceConsole [--help] [--xce] [--local file]");
            System.exit(0);

            return;
        }

        // 初始化数据库连接
        XceDataSource ds = new XceDataSource();

        // 默认是只读数据库
        ds.setReadOnly(true);

        if ("--xce".equalsIgnoreCase(args[0])) {

            // 创建配置信息
            RemoteXmlDocXceConfigurator configurator = new RemoteXmlDocXceConfigurator();

            ds.setConfigurator(configurator);

        } else if ("--local".equalsIgnoreCase(args[0])) {

            if (args.length < 2) {

                System.out.println( // NL
                        "ERROR: usage: java -cp * XceConsole --local {*.properties}");
                System.exit(0);

                return;
            }

            Properties props = new Properties();

            // 读取配置文件
            String filePath = args[1];

            FileInputStream fin = null;

            try {
                fin = new FileInputStream(filePath);

                props.load(fin);

            } catch (FileNotFoundException e) {

                if (logger.isDebugEnabled()) {

                    logger.error("ERROR: file not found: " + filePath, e);

                } else {

                    logger.error("ERROR: file not found: " + filePath + " - "
                            + e.getLocalizedMessage());
                }

                System.exit(0);

                return;

            } catch (IOException e) {

                if (logger.isDebugEnabled()) {

                    logger.error("ERROR: io exception: " + filePath, e);

                } else {

                    logger.error("ERROR: io exception: " + filePath + " - "
                            + e.getLocalizedMessage());
                }

                System.exit(0);

                return;

            } finally {

                if (fin != null) {

                    try {
                        fin.close();

                    } catch (IOException e) {

                        if (logger.isDebugEnabled()) {

                            logger.warn("WARNING: can not close tream", e);

                        } else {

                            logger.warn( // NL
                                    "WARNING: can not close stream - " + e.getLocalizedMessage());
                        }
                    }
                }
            }

            // 初始化数据库连接
            TestXceConnector connector = new TestXceConnector();

            connector.setUrl(props.getProperty("jade-jdbc.url"));
            connector.setUsername(props.getProperty("jade-jdbc.username"));
            connector.setPassword(props.getProperty("jade-jdbc.password"));

            ds.setConnector(connector);

            // 创建配置信息
            XmlDocXceConfigurator configurator = new XmlDocXceConfigurator();

            ds.setConfigurator(configurator);

        } else {
            System.out.println( // NL
                    "ERROR: usage: java -cp * XceConsole [--help] [--xce] [--local file]");

            System.exit(0);

            return;
        }

        // 创建控制台输入输出
        PrintWriter cout = new PrintWriter(System.out);

        // 用默认的  Virtual-Key 绑定
        ConsoleReader cin = new ConsoleReader(System.in, cout);

        while (true) {

            cout.print("XceConsole>");

            cout.flush();

            String line = cin.readLine();

            // 检查语句是否结束
            if (line == null) {

                System.out.println( // NL
                        "INFO: console input end, using \'exit\' or \'quit\' instead Ctrl+C.");
                System.exit(0);

                return;
            }

            // 忽略空行
            if (EMPTY_PATTERN.matcher(line).matches()) {

                continue;
            }

            // 检查  EXIT / QUIT 命令
            if (EXIT_PATTERN.matcher(line).matches()) {

                // 退出程序
                System.out.println("INFO: exiting, bye.");
                System.exit(0);

                return;
            }

            // 检查  USE {catalog} 命令
            Matcher matchUse = USE_PATTERN.matcher(line);

            if (matchUse.matches()) {

                String catalog = matchUse.group(1).trim();

                System.out.println("INFO: using catalog \'" + catalog + '\'');

                // 执行数据库切换
                ds.setCatalog(catalog);

                continue;
            }

            // 检查  SET {master|slave} 命令
            Matcher matchSet = SET_PATTERN.matcher(line);

            if (matchSet.matches()) {

                String flag = matchSet.group(1).trim();

                // 匹配  MASTER | SLAVE 命令
                if ("MASTER".equalsIgnoreCase(flag)) {

                    ds.setForceWrite(true);

                } else if ("SLAVE".equalsIgnoreCase(flag)) {

                    ds.setForceWrite(false);
                }

                if (ds.isForceWrite()) {

                    System.out.println("INFO: set \'master\' mode");

                } else {

                    System.out.println("INFO: set \'slave\' mode");
                }

                continue;
            }

            // 检查  ReadOnly {on | off} 命令
            Matcher matchReadOnly = READONLY_PATTERN.matcher(line);

            // 检查  ReadOnly 命令
            if (matchReadOnly.matches()) {

                String state = matchReadOnly.group(1).trim();

                // 匹配  ON | OFF 命令
                if ("ON".equalsIgnoreCase(state)) {

                    ds.setReadOnly(true);

                } else if ("OFF".equalsIgnoreCase(state)) {

                    ds.setReadOnly(false);
                }

                if (ds.isReadOnly()) {

                    System.out.println("INFO: read-only mode is on");

                } else {

                    System.out.println("INFO: read-only mode is off");
                }

                continue;
            }

            // 检查  DEBUG {level} 命令
            Matcher matchDebug = DEBUG_PATTERN.matcher(line);

            if (matchDebug.matches()) {

                Level level = Level.toLevel(matchDebug.group(1).trim(), Level.INFO);

                System.out.println("INFO: debug level \'" + level + '\'');

                // 调整日志输出级别
                Logger.getRootLogger().setLevel(level);

                continue;
            }

            // 接受多行输入
            StringBuilder builder = new StringBuilder();

            builder.append(line);

            while (!LINE_PATTERN.matcher(builder).matches()) {

                line = cin.readLine();

                // 检查语句是否结束
                if (line == null) {

                    System.out.println( // NL
                            "INFO: console input end, using \'exit\' or \'quit\' instead Ctrl+C.");
                    System.exit(0);

                    return;
                }

                builder.append('\n');
                builder.append(line);
            }

            // 执行  SQL 语句
            if ("".equals(ds.getCatalog())) {

                System.out.println("ERROR: type 'use {catalog}' choice a catalog first.");

            } else {

                // 执行  SQL 语句。
                execute(cout, ds, builder.toString());
            }
        }
    }

    // 执行所需的语句
    private static void execute(PrintWriter cout, DataSource ds, String command) {

        Connection conn = null;

        Statement stmt = null;

        try {
            conn = ds.getConnection();

            stmt = conn.createStatement();

            stmt.execute(command);

            System.out.println();

            // 获得连接的数据库信息
            DatabaseMetaData dbmd = conn.getMetaData();

            String url = dbmd.getURL();

            // 去掉参数, 访问数据库用户名 / 密码被显示
            System.out.println("INFO: Routing to " + url.split("\\?")[0]);

            int updateCount;

            do {
                // 获取更新记录
                updateCount = stmt.getUpdateCount();

                if (updateCount >= 0) {

                    System.out.println("INFO: Query OK, " + updateCount + " records affected.");

                } else {

                    // 获取查询结果
                    ResultSet rs = stmt.getResultSet();

                    print(cout, rs);

                    cout.println();
                }

            } while (stmt.getMoreResults());

        } catch (SQLException e) {

            if (logger.isDebugEnabled()) {

                logger.error("ERROR: SQL exception: " + command, e);

            } else {

                logger.error("ERROR: SQL exception: " + command + " - " + e.getLocalizedMessage());
            }

        } catch (Throwable e) {

            if (logger.isDebugEnabled()) {

                logger.error("ERROR: Unknown exception: " + command, e);

            } else {

                logger.error("ERROR: Unknown exception: " + command + " - "
                        + e.getLocalizedMessage());
            }

        } finally {

            JdbcUtils.safeClose(stmt);
            JdbcUtils.safeClose(conn);
        }
    }

    // 输出所给的结果
    private static void print(PrintWriter cout, ResultSet rs) throws SQLException {

        ResultSetMetaData rsmd = rs.getMetaData();

        // 整理查询结果
        HashSet<String> tables = new HashSet<String>();

        int columnCount = rsmd.getColumnCount();

        int[] columnWidths = new int[columnCount];

        String[] columns = new String[columnCount];

        for (int column = 0; column < columnCount; column++) {

            String columnName = rsmd.getColumnLabel(column + 1);

            columns[column] = columnName;

            columnWidths[column] = columnName.length();

            String table = rsmd.getTableName(column + 1);

            if (!table.isEmpty()) {
                tables.add(table);
            }
        }

        ArrayList<String[]> valuesList = new ArrayList<String[]>();

        while (rs.next()) {

            String[] values = new String[columnCount];

            for (int column = 0; column < columnCount; column++) {

                String value = rs.getString(column + 1);

                if (value == null) {
                    value = "NULL"; // 显示大写的  'NULL'
                }

                values[column] = value;

                if (columnWidths[column] < value.length()) {
                    columnWidths[column] = value.length();
                }
            }

            valuesList.add(values);
        }

        // 打印查询结果
        StringBuilder line = new StringBuilder();
        StringBuilder head = new StringBuilder();

        for (int column = 0; column < columnCount; column++) {

            line.append('+');
            line.append('-');
            head.append('|');
            head.append(' ');

            int displaySize = columnWidths[column];

            for (int i = 0; i < displaySize; i++) {
                line.append('-');
            }

            String columnName = columns[column];

            head.append(columnName);

            for (int i = columnName.length(); i < displaySize; i++) {
                head.append(' ');
            }

            line.append('-');
            head.append(' ');
        }

        head.append('|');
        line.append('+');

        cout.println(line);
        cout.println(head);
        cout.println(line);

        for (String[] values : valuesList) {

            for (int column = 0; column < columnCount; column++) {

                cout.print('|');
                cout.print(' ');

                int displaySize = columnWidths[column];

                String value = values[column];

                cout.print(value);

                for (int i = value.length(); i < displaySize; i++) {
                    cout.print(' ');
                }

                cout.print(' ');
            }

            cout.println('|');
        }

        cout.println(line);

        if (!tables.isEmpty()) {

            cout.print("INFO: Records from table");

            for (String table : tables) {
                cout.print(' ');
                cout.print(table);
            }

            cout.println();
        }

        cout.println("INFO: " + valuesList.size() + " records in set.");
    }
}
