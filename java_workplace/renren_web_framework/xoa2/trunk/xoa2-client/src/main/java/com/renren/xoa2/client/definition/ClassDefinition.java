package com.renren.xoa2.client.definition;

import java.lang.reflect.Constructor;

import org.apache.thrift.protocol.TProtocol;

import com.renren.xoa2.annotation.XoaService;

/**
 * 接口类相关信息
 * 
 * @author yanghe.liang
 * @since 2011-12-12 上午01:15:09
 * 
 * 
 * @author xun.dai@renren-inc.com 2012-05-10 
 */
public class ClassDefinition {

    private String serviceId;

    private Class<?> serviceClientClass;

    private Constructor<?> serviceClientConstructor;
    
    public ClassDefinition(Class<?> serviceInterface) throws ClassNotFoundException, SecurityException, NoSuchMethodException{
        String clientClassName = resolveClientClassName(serviceInterface);
        this.serviceClientClass = Class.forName(clientClassName);
        this.serviceId = revolveServiceId(serviceInterface);
        this.serviceClientConstructor = serviceClientClass.getConstructor(TProtocol.class);
    }
    
    private String resolveClientClassName(Class<?> serviceClass) {
        String packageName = serviceClass.getPackage().getName();
        String simpleClassName = serviceClass.getSimpleName();
        simpleClassName = simpleClassName.substring(1); // remove heading I
        return packageName + "." + simpleClassName + "$Client";
    }

    private String revolveServiceId(Class<?> serviceClass) {
        XoaService xoaService = serviceClass.getAnnotation(XoaService.class);
        return xoaService != null ? xoaService.value().trim() : "";
    }

    
    public String getServiceId() {
        return serviceId;
    }
    
    public Class<?> getServiceClientClass() {
        return serviceClientClass;
    }

    public Constructor<?> getServiceClientConstructor() {
        return serviceClientConstructor;
    }

}
