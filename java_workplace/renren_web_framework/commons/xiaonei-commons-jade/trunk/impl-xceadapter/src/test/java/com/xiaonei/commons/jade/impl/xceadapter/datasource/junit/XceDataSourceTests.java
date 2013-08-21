/**
 * 
 */
package com.xiaonei.commons.jade.impl.xceadapter.datasource.junit;

import java.sql.Connection;
import java.sql.Driver;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.Date;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.concurrent.atomic.AtomicInteger;

import javax.sql.DataSource;

import junit.framework.TestCase;

import org.apache.commons.dbcp.BasicDataSource;
import org.apache.log4j.PropertyConfigurator;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCreator;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.datasource.SimpleDriverDataSource;

import com.xiaonei.commons.jade.impl.jdbcwrapper.RWSplitDataSource;
import com.xiaonei.commons.jade.impl.xceadapter.configurator.XmlDocXceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.connector.TestXceConnector;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceDataSource;

/**
 * 演示: JdbcTemplate 的功能。
 * 
 * @author han.liao
 */
public class XceDataSourceTests extends TestCase {

    // 使用的. JdbcTemplate 实例。
    protected JdbcTemplate jdbcTemplate;

    protected boolean validate = false;

    protected boolean printInfo = false;

    protected AtomicInteger id = new AtomicInteger(1);

    protected ThreadLocal<Integer> id1 = new ThreadLocal<Integer>();

    protected ThreadLocal<Integer> id2 = new ThreadLocal<Integer>();

    protected ThreadLocal<Integer> id3 = new ThreadLocal<Integer>();

    static {

        // 初始化  log4j 配置 
        Properties props = new Properties();

        props.setProperty("log4j.rootLogger", "DEBUG, stdout");
        props.setProperty("log4j.appender.stdout", "org.apache.log4j.ConsoleAppender");
        props.setProperty("log4j.appender.stdout.layout", "org.apache.log4j.PatternLayout");

        PropertyConfigurator.configure(props);
    }

    /**
     * @throws java.lang.Exception
     */
    @Override
    public void setUp() throws Exception {

        // 创建: SimpleDriverDataSource 用于测试。
        // DataSource ds = createSimpleDataSource();
        // DataSource ds = createRealDataSource();
        // DataSource ds = createHADataSource();
        DataSource ds = createXceDataSource();

        // 创建 : JdbcTemplate 实例。
        jdbcTemplate = new JdbcTemplate(ds);

        id1.set(1);
        id2.set(2);
        id3.set(3);
    }

    /**
     * 执行数据表删除添加。
     */
    public void testDropCreate() {

        // 删除拆分数据表
        jdbcTemplate.execute("DROP TABLE IF EXISTS account");
        jdbcTemplate.execute("DROP TABLE IF EXISTS account_1st");
        jdbcTemplate.execute("DROP TABLE IF EXISTS account_2nd");
        jdbcTemplate.execute("DROP TABLE IF EXISTS account_3rd");
        jdbcTemplate.execute("DROP TABLE IF EXISTS account_4th");
        jdbcTemplate.execute("DROP TABLE IF EXISTS account_5th");

        // 创建拆分数据表
        jdbcTemplate.execute("CREATE TABLE account (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");

        jdbcTemplate.execute("CREATE TABLE account_1st (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");

        // 创建拆分数据表
        jdbcTemplate.execute("CREATE TABLE account_2nd (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");

        // 创建拆分数据表
        jdbcTemplate.execute("CREATE TABLE account_3rd (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");

        // 创建拆分数据表
        jdbcTemplate.execute("CREATE TABLE account_4th (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");

        // 创建拆分数据表
        jdbcTemplate.execute("CREATE TABLE account_5th (" + "id               INTEGER NOT NULL, "
                + "name             VARCHAR(40) NOT NULL, "
                + "password         VARCHAR(20) NOT NULL, " // NL
                + "profile          VARCHAR(255), " // NL
                + "creation_date    DATE NOT NULL, " // NL 
                + "updating_date    DATE NULL)");
    }

    /**
     * 插入数据。
     */
    public void testInsert() {

        // 执行固定的语句
        id1.set(id.getAndAdd(1));

        int number = jdbcTemplate
                .update("INSERT INTO account (id, name, password, creation_date) VALUES ("
                        + id1.get() + ", '张三', 'zhang.san', '2009-08-31')");

        if (validate) {
            assertEquals(1, number);
        }

        if (printInfo) {
            System.out.println("jdbcTemplate.update 01: " + number);
        }

        // 使用数组传递参数
        id2.set(id.getAndAdd(1));

        number = jdbcTemplate.update(
                "INSERT INTO account (id, name, password, creation_date) VALUES (?, ?, ?, ?)",
                new Object[] { id2.get(), "李四", "li.si", new Date() });

        if (validate) {
            assertEquals(1, number);
        }

        if (printInfo) {
            System.out.println("jdbcTemplate.update 02: " + number);
        }

        // 使用 数组同时传递参数与类型
        id3.set(id.getAndAdd(1));

        number = jdbcTemplate
                .update(
                        "INSERT INTO account (id, name, password, profile, creation_date) VALUES (?, ?, ?, ?, ?)",
                        new Object[] { id3.get(), "王五", "wang.wu", "王五的档案", new Date() }, // NL
                        new int[] { Types.INTEGER, Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
                                Types.DATE });

        if (validate) {
            assertEquals(1, number);
        }

        if (printInfo) {
            System.out.println("jdbcTemplate.update 03: " + number);
        }
    }

