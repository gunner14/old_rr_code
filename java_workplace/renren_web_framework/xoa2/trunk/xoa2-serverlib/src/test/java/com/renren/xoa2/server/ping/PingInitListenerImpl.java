/**
 * 
 */

package com.renren.xoa2.server.ping;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.server.IInitListener;
import com.renren.xoa2.server.impl.XoaService;


/**
 * @author jinchao.wen
 * @email  jinchao.wen@opi-corp.com
 * @date   2011-12-8
 */
public class PingInitListenerImpl implements IInitListener {
    
    private static Logger logger = LoggerFactory.getLogger(PingInitListenerImpl.class);

    /* (non-Javadoc)
     * @see com.renren.xoa2.server.IInitListener#update(java.lang.Object, java.lang.Object)
     */
    @Override
    public void update(Object o, Object arg) {
    	
        logger.info(((XoaService)o).getConfig() + ", " + arg);
    }

}
