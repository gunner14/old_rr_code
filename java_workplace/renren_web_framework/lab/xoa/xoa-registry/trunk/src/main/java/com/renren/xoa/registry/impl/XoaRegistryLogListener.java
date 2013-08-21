package com.renren.xoa.registry.impl;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa.registry.XoaRegistryListener;
import com.renren.xoa.registry.XoaServiceDescriptor;


public class XoaRegistryLogListener implements XoaRegistryListener {

    protected static Log logger = LogFactory.getLog(XoaRegistryLogListener.class);
    
    @Override
    public void onNodeDeleted(XoaServiceDescriptor node) {
        if (logger.isInfoEnabled()) {
            logger.info("Node deleted: " + node);
        }
    }

    @Override
    public void onNodeDisabled(XoaServiceDescriptor node) {
        if (logger.isInfoEnabled()) {
            logger.info("Node disabled: " + node);
        }
    }

}