    /**
     * 更新数据。
     */
    public void testUpdate() {

        // 使用: PreparedStatementSetter 执行查询
        int number = jdbcTemplate.update(
                "UPDATE account SET profile = ?, updating_date = ? WHERE id = ?",
                new PreparedStatementSetter() {

                    @Override
                    public void setValues(PreparedStatement ps) throws SQLException {

                        ps.setString(1, "张三是个快乐的青年");
                        ps.setDate(2, new java.sql.Date(System.currentTimeMillis()));
                        ps.setInt(3, id1.get());
                    }
                });

        if (validate) {
            assertEquals(1, number);
        }

        if (printInfo) {
            System.out.println("jdbcTemplate.update 04: " + number);
        }

        // 使用: PreparedStatementCreator 执行查询
        number = jdbcTemplate.update(new PreparedStatementCreator() {

            @Override
            public PreparedStatement createPreparedStatement(Connection con) throws SQLException {

                PreparedStatement ps = con
                        .prepareStatement("UPDATE account SET profile = ?, updating_date = ? WHERE id = ?");

                ps.setString(1, "李四是个问题中年");
                ps.setDate(2, new java.sql.Date(System.currentTimeMillis()));
                ps.setInt(3, id2.get());

                return ps;
            }
        });

        if (validate) {
            assertEquals(1, number);
        }

        if (printInfo) {
            System.out.println("jdbcTemplate.update 05: " + number);
        }
    }

    /**
     * 数据库查询。
     */
    public void testSimpleQuery() {

        // 使用 : queryForMap 查询: 
        Map<?, ?> result = jdbcTemplate.queryForMap("SELECT * FROM account WHERE id = ?",
                new Object[] { id3.get() });

        for (Entry<?, ?> entry : result.entrySet()) {

            if (printInfo) {
                System.out.println("jdbcTemplate.queryForMap: " + entry.getKey() + " = "
                        + entry.getValue());
            }
        }
    }

    /**
     * 包含对象映射的数据库查询。
     */
    public void testQuery() {

        // 创建定制的 : RowMapper 对象
        RowMapper mapper = new RowMapper() {

            @Override
            public Object mapRow(ResultSet rs, int rowNum) throws SQLException {

                Account account = new Account();

                account.setId(rs.getInt("id"));
                account.setName(rs.getString("name"));
                account.setPassword(rs.getString("password"));
                account.setProfile(rs.getString("profile"));
                account.setCreationDate(rs.getDate("creation_date"));
                account.setUpdatingDate(rs.getDate("updating_date"));

                return account;
            }
        };

        // 使用 : queryForObject 查询单个对象: 
        Account account = (Account) jdbcTemplate.queryForObject(
                "SELECT * FROM account WHERE id = ?", new Object[] { id1.get() }, mapper);

        if (printInfo) {
            System.out.println("jdbcTemplate.queryForObject: " + account);
        }
    }

    /**
     * 创建: SimpleHADataSource 用于测试。
     * 
     * @return DataSource.
     */
    protected static DataSource createXceDataSource() {

        TestXceConnector connector = new TestXceConnector();

        connector.setUrl("jdbc:mysql://localhost/test?useUnicode=true&characterEncoding=utf8");
        connector.setUsername("root");
        connector.setPassword("password");

        // PropertiesXceConfigurator configurator = new PropertiesXceConfigurator();
        // RemoteXceConfigurator configurator = new RemoteXceConfigurator();
        XmlDocXceConfigurator configurator = new XmlDocXceConfigurator();

        XceDataSource ds = new XceDataSource(configurator, connector);

        ds.setCatalog("demo");

        return ds;
    }

    /**
     * 创建: SimpleHADataSource 用于测试。
     * 
     * @return DataSource.
     */
    protected static DataSource createHADataSource() {

        DataSource source = createRealDataSource();

        return new RWSplitDataSource(source, source);
    }

    /**
     * 创建: BasicDataSource 用于测试。
     * 
     * @return DataSource.
     */
    protected static DataSource createRealDataSource() {

        try {
            Class.forName("com.mysql.jdbc.Driver");

            BasicDataSource ds = new BasicDataSource();

            ds.setDriverClassName("com.mysql.jdbc.Driver");

            ds.setUrl("jdbc:mysql://localhost/test?useUnicode=true&characterEncoding=utf8");

            ds.setUsername("root");
            ds.setPassword("password");

            ds.setMaxIdle(40);
            ds.setMinIdle(10);

            ds.setMaxActive(100);

            return ds;

        } catch (Exception e /* ClassNotFoundException, InstantiationException, 
                                IllegalAccessException */) {
            e.printStackTrace();

            throw new RuntimeException(e);
        }
    }

