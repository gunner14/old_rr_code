package com.renren.xoa2.quota;

import java.lang.reflect.InvocationHandler;

import com.renren.xcs.FindServiceRegistryFactory;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xcs.ServiceRegistryFactory;
import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.client.DefaultServiceFactory;
import com.renren.xoa2.client.IServiceFactory;
import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.logprofile.collector.Chunk;
import com.renren.xoa2.logprofile.collector.ILogCollectService;
import com.renren.xoa2.quota.checker.QuotaChecker;

/**
 * 封装创建Service实例的逻辑，缓存Service的Proxy实例，IServiceFactory的集成配额检查的默认实现。
 * 
 * @author Xue Wenxin Nov 12, 2012 2:51:41 PM
 */
public class QuotaLimitedServiceFactory extends DefaultServiceFactory {

    private ServiceRegistryFactory findServiceRegistryFactory;

    private QuotaCheckerFactory quotaCheckerFactory;

    private IServiceFactory serviceFactory;

    public QuotaLimitedServiceFactory() {
        super();
        this.findServiceRegistryFactory = new FindServiceRegistryFactory();
        this.serviceFactory = new DefaultServiceFactory();
        this.quotaCheckerFactory = new QuotaCheckerFactory();
    }

    @Override
    protected InvocationHandler createInocationHandler(ClassDefinition serviceDefinition) {
        QuotaChecker quotaChecker;
        ILogCollectService collectorService;
        if (XcsConfigurations.isHostsBinded() || "test".equals(XcsConfigurations.getConfiguration().getRoot())) {
            // 如果绑定hosts，不检查Quota
            quotaChecker = new QuotaChecker() {

                @Override
                public boolean hasQuota() {
                    return true;
                }
            };
            // 如果绑定hosts，不记录
            collectorService = new ILogCollectService() {

                @Override
                public void addLog(Chunk chunk) {

                }
            };
        } else {
            ServiceRegistryAccessor accessor = findServiceRegistryFactory
                    .getServiceRegistryAccessor(XcsConfigurations.getConfiguration(),
                            serviceDefinition.getServiceId(), serviceDefinition.getVersion());
            quotaChecker = quotaCheckerFactory.getQuotaChecker(accessor,
                    XcsConfigurations.getConfiguration(), serviceFactory);
            collectorService = serviceFactory.getService(ILogCollectService.class, 100);
        }
        return new QuotaLimitedServiceInvocationHandler(getServiceRouter(), serviceDefinition,
                quotaChecker, collectorService);
    }

}
