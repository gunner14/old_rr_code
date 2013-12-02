package xce.tripod;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import Ice.Current;

import com.xiaonei.xce.log.TimeCost;

/**
 * ProducerManagerImpl实现
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class ProducerManagerImpl extends _ProducerManagerDisp {

    private static final long serialVersionUID = 1L;

    private static final Log log = LogFactory.getLog("oce_log");

    @Override
    public byte[] produce(String key, String namespaceId, String businessId, boolean useMaster,
            Current current) {
        TimeCost tc = TimeCost.begin(log, "ProducerManagerImpl.produce() key:" + key + "\tns:"
                + namespaceId + "\tbiz:" + businessId + "\tuseMaster:" + useMaster);
        Producer producer = ProducerFactory.getInstance().get(businessId);
        if (producer == null) {
            System.err
                    .println("ProducerManagerImpl.produce() ProducerFactory.getInstance().get() return null Producer biz:"
                            + businessId);
            return null;
        }
        byte[] value = null;
        try {
            value = producer.create(key, useMaster);
        } catch (Exception e) {
            e.printStackTrace();
        }
        tc.endFinally();
        return value;
    }

    @Override
    public String ping(Current arg0) {
        return "PONG";
    }

}
