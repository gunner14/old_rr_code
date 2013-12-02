package com.renren.xoa2.quota;

import java.lang.reflect.Method;

import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.client.ServiceInvocationHandler;
import com.renren.xoa2.client.ServiceRouter;
import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.logprofile.collector.Chunk;
import com.renren.xoa2.logprofile.collector.ILogCollectService;
import com.renren.xoa2.logprofile.collector.LogUtil;
import com.renren.xoa2.quota.checker.QuotaChecker;
import com.renren.xoa2.quota.exception.XoaQuotaException;

/**
 * 集成Quota Service 的 InvocationHandler
 * 
 * @author Xue Wenxin Nov 12, 2012 3:08:34 PM
 */
public class QuotaLimitedServiceInvocationHandler extends ServiceInvocationHandler {

    /** xoa的业务类型 */
    private static final String XOA_BUSINESS_TYPE = "XOA-Client";

    private static org.slf4j.Logger logger = org.slf4j.LoggerFactory
            .getLogger(QuotaLimitedServiceInvocationHandler.class);

    private QuotaChecker quotaChecker;

    private ILogCollectService collectorService;

    public QuotaLimitedServiceInvocationHandler(ServiceRouter serviceRouter,
            ClassDefinition serviceDefinition, QuotaChecker quotaChecker,
            ILogCollectService collectorService) {
        super(serviceRouter, serviceDefinition);
        this.quotaChecker = quotaChecker;
        this.collectorService = collectorService;
    }

    @Override
    protected void beforeInvoke(Object proxy, Method method, Object[] args) {
        // 检查配额
        if (!quotaChecker.hasQuota()) {
            throw new XoaQuotaException("achieve the quota!! serviceId:"
                    + getServiceDefinition().getServiceId());
        }
    }

    @Override
    protected Object round(Object proxy, Method method, Object[] args) throws Throwable {
        long startTime = System.currentTimeMillis();
        try {
            beforeInvoke(proxy, method, args);
            return doInvoke(proxy, method, args);
        } finally {
            String userName = XcsConfigurations.getConfiguration().getClientUsername();
            long timestamp = System.currentTimeMillis();
            Chunk chunk = new Chunk();
            chunk.setBusinessType(XOA_BUSINESS_TYPE);
            chunk.setData(LogUtil.build(timestamp,
                    super.getServiceDefinition().getServiceId(), userName, "costtime", 
                    "", "", "", System.currentTimeMillis() - startTime));
            try {
                collectorService.addLog(chunk);
            } catch (Exception e) {
                logger.error("Fail to call ILogCollectService.addLog(Chunk) in xoa client invocation : "
                        + e.getMessage());
            }
        }
    }

    @Override
    protected void onFrameworkException(Throwable e) {
        Chunk chunk = new Chunk();
        chunk.setBusinessType(XOA_BUSINESS_TYPE);
        chunk.setData(LogUtil.build(System.currentTimeMillis(),
                super.getServiceDefinition().getServiceId(),
                XcsConfigurations.getConfiguration().getClientUsername(), "exception",
                "", "", "", e));
        try {
            collectorService.addLog(chunk);
        } catch (Exception ee) {
            logger.error("Fail to call ILogCollectService.addLog(Chunk) in xoa client invocation : "
                    + ee.getMessage());
        }
    }

}
