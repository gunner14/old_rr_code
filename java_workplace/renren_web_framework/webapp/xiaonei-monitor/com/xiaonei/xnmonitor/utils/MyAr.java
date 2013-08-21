package com.xiaonei.xnmonitor.utils;

import net.paoding.rose.ar.Ar;

import org.hibernate.SessionFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class MyAr  extends Ar {

    static {
        ApplicationContext arConfig = new ClassPathXmlApplicationContext(
                "applicationContext-ar.xml");
        Ar.setSessionFactory((SessionFactory) arConfig.getBean("sessionFactory"));

    }

    public MyAr() {
    }

}
