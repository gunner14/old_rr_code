package com.xiaonei.commons.jade.impl.xceadapter.datasource.junit;

import junit.framework.Assert;
import junit.framework.TestCase;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.SqlRewriter;

/**
 * 测试重写: SQL 语句的工具类。
 * 
 * @author han.liao
 */
public class SqlRewriterTests extends TestCase {

    /**
     * 测试重写: SELECT 语句。
     */
    public void testSelect() {

        String SQL = "SELECT * FROM account WHERE 1 = 1 AND id = ? AND (creation_date > 20 OR name = 'xx') ORDER BY id ASC";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "SELECT * FROM account_x account WHERE 1 = 1 AND id = ? AND (creation_date > 20 OR name = 'xx') ORDER BY id ASC",
            rewritten);
    }

    /**
     * 测试重写: SELECT ... JOIN 语句。
     */
    public void testJoin() {

        String SQL = "SELECT * FROM account LEFT JOIN user ON user.id = account.user_id WHERE user.id = ? AND (account.creation_date > 20 OR account.name = 'xx') ORDER BY id ASC";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "SELECT * FROM account_x account LEFT JOIN user ON user.id = account.user_id WHERE user.id = ? AND (account.creation_date > 20 OR account.name = 'xx') ORDER BY id ASC",
            rewritten);
    }

    /**
     * 测试重写: SELECT ... JOIN 多张表语句。
     */
    public void testMultiTable() {

        String SQL = "SELECT * FROM account, user, message WHERE account.user_id = user.id AND user.id = message.user_id AND user.id = ? AND (message.creation_date > 20 OR user.name = 'xx') ORDER BY id ASC";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "SELECT * FROM account_x account, user, message WHERE account.user_id = user.id AND user.id = message.user_id AND user.id = ? AND (message.creation_date > 20 OR user.name = 'xx') ORDER BY id ASC",
            rewritten);
    }

    /**
     * 测试重写: INSERT 语句。
     */
    public void testInsert() {

        String SQL = "INSERT INTO account (id, user_id, name, profile, creation_date) VALUES (?, ?, ?, ?, ?)";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "INSERT INTO account_x (id, user_id, name, profile, creation_date) VALUES (?, ?, ?, ?, ?)",
            rewritten);
    }

    /**
     * 测试重写: INSERT INTO ... SELETE 语句。
     */
    public void testInsertSelect() {

        String SQL = "INSERT INTO account_del (id, user_id, name, profile, creation_date)" +
        		" SELECT id, user_id, name, profile, creation_date FROM account WHERE id = ?";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
                "INSERT INTO account_del (id, user_id, name, profile, creation_date)" +
                " SELECT id, user_id, name, profile, creation_date FROM account_x account WHERE id = ?",
            rewritten);
    }

    /**
     * 测试重写: REPLACE 语句。
     */
    public void testReplace() {

        String SQL = "REPLACE INTO account (name, profile) VALUES (?, ?)";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "REPLACE INTO account_x (name, profile) VALUES (?, ?)",
            rewritten);
    }

    /**
     * 测试重写: UPDATE 语句。
     */
    public void testUpdate() {

        String SQL = "UPDATE account SET name = ?, profile = ? WHERE id = ?";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(
            "UPDATE account_x account SET name = ?, profile = ? WHERE id = ?",
            rewritten);
    }

    /**
     * 测试重写: SELECT LAST_INSERT_ID() 语句。
     */
    public void testLastId() {

        String SQL = "SELECT LAST_INSERT_ID()";
        System.out.println(SQL);

        String rewritten = SqlRewriter.rewriteSqlTable(SQL, "account", "account_x");
        System.out.println(rewritten);
        System.out.println();

        Assert.assertEquals(SQL, rewritten);
    }
}
