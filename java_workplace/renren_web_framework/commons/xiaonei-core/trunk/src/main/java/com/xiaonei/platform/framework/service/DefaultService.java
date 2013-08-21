package com.xiaonei.platform.framework.service;

import com.xiaonei.platform.framework.dto.DTO;
import com.xiaonei.platform.framework.dto.XnDto;
import com.xiaonei.platform.framework.exception.ServiceException;
import com.xiaonei.platform.framework.view.View;
import com.xiaonei.platform.framework.view.XnView;

/**
 * 
 * @author lifs
 * 
 */
public abstract class DefaultService extends XnService{

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.xiaonei.platform.framework.service.Service#doService(com.xiaonei.platform.framework.dto.DTO)
	 */
	public View doService(DTO dto) throws ServiceException {
		return _doService((XnDto) dto);
	}

	protected abstract XnView _doService(XnDto dto) throws ServiceException;
}
