package com.dodoyo.invite.dao;

import net.paoding.rose.jade.annotation.DAO;

import com.xiaonei.jade.datasource.Catalogs;


@DAO (catalog = Catalogs.INVITE_LETTER)
public interface JadeKXEmailDAO extends JadeEmailDAO
{
}
