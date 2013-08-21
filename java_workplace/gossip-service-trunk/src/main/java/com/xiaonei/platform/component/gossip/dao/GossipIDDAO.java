package com.xiaonei.platform.component.gossip.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.jade.datasource.Catalogs;

/**
 *
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 *
 */
@DAO(catalog = Catalogs.IDSEQUENCE)
//@DAO
public interface GossipIDDAO {
	@SQL("select nextval('gossip_id_seq'::text)")
	public long getNext() ;
}