    /**
     * 创建: SimpleDriverDataSource 用于测试。
     * 
     * @return DataSource.
     */
    protected static DataSource createSimpleDataSource() {

        try {
            Class<?> clazz = Class.forName("com.mysql.jdbc.Driver");

            Properties prop = new Properties();

            prop.put("user", "root");
            prop.put("password", "password");
            // prop.put("useUnicode", "true");
            // prop.put("characterEncoding", "UTF-8");

            return new SimpleDriverDataSource((Driver) clazz.newInstance(), // NL 
                    "jdbc:mysql://localhost/liaohan", prop);

        } catch (Exception e /* ClassNotFoundException, InstantiationException, 
                                IllegalAccessException */) {
            e.printStackTrace();

            throw new RuntimeException(e);
        }
    }

    public static void main(String... args) throws Exception {

        final int THREAD_COUNT = 10, LOOP_COUNT = 10;

        // 使用的锁
        final Object beforeLock = new Object();
        final Object notifyLock = new Object();

        final AtomicInteger notifyCount = new AtomicInteger(0);

        // 创建. JdbcTemplateTests
        final XceDataSourceTests xceDataSourceTests = new XceDataSourceTests();

        // 设置. JdbcTemplateTests
        xceDataSourceTests.setUp();

        xceDataSourceTests.validate = false;
        xceDataSourceTests.printInfo = false;

        // 重新创建数据表
        xceDataSourceTests.testDropCreate();

        for (int i = 0; i < THREAD_COUNT; i++) {

            final int tid = i;

            // 创建线程
            Thread thread = new Thread() {

                @Override
                public void run() {

                    try {
                        // 保证线程进入. wait() 后, main 线程才有机会调用. notifyAll. 
                        synchronized (beforeLock) {

                            notifyCount.addAndGet(1);

                            synchronized (notifyLock) {
                                notifyLock.notify();
                            }

                            System.out.println("[" + new Date() + "] Thread #" + tid + " launch");

                            beforeLock.wait();
                        }

                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    System.out.println("[" + new Date() + "] Thread #" + tid + " start");

                    int error = 0;

                    long performance = 0;

                    for (int index = 0; index < LOOP_COUNT; index++) {

                        long milliseconds = System.currentTimeMillis();

                        try {
                            // 调用测试方法。
                            xceDataSourceTests.testInsert();
                            xceDataSourceTests.testUpdate();
                            xceDataSourceTests.testSimpleQuery();
                            xceDataSourceTests.testQuery();

                        } catch (Exception e) {
                            e.printStackTrace();
                            error++;
                        }

                        performance += System.currentTimeMillis() - milliseconds;
                    }

                    System.out.println("[" + new Date() + "] Thread #" + tid + " end: error = "
                            + error + ", Avg performance = " + (performance / LOOP_COUNT) + "ms");
                }
            };

            thread.start();
        }

        System.out.println("[" + new Date() + "] All thread created");

        while (THREAD_COUNT > notifyCount.get()) {
            synchronized (notifyLock) {
                notifyLock.wait();
            }
        }

        System.out.println("[" + new Date() + "] All thread launched");

        synchronized (beforeLock) {
            beforeLock.notifyAll();
        }

        System.out.println();
    }
}

/**
 * 保存查询结果的: POJO 对象。
 * 
 * <pre>
 * +---------------+--------------+------+-----+---------+----------------+
 * | Field         | Type         | Null | Key | Default | Extra          |
 * +---------------+--------------+------+-----+---------+----------------+
 * | id            | int(11)      | NO   | PRI | NULL    | auto_increment |
 * | name          | varchar(40)  | NO   |     |         |                |
 * | password      | varchar(20)  | NO   |     |         |                |
 * | profile       | varchar(255) | YES  |     | NULL    |                |
 * | creation_date | date         | NO   |     |         |                |
 * | updating_date | date         | YES  |     | NULL    |                |
 * +---------------+--------------+------+-----+---------+----------------+
 * 
 * CREATE TABLE account (
 *     id               INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
 *     name             VARCHAR(40) NOT NULL,
 *     password         VARCHAR(20) NOT NULL,
 *     profile          VARCHAR(255),
 *     creation_date     DATE NOT NULL,
 *     updating_date     DATE NULL
 * );
 * </pre>
 */
class Account {

    int id;

    String name, password, profile;

    Date creationDate, updatingDate;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getProfile() {
        return profile;
    }

    public void setProfile(String profile) {
        this.profile = profile;
    }

    public Date getCreationDate() {
        return creationDate;
    }

    public void setCreationDate(Date creationDate) {
        this.creationDate = creationDate;
    }

    public Date getUpdatingDate() {
        return updatingDate;
    }

    public void setUpdatingDate(Date updatingDate) {
        this.updatingDate = updatingDate;
    }

    @Override
    public String toString() {
        return "account(" + id + ") " + name + " | " + password + " | " + profile + " | "
                + creationDate + " | " + updatingDate;
    }
}
