/**
 * 
 */

package com.renren.xoa2.server;

import com.renren.xoa2.server.impl.XoaService;
import com.renren.xoa2.server.impl.XoaServiceConfig;



/**
 * @author jinchao.wen
 * @email  jinchao.wen@opi-corp.com
 * @date   2011-12-7
 */
public interface IServiceBuilder {
    
/*  public TProcessor buildProcessor();
    public TServerTransport buildTServerTransport();
    public TServer buildTServer();*/
    
    /**
     * 由XoaServiceConfig实例生成XoaService对象实例
     * @param config
     * @return
     */
    public XoaService build(XoaServiceConfig config);
}
