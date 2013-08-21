package com.xiaonei.jade.test;

import net.paoding.rose.scanning.context.RoseAppContext;

import com.xiaonei.jade.test.dao.EntryDAO;

public class Main {

    public static void main(String[] args) {
        RoseAppContext ctx = new RoseAppContext();
        EntryDAO dao = ctx.getBean(EntryDAO.class);
        System.out.println(dao.get(22000));
    }
}
