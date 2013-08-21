package com.renren.entrybase.mysql;

import java.util.Random;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.entrybase.EntryBase;
import com.renren.entrybase.model.Entry;
import com.renren.entrybase.model.Field;

public class CommentBaseTest {

    private static EntryBase base;

    private static CommentCenterDataSourceFactory mockDataSourceFactory;

    @BeforeClass
    public static void init() {
        EntryBaseBuilder builder = new EntryBaseBuilder();
        builder.setDataSource(mockDataSourceFactory = new CommentCenterDataSourceFactory());
        builder.setIndexFields("resource", new String[] { "resource", "author" });
        builder.setEntryPartitions(10); //
        builder.setIndexPartitions("default", 10); // 
        base = builder.build();
    }

    @AfterClass
    public static void destroy() {
        mockDataSourceFactory.destroy();
    }

    @Test
    public void test() {
        Entry entry = base.get(1);
        System.out.println("get: " + entry);

        base.deleteEntry(1);
        entry = base.get(1);
        System.out.println("delete and get: " + entry);

        entry = base.addEntry(1, //
                new Field("author", 1234567), //
                new Field("resource", 7654321), //
                new Field("author.name", "zhiliang.wang"), //
                new Field("body", "bodybodybody"));
        System.out.println("added: " + entry);

        entry = base.updateEntry(1, false, new Field("x", new Random().nextInt()));

        System.out.println("updated: " + entry);
    }
}
