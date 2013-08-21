/**
 * 
 */

package com.renren.xoa2.server.conf;


/**
 * @author jinchao.wen
 * @email  jinchao.wen@opi-corp.com
 * @date   2011-12-8
 */
public interface IConfigurable {
    
    void setConf(Configuration  conf);
    Configuration getConf();
}
