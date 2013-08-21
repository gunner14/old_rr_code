/**   
 * @ DynaProxyNode.java Create on 2011-12-8 下午5:22:24   
 *     
 */   
package com.renren.xoa2.client.proxyaop;   
  
/**   
 * @author gang.pan 
 * @mail gang.pan@renren-inc.com 
 * @version 1.0   
 */
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class DynaProxyNode implements InvocationHandler {
    /**
     * 操作者
     */
    private Object proxy;
    /**
     * 要处理的对象(也就是我们要在方法的前后加上业务逻辑的对象,如例子中的Hello)
    */
    private Object delegate;

    /**
     * 动态生成方法被处理过后的对象 (写法固定)
     * 
     * @param delegate
    * @param proxy
     * @return
     */
    public Object bind(Object delegate,Object proxy) {
        
        this.proxy = proxy;
        this.delegate = delegate;
        return Proxy.newProxyInstance(
                this.delegate.getClass().getClassLoader(), this.delegate
                        .getClass().getInterfaces(), this);
    }
    /**
   * 要处理的对象中的每个方法会被此方法送去JVM调用,也就是说,要处理的对象的方法只能通过此方法调用
     * 此方法是动态的,不是手动调用的
    */
    public Object invoke(Object proxy, Method method, Object[] args)
            throws Throwable {
       Object result = null;
        try {
            //反射得到操作者的实例
            Class clazz = this.proxy.getClass();
            //反射得到操作者的Start方法
            Method start = clazz.getDeclaredMethod("start",
                    new Class[] { Method.class });
            //反射执行start方法
            start.invoke(this.proxy, new Object[] { method });
            //执行要处理对象的原本方法
            result = method.invoke(this.delegate, args);
//            反射得到操作者的end方法
            Method end = clazz.getDeclaredMethod("end",
                    new Class[] { Method.class });
//            反射执行end方法
            end.invoke(this.proxy, new Object[] { method });

        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
}
  