package com.xiaonei.jade.test.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.jade.test.Entry;

@DAO(catalog = "entry")
public interface EntryDAO {

    @SQL("select * from entry_info where id=:1")
    public Entry get(long gid);
}
