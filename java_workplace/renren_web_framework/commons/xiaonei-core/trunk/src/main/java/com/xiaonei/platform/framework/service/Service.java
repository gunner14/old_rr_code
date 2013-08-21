package com.xiaonei.platform.framework.service;


import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.framework.dto.DTO;
import com.xiaonei.platform.framework.exception.ServiceException;
import com.xiaonei.platform.framework.view.View;

/**
 * 
 * @author lifs
 * 
 */
public interface Service {

	public static Log logger = LogFactory.getFactory().getInstance(Service.class);
	
	/**
	 * 
	 * @param dto TODO
	 * @return
	 * @throws ServiceException
	 */
	public View doService(DTO dto) throws ServiceException;
	
	
}
