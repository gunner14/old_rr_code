package com.dodoyo.invite.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.jade.datasource.Catalogs;


@DAO (catalog = Catalogs.IDSEQUENCE)
public interface JadeRRSequenceDAO extends JadeSequenceDAO
{
    //invite_url_paras_id_seq
    @SQL("select nextval('invite_url_paras_id_seq')")
    public int getSequenceId();
}
