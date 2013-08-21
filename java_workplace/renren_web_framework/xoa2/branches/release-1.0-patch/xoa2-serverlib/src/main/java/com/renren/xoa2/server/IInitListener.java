/**
 * 
 */

package com.renren.xoa2.server;





/**
 * 服务启动监听器，用户可自己实现，并注册到xoa.xml文件中
 * 
 * @author jinchao.wen
 * @email  jinchao.wen@opi-corp.com
 * @date   2011-12-7
 */
public interface  IInitListener {
    
    /**
     * xoa服务启动时将调用此方法
     * 
     * @param o 
     * @param arg
     */
    public void update( Object o, Object arg) ;
}
