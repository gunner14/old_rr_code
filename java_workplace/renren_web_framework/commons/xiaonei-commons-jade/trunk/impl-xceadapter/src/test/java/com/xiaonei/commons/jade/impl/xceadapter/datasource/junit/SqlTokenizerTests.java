package com.xiaonei.commons.jade.impl.xceadapter.datasource.junit;

import junit.framework.TestCase;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.SqlTokenizer;

/**
 * 测试简化: SQL 解析工具。
 * 
 * @author han.liao
 */
public class SqlTokenizerTests extends TestCase {

    /**
     * 测试解析: SELECT 语句。
     */
    public void testSelect() {

        String sql = "SELECT * FROM account WHERE 1 = 1 AND id = ? AND (creation_date > 20 OR name = 'xx') ORDER BY id ASC";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        tokenizer.setUnwrapBracket(true);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: SELECT ... JOIN 语句。
     */
    public void testJoin() {

        String sql = "SELECT * FROM account LEFT JOIN user ON user.id = account.user_id WHERE user.id = ? AND (account.creation_date > 20 OR account.name = 'xx') ORDER BY id ASC";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: SELECT ... JOIN 多表语句。
     */
    public void testMultiTable() {

        String sql = "SELECT * FROM account, user, message WHERE account.user_id = user.id AND user.id = message.user_id AND user.id = ? AND (message.creation_date > 20 OR user.name = 'xx') ORDER BY id ASC";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        tokenizer.setWantedComma(true);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: INSERT 语句。
     */
    public void testInsert() {

        String sql = "INSERT INTO account (id, user_id, name, profile, creation_date) VALUES (?, ?, ?, ?, ?)";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: REPLACE 语句。
     */
    public void testReplace() {

        String sql = "REPLACE INTO account (name, profile) VALUES (?, ?)";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: UPDATE 语句。
     */
    public void testUpdate() {

        String sql = "UPDATE account SET name = ?, profile = ? WHERE id = ?";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: SELECT LAST_INSERT_ID() 语句。
     */
    public void testLastId() {

        String sql = "SELECT LAST_INSERT_ID()";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }

    /**
     * 测试解析: SELECT @@LAST_INSERT_ID 语句。
     */
    public void testVariant() {

        String sql = "SELECT @@LAST_INSERT_ID";

        System.out.println(sql);

        SqlTokenizer tokenizer = new SqlTokenizer(sql);

        while (tokenizer.moreTokens()) {

            System.out.print(tokenizer.nextToken() + '|');
        }

        System.out.println();
    }
}
