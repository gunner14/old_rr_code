package com.xiaonei.platform.core.opt;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.Logger;

/**
 * 
 * Op
 */
public abstract class Op {
    protected Log logger = LogFactory.getLog(this.getClass());
    
	protected String sql;

	public final String getSql() {
	    if (logger.isDebugEnabled()) {
            logger.debug("getSQL: " + sql);
        }
		return sql;
	}
	
	/** 多功能参数,不要改变这个初始值 */
	protected int multiPurposeParam = -1;

	public int getMultiPurposeParam() {
		return multiPurposeParam;
	}

	private int photoDatabaseNumber;

	public int getPhotoDatabaseNumber() {
		return photoDatabaseNumber;
	}

	public void setPhotoDatabaseNumber(int photoDatabaseNumber) {
		this.photoDatabaseNumber = photoDatabaseNumber;
	}
	/**业务名*/
	protected String bizName ;
	/**数据库表名*/
	protected String dbTableName;
	
	/**散表表名的后缀，如gossip_2,应该传入 2*/
	protected int tableSuffix = -1;

	public int getTableSuffix() {
		return tableSuffix;
	}
	public String getBizName() {
		return bizName;
	}

	public final void log(Log logger) {
		if (logger.isDebugEnabled()) {
			logger.debug(getSql());
		}
	}
	public void log(Logger log) {
		if (log.isDebugEnabled()) {
			log.debug(getSql());
		}
	}

}