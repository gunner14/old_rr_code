/**   
 * @ InterceptorClass.java Create on 2011-12-16 下午5:56:45   
 *     
 */   
package com.renren.xoa2.client.filter;   
  
/**   
 * @author gang.pan 
 * @mail gang.pan@renren-inc.com 
 * @version 1.0   
 * 公用的拦截器方法 用于监控service调用时间和并发量 
 * 
**/ 
public class InterceptorClass { 
    public void before(){ 
        System.out.println("拦截器InterceptorClass方法调用:before()!"); 
    } 
    public void after(){ 
        System.out.println("拦截器InterceptorClass方法调用:after()!"); 
    } 
}