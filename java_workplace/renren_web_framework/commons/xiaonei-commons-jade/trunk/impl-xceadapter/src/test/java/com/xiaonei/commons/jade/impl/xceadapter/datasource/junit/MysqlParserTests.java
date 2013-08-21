package com.xiaonei.commons.jade.impl.xceadapter.datasource.junit;

import java.io.StringReader;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import junit.framework.TestCase;

import org.junit.Assert;

import com.meidusa.amoeba.mysql.parser.sql.MysqlParser;
import com.meidusa.amoeba.parser.ParseException;
import com.meidusa.amoeba.parser.dbobject.Column;
import com.meidusa.amoeba.parser.dbobject.Table;
import com.meidusa.amoeba.parser.expression.BaseExpressionList;
import com.meidusa.amoeba.parser.expression.ColumnExpression;
import com.meidusa.amoeba.parser.expression.ComparisonExpression;
import com.meidusa.amoeba.parser.expression.ConstantExpression;
import com.meidusa.amoeba.parser.expression.Expression;
import com.meidusa.amoeba.parser.expression.FunctionExpression;
import com.meidusa.amoeba.parser.expression.ParameterExpression;
import com.meidusa.amoeba.parser.statment.DMLStatment;
import com.meidusa.amoeba.parser.statment.Statment;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceParseInfo;

/**
 * 测试: Amoeba 自带的: MySQL 语句解析器。
 * 
 * @author han.liao
 */
public class MysqlParserTests extends TestCase {

