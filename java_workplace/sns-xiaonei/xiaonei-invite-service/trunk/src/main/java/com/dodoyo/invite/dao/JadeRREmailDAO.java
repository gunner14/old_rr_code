package com.dodoyo.invite.dao;

import net.paoding.rose.jade.annotation.DAO;

import com.xiaonei.jade.datasource.Catalogs;

@DAO (catalog = Catalogs.IMMEDIATE_EMAIL)
public interface JadeRREmailDAO extends JadeEmailDAO
{
}
