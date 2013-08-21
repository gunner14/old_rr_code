package com.renren.entrybase.model.query.parser;

import java.util.List;

import junit.framework.Assert;

import org.junit.Test;

import com.renren.entrybase.model.query.QField;
import com.renren.entrybase.model.query.Query;

public class QueryTest {

    @Test
    public void test1() {
        String queryString = "tag:1234 author:(5678 5432)";
        Query query = Query.toQuery(queryString);
        Assert.assertEquals("tag:(1234) and author:(5678 or 5432);", query.toString());

        List<QField> fields = query.getFields();
        Assert.assertEquals(2, fields.size());
        QField tag = fields.get(0);
        Assert.assertEquals("tag", tag.getName());
        Assert.assertEquals(1, tag.getValues().size());
        Assert.assertEquals("1234", tag.getValues().get(0).getValue1());
        Assert.assertNull(tag.getValues().get(0).getValue2());

        QField author = fields.get(1);
        Assert.assertEquals("author", author.getName());
        Assert.assertEquals(2, author.getValues().size());
        Assert.assertEquals("5678", author.getValues().get(0).getValue1());
        Assert.assertEquals("5432", author.getValues().get(1).getValue1());
    }

    @Test
    public void test2() {
        String queryString = "author:(5678 or 5432) and tag:(1234)";
        Query query = Query.toQuery(queryString);
        Assert.assertEquals("author:(5678 or 5432) and tag:(1234);", query.toString());

        List<QField> fields = query.getFields();
        QField author = fields.get(0);
        Assert.assertEquals("author", author.getName());
        Assert.assertEquals(2, author.getValues().size());
        Assert.assertEquals("5678", author.getValues().get(0).getValue1());
        Assert.assertEquals("5432", author.getValues().get(1).getValue1());

        Assert.assertEquals(2, fields.size());
        QField tag = fields.get(1);
        Assert.assertEquals("tag", tag.getName());
        Assert.assertEquals(1, tag.getValues().size());
        Assert.assertEquals("1234", tag.getValues().get(0).getValue1());
        Assert.assertNull(tag.getValues().get(0).getValue2());

    }

    @Test
    public void test3() {
        String queryString = "create_time:(5678-5432) and tag:(1234)";
        Query query = Query.toQuery(queryString);
        Assert.assertEquals("create_time:(5678-5432) and tag:(1234);", query.toString());

        List<QField> fields = query.getFields();
        QField createTime = fields.get(0);
        Assert.assertEquals("create_time", createTime.getName());
        Assert.assertEquals(1, createTime.getValues().size());
        Assert.assertEquals("5678", createTime.getValues().get(0).getValue1());
        Assert.assertEquals("5432", createTime.getValues().get(0).getValue2());

        Assert.assertEquals(2, fields.size());
        QField tag = fields.get(1);
        Assert.assertEquals("tag", tag.getName());
        Assert.assertEquals(1, tag.getValues().size());
        Assert.assertEquals("1234", tag.getValues().get(0).getValue1());
        Assert.assertNull(tag.getValues().get(0).getValue2());
    }
}