    /**
     * 测试: SELECT 语句解析。
     */
    public void testSelect() {

        String SQL = "SELECT month, account.day FROM account WHERE month = ? "
                + "AND account.minute = ? AND (creation_date < 0 OR name LIKE ?) "
                + "AND week = hour(now()) ORDER BY id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("account", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("month".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("account", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("account", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("name".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.month")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.minute")));
        Assert.assertEquals(0L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.creation_date")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.name")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.week")));
    }

    /**
     * 测试: SELECT 语句解析。
     */
    public void testSelectForceIndex() {

        String SQL = "SELECT `month`, account.day FROM account "
                + "FORCE INDEX (index_for_month) WHERE month = ? "
                + "AND account.minute = ? AND (creation_date < 0 OR name LIKE ?) "
                + "AND week = hour(now()) ORDER BY id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("account", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("month".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("account", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("account", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("name".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.month")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.minute")));
        Assert.assertEquals(0L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.creation_date")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.name")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.week")));
    }

    /**
     * 测试: SELECT ... LIMIT 语句解析。
     */
    public void testSelectLimit() {

        String SQL = "SELECT id, month, message.day FROM message WHERE hour = ? AND "
                + "message.minute = ? ORDER BY id ASC LIMIT 10, 30";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: SELECT ... LIMIT 语句解析。
     */
    public void testSelectLimit2() {

        String SQL = "SELECT id, month, message.day FROM message WHERE hour = ? AND "
                + "message.minute = ? ORDER BY id ASC LIMIT ?,?";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(4, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: SELECT ... LIMIT 语句解析。
     */
    public void testSelectLimit3() {

        String SQL = "SELECT id, month, message.day FROM message WHERE hour = ? AND "
                + "message.minute = ? ORDER BY id ASC LIMIT ?, 20";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: SELECT ... LIMIT 语句解析。
     */
    public void testSelectLimit4() {

        String SQL = "SELECT id, month, message.day FROM message WHERE hour = ? AND "
                + "message.minute = ? ORDER BY id ASC LIMIT ?";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: PostgreSQL: SELECT ... LIMIT n OFFSET m 语句解析。
     */
    public void testSelectLimitOffset() {

        String SQL = "SELECT id, `month`, message.day FROM message WHERE `hour` = ? AND "
                + "message.minute = ? ORDER BY id ASC LIMIT 20 OFFSET 10";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: SELECT ... LIMIT 语句解析。
     */
    public void testSelectIn() {

        String SQL = "SELECT id FROM message WHERE hour = ? AND "
                + "message.minute = ? AND user_id IN (?, 31, ?, 13) ORDER BY id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);

        Assert.assertEquals(4, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("message", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("minute".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("message", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("user_id".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNotNull(parseInfo.getColumn(4));
        Assert.assertEquals("message", parseInfo.getColumn(4).getTable().getName());
        Assert.assertEquals("user_id".toUpperCase(), parseInfo.getColumn(4).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.hour")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.minute")));
    }

    /**
     * 测试: SELECT ... LEFT JOIN 语句解析。
     */
    public void testJoin() {

        String SQL = "SELECT account.*, user.id FROM account LEFT JOIN user ON "
                + "user.month = account.day WHERE user.mintue = ? AND (account.year > 20 "
                + "OR user.name = 'xxx') ORDER BY id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(2, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);
        Assert.assertEquals(XceParseInfo.newTable("user"), parseInfo.getTables()[1]);

        Assert.assertEquals(1, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("user", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("mintue".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("user.mintue")));
        Assert.assertEquals(20L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.year")));
        Assert.assertEquals("xxx", parseInfo.getParam( // NL
                XceParseInfo.newColumn("user.name")));
    }

    /**
     * 测试: SELECT ... 从多个数据表获取数据的解析。
     */
    public void testMultiTable() {

        String SQL = "SELECT account.id, user.id, message.* FROM account, user, message "
                + "WHERE account.year = user.id AND user.id = message.day AND user.month = ? "
                + "AND (message.creation_date > 20 OR user.name = 'xx') AND account.id = ? "
                + "ORDER BY id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(3, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message"), parseInfo.getTables()[0]);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[1]);
        Assert.assertEquals(XceParseInfo.newTable("user"), parseInfo.getTables()[2]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("user", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("month".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("account", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("user.month")));
        Assert.assertEquals(20L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.creation_date")));
        Assert.assertEquals("xx", parseInfo.getParam( // NL
                XceParseInfo.newColumn("user.name")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.id")));
    }

    /**
     * 测试: SELECT ... JOIN 多张表语句解析。
     */
    public void testAlias() {

        String SQL = "SELECT A.id, A.name, M.minute, M.content FROM account A, "
                + "message AS M WHERE A.m_id = M.id AND M.id = A.day AND A.month = ? "
                + "AND (M.creation_date > 20 OR A.name = 'xx') AND M.id = ? " // NL
                + "ORDER BY A.id ASC";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(2, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("message", "A"), parseInfo.getTables()[0]);
        Assert.assertEquals(XceParseInfo.newTable("account", "M"), parseInfo.getTables()[1]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("account", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("month".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("message", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.month")));
        Assert.assertEquals(20L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.creation_date")));
        Assert.assertEquals("xx", parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.name")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("message.id")));
    }

    /**
     * 测试: INSERT 语句解析。
     */
    public void testInsert() {

        String SQL = "INSERT account (id, user_id, month, `hour`, profile, creation_date) "
                + "VALUES (?, ?, 11, ?, 'ddd', ?)";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);

        Assert.assertEquals(4, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("account", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("account", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("user_id".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("account", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNotNull(parseInfo.getColumn(4));
        Assert.assertEquals("account", parseInfo.getColumn(4).getTable().getName());
        Assert.assertEquals("creation_date".toUpperCase(), parseInfo.getColumn(4).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.user_id")));
        Assert.assertEquals(11L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.month")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.hour")));
        Assert.assertEquals("ddd", parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.profile")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.creation_date")));
    }

    /**
     * 测试: INSERT 语句解析。
     */
    public void testInsertNoColumn() {

        String SQL = "INSERT INTO account VALUES (?, ?, ?, ?, ?)";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);

        Assert.assertEquals(5, parseInfo.getParamCount());
    }

    /**
     * 测试: INSERT INTO ... SELECT 语句解析。
     */
    public void testInsertSelect() {

        String SQL = "insert into inv_comment_del(id,inv_id,member_id,member_name,head_url,comment_body,"
                + "parent_id,comment_time,replynum,del_userid,del_time) select id,inv_id,"
                + "member_id,member_name,head_url,comment_body,parent_id,comment_time,"
                + "replynum,? as del_userid,now() as del_time from inv_comment where id=?";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(2, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("inv_comment_del"), parseInfo.getTables()[0]);
        Assert.assertEquals(XceParseInfo.newTable("inv_comment"), parseInfo.getTables()[1]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("inv_comment", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("inv_comment.id")));
    }

    /**
     * 测试: REPLACE INTO ... SELECT 语句解析。
     */
    public void testReplaceSelect() {

        String SQL = "replace into album_comment_del select id, album, author, name, "
                + "head_url, body, time, ?, now(), 278299750, 0 from  album_comment "
                + "where album = ? and owner = ?";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(2, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("album_comment_del"), parseInfo.getTables()[0]);
        Assert.assertEquals(XceParseInfo.newTable("album_comment"), parseInfo.getTables()[1]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("album_comment", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("album".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("album_comment", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("owner".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("album_comment.album")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("album_comment.owner")));
    }

    /**
     * 测试: INSERT INTO ... SELECT 语句解析。
     */
    public void testInsertIgnoreInto() {

        String SQL = "insert ignore into inv_comment_del(id,inv_id,member_id,"
                + "member_name,head_url,comment_body,parent_id,comment_time,"
                + "replynum,del_userid,del_time) values (?,?,?,?,?,?,?,?,?,?,now())";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("inv_comment_del"), parseInfo.getTables()[0]);

        Assert.assertEquals(10, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("inv_id".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("member_id".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNotNull(parseInfo.getColumn(4));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(4).getTable().getName());
        Assert.assertEquals("member_name".toUpperCase(), parseInfo.getColumn(4).getName());

        Assert.assertNotNull(parseInfo.getColumn(5));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(5).getTable().getName());
        Assert.assertEquals("head_url".toUpperCase(), parseInfo.getColumn(5).getName());

        Assert.assertNotNull(parseInfo.getColumn(6));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(6).getTable().getName());
        Assert.assertEquals("comment_body".toUpperCase(), parseInfo.getColumn(6).getName());

        Assert.assertNotNull(parseInfo.getColumn(7));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(7).getTable().getName());
        Assert.assertEquals("parent_id".toUpperCase(), parseInfo.getColumn(7).getName());

        Assert.assertNotNull(parseInfo.getColumn(8));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(8).getTable().getName());
        Assert.assertEquals("comment_time".toUpperCase(), parseInfo.getColumn(8).getName());

        Assert.assertNotNull(parseInfo.getColumn(9));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(9).getTable().getName());
        Assert.assertEquals("replynum".toUpperCase(), parseInfo.getColumn(9).getName());

        Assert.assertNotNull(parseInfo.getColumn(10));
        Assert.assertEquals("inv_comment_del", parseInfo.getColumn(10).getTable().getName());
        Assert.assertEquals("del_userid".toUpperCase(), parseInfo.getColumn(10).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.inv_id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.member_id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.member_name")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.head_url")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.comment_body")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.parent_id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.comment_time")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.replynum")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.del_userid")));
    }

    /**
     * 测试: INSERT INTO ... ON DUPLICATE KEY UPDATE 语句解析。
     */
    public void testDuplicateKeyUpdate() {

        // 注意：为兼容这类查询语句, 直接忽略  "ON DUPLICATE KEY UPDATE" 之后的部分
        String SQL = "INSERT INTO friends_doing (id, userid, content, dtime, `type`, link , meta)"
                + " VALUES (?, ?, ?, now(), ?, ?, ?) ON DUPLICATE KEY UPDATE"
                + " id = ?, content = ?, dtime = now(), `type` = ?, link = ?, meta = ?";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("friends_doing"), parseInfo.getTables()[0]);

        Assert.assertEquals(6, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("userid".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("content".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNotNull(parseInfo.getColumn(4));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(4).getTable().getName());
        Assert.assertEquals("type".toUpperCase(), parseInfo.getColumn(4).getName());

        Assert.assertNotNull(parseInfo.getColumn(5));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(5).getTable().getName());
        Assert.assertEquals("link".toUpperCase(), parseInfo.getColumn(5).getName());

        Assert.assertNotNull(parseInfo.getColumn(6));
        Assert.assertEquals("friends_doing", parseInfo.getColumn(6).getTable().getName());
        Assert.assertEquals("meta".toUpperCase(), parseInfo.getColumn(6).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "userid")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "content")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "type")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "link")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("friends_doing", "meta")));
    }

    /**
     * 测试: REPLACE 语句解析。
     */
    public void testReplace() {

        String SQL = "REPLACE INTO month (name, profile, hour, minute, second) "
                + "VALUES (?, 'RRR', ?, 60, ?)";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("month"), parseInfo.getTables()[0]);

        Assert.assertEquals(3, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("month", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("name".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("month", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("month", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("second".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("month.name")));
        Assert.assertEquals("RRR", parseInfo.getParam( // NL
                XceParseInfo.newColumn("month.profile")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("month.hour")));
        Assert.assertEquals(60L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("month.minute")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("month.second")));
    }

    /**
     * 测试: UPDATE 语句解析。
     */
    public void testUpdate() {

        String SQL = "UPDATE account SET id = id + 1, month = 3, hour = 9, profile = 'day' "
                + "WHERE month = ? AND hour = ? AND year = 2009";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("account"), parseInfo.getTables()[0]);

        Assert.assertEquals(2, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("account", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("month".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("account", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("hour".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.month")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.hour")));
        Assert.assertEquals(2009L, parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.year")));
    }

    /**
     * 测试: 包含转义的 SQL 语句解析。
     */
    public void testEscapedColumn() {
        String SQL = "INSERT INTO doing_del "
                + "(id, userid, content, dtime, `type`, comment_count, `link`,"
                + " meta, source, del_userid, del_time) VALUES"
                + " (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, now())";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(1, parseInfo.getTables().length);
        Assert.assertEquals(XceParseInfo.newTable("doing_del"), parseInfo.getTables()[0]);

        Assert.assertEquals(10, parseInfo.getParamCount());

        Assert.assertNotNull(parseInfo.getColumn(1));
        Assert.assertEquals("doing_del", parseInfo.getColumn(1).getTable().getName());
        Assert.assertEquals("id".toUpperCase(), parseInfo.getColumn(1).getName());

        Assert.assertNotNull(parseInfo.getColumn(2));
        Assert.assertEquals("doing_del", parseInfo.getColumn(2).getTable().getName());
        Assert.assertEquals("userid".toUpperCase(), parseInfo.getColumn(2).getName());

        Assert.assertNotNull(parseInfo.getColumn(3));
        Assert.assertEquals("doing_del", parseInfo.getColumn(3).getTable().getName());
        Assert.assertEquals("content".toUpperCase(), parseInfo.getColumn(3).getName());

        Assert.assertNotNull(parseInfo.getColumn(4));
        Assert.assertEquals("doing_del", parseInfo.getColumn(4).getTable().getName());
        Assert.assertEquals("dtime".toUpperCase(), parseInfo.getColumn(4).getName());

        Assert.assertNotNull(parseInfo.getColumn(5));
        Assert.assertEquals("doing_del", parseInfo.getColumn(5).getTable().getName());
        Assert.assertEquals("type".toUpperCase(), parseInfo.getColumn(5).getName());

        Assert.assertNotNull(parseInfo.getColumn(6));
        Assert.assertEquals("doing_del", parseInfo.getColumn(6).getTable().getName());
        Assert.assertEquals("comment_count".toUpperCase(), parseInfo.getColumn(6).getName());

        Assert.assertNotNull(parseInfo.getColumn(7));
        Assert.assertEquals("doing_del", parseInfo.getColumn(7).getTable().getName());
        Assert.assertEquals("link".toUpperCase(), parseInfo.getColumn(7).getName());

        Assert.assertNotNull(parseInfo.getColumn(8));
        Assert.assertEquals("doing_del", parseInfo.getColumn(8).getTable().getName());
        Assert.assertEquals("meta".toUpperCase(), parseInfo.getColumn(8).getName());

        Assert.assertNotNull(parseInfo.getColumn(9));
        Assert.assertEquals("doing_del", parseInfo.getColumn(9).getTable().getName());
        Assert.assertEquals("source".toUpperCase(), parseInfo.getColumn(9).getName());

        Assert.assertNotNull(parseInfo.getColumn(10));
        Assert.assertEquals("doing_del", parseInfo.getColumn(10).getTable().getName());
        Assert.assertEquals("del_userid".toUpperCase(), parseInfo.getColumn(10).getName());

        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.id")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.userid")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.content")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.dtime")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.type")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.comment_count")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.link")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.meta")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.source")));
        Assert.assertNull(parseInfo.getParam( // NL
                XceParseInfo.newColumn("account.del_userid")));
    }

    /**
     * 测试: SELECT LAST_INSERT_ID() 语句解析。
     */
    public void testLastId() {

        String SQL = "SELECT LAST_INSERT_ID()";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(0, parseInfo.getTables().length);
        Assert.assertEquals(0, parseInfo.getParamCount());
    }

    /**
     * 测试: SELECT @@LAST_INSERT_ID 语句解析。
     */
    public void testVariant() {

        String SQL = "SELECT @@LAST_INSERT_ID";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(0, parseInfo.getTables().length);
        Assert.assertEquals(0, parseInfo.getParamCount());
    }

    /**
     * 测试: SELECT NEXTVAL('name') 语句解析。
     */
    public void testNextVal() {

        String SQL = "SELECT NEXTVAL('name'::text)";
        underMySqlParser(SQL);

        XceParseInfo parseInfo = new XceParseInfo(SQL);
        parseInfo.parseInfo();

        Assert.assertEquals(0, parseInfo.getTables().length);
        Assert.assertEquals(0, parseInfo.getParamCount());
    }

    // 匹配  LIMIT n, m 语法
    protected static Pattern LIMIT_PATTERN = Pattern.compile(
            "LIMIT\\s+(\\d+|\\?)(?:,\\s*(\\d+|\\?))?\\s*$", Pattern.CASE_INSENSITIVE);

    /**
     * 执行 MysqlParser 解析，并检查解析失败。
     * 
     * @param string - 输入字符串。
     */
    private void underMySqlParser(String string) {

        String parseSql = string;

        // 解决  LIMIT n, m 语法的解析问题
        Matcher matcher = LIMIT_PATTERN.matcher(string);
        if (matcher.find()) {
            parseSql = string.substring(0, matcher.start());
        }

        // 使用  Amoeba 自带的解析器分析查询语句。
        MysqlParser parser = new MysqlParser(new StringReader(parseSql));
        parser.setFunctionMap(XceParseInfo.getFunctionMap());
        parser.enable_tracing();

        try {
            outputStatement(parser.doParse(), 0);

        } catch (ParseException e) {

            e.printStackTrace();
            Assert.fail("fail in parsing sql: " + string);
        }
    }

    /**
     * 按给定数目的空格缩进，输出给定的字符串。
     * 
     * @param string - 输出的字符串
     * 
     * @param indent - 缩进数目
     */
    private static void outputIndent(String string, int indent) {

        char[] array = new char[indent * 2];

        Arrays.fill(array, ' ');

        System.out.println(new String(array) + string);
    }

    /**
     * 递归输出所给的解析表达式。
     * 
     * @param expr - 输出的表达式
     * 
     * @param indent - 缩进数目
     */
    private static void outputExpressions(Expression expr, int indent) {

        if (expr instanceof BaseExpressionList) {

            // 输出组合表达式
            BaseExpressionList exprList = (BaseExpressionList) expr;
            outputIndent(exprList.getClass() + " (" + exprList + ") {", indent);

            List<Expression> list = exprList.getAllExpression();
            for (int i = 0; i < list.size(); i++) {
                outputExpressions(list.get(i), indent + 1);
            }

            outputIndent("}", indent);

        } else if (expr instanceof ColumnExpression) {

            // 输出列表达式
            ColumnExpression columnExpr = (ColumnExpression) expr;
            outputIndent(columnExpr.getClass() + "(" + columnExpr + ") {", indent);

            Column column = columnExpr.getColumn();
            outputIndent(column.getClass() + "(" + column + ") {", indent + 1);
            outputIndent("Table = " + column.getTable(), indent + 2);
            outputIndent("Name  = " + column.getName(), indent + 2);
            outputIndent("}", indent + 1);

            outputExpressions(columnExpr.getExpression(), indent + 1);
            outputIndent("}", indent);

        } else if (expr instanceof ComparisonExpression) {

            // 输出比较表达式
            ComparisonExpression compExpr = (ComparisonExpression) expr;
            outputIndent(compExpr.getClass() + "(" + compExpr + ") {", indent);
            outputIndent("Comparison = " + compExpr.getComparison(), indent + 1);

            outputExpressions(compExpr.getExpression(), indent + 1);
            outputIndent("}", indent);

        } else if (expr instanceof ParameterExpression) {

            // 输出待定参数表达式
            ParameterExpression paramExpr = (ParameterExpression) expr;
            outputIndent(paramExpr.getClass() + "(" + paramExpr + ")", indent);

        } else if (expr instanceof ConstantExpression) {

            // 输出常量表达式
            ConstantExpression constExpr = (ConstantExpression) expr;
            outputIndent(constExpr.getClass() + "(" + constExpr + ")", indent);

        } else if (expr instanceof FunctionExpression) {

            // 输出函数表达式
            FunctionExpression funcExpr = (FunctionExpression) expr;
            outputIndent(funcExpr.getClass() + "(" + funcExpr + ") {", indent);
            outputIndent("Function = " + funcExpr.getFunction(), indent + 1);
            outputIndent("ArgList  = {", indent + 1);

            List<Expression> list = funcExpr.getArgList();
            for (int i = 0; i < list.size(); i++) {
                outputExpressions(list.get(i), indent + 2);
            }

            outputIndent("}", indent + 1);
            outputIndent("}", indent);
        }
    }

    /**
     * 递归输出所给的解析语句。
     * 
     * @param stmt - 输出的解析语句
     * 
     * @param indent - 缩进数目
     */
    private static void outputStatement(Statment stmt, int indent) {

        if (stmt instanceof DMLStatment) {

            DMLStatment dmlStmt = (DMLStatment) stmt;
            outputIndent(dmlStmt.getClass() + "(" + dmlStmt.toString() + ") {", indent);
            outputIndent("ParameterCount = " + dmlStmt.getParameterCount(), indent + 1);
            outputIndent("Tables = {", indent + 1);

            for (Table table : dmlStmt.getTables()) {
                outputIndent(table.getClass() + "(" + table + ") {", indent + 2);
                outputIndent("Schema = " + table.getSchema(), indent + 3);
                outputIndent("Name   = " + table.getName(), indent + 3);
                outputIndent("Alias  = " + table.getAlias(), indent + 3);
                outputIndent("}", indent + 2);
            }

            outputIndent("}", indent + 1);

            Expression expr = dmlStmt.getExpression();
            if (expr != null) {
                outputIndent("Expression = {", indent + 1);
                outputExpressions(expr, indent + 2);
                outputIndent("}", indent + 1);
            }

            outputIndent("}", indent);

        } else {
            outputIndent(stmt.getClass() + "(" + stmt + ")", indent);
        }
    }
}
