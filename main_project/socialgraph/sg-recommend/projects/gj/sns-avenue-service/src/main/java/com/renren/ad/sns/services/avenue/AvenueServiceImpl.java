package com.renren.ad.sns.services.avenue;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;

import com.renren.ad.sns.services.avenue.protocol.AvenueBlob;
import com.renren.ad.sns.services.avenue.protocol.AvenueService;
import com.renren.ad.sns.services.avenue.protocol.IndexType;
import com.renren.ad.sns.util.thrift.ThriftServerImplementor;
import com.renren.ad.util.container.SequenceQueue;

/**
 * @author xiong.hua
 */
public class AvenueServiceImpl extends ThriftServerImplementor implements AvenueService.Iface {

    private static final String DEFAULT_INX_PATH = "gj.avenue";

    private static final Log LOG = LogFactory.getLog(AvenueServiceImpl.class);

    // Avenue build utility
    private AvenueBuilder avenueBuilder;

    // target Avenue object, updated every certain interval
    //    private AvenueIndex avenueIndex;

    private Map<IndexType, AvenueIndex> avenueIndexes = new HashMap<IndexType, AvenueIndex>();

    private long buildInterval = 5 * 60 * 1000;

    protected TProcessor createProcessor() {
        return new AvenueService.Processor(this);
    }

    // Avenue feed register
    public void regist(String host, int port, long version) throws TException {
        Map<IndexType, Long> versions = new HashMap<IndexType, Long>();
        for (IndexType type : IndexType.values()) {
            versions.put(type, 0L);
        }
        AvenueFeedStub server = new AvenueFeedStub(host, port, versions);

        synchronized (subs) {
            if (!subs.contains(server)) {
                // try to push index at register time
                if (avenueIndexes != null && avenueIndexes.size() > 0) {
                    // old version
//                    AvenueIndex index = avenueIndexes.get(IndexType.NEWEST);
//                    if (index != null) {
//                        server.toAvenueFeedProxy().push(index.getAvenueBlob());
//                    }
// 
                    // new version
                	LOG.info("regist: avenueIndexes not null && size > 0");
                    for (IndexType type : IndexType.values()) {
                        AvenueIndex index = avenueIndexes.get(type);
                        if (index != null) {
                            server.toAvenueFeedProxy().pushIndex(index.getAvenueBlob(), type);
                        }
                    }
                }else{
                	LOG.info("regist: avenueIndexes null or size == 0");
                }
                subs.add(server);
                LOG.info(server + " registed");
            }
        }
    }

    public AvenueBlob poll(String host) throws TException {
        return pullIndex(host, IndexType.NEWEST);
    }

    // feed would first try to download newest version of Avenue
    public AvenueBlob pullIndex(String host, IndexType indexType) throws TException {
        if (avenueIndexes != null && avenueIndexes.size() > 0) {
            AvenueIndex index = avenueIndexes.get(indexType);
            if (index != null) {
                LOG.info(host + " is downloading " + indexType + " index.");
                return index.getAvenueBlob();
            }
        }
        return null;
    }

    // Avenue feed stub holds reference to remote Avenue server
    private class AvenueFeedStub {

        String host;

        int port;

        Map<IndexType, Long> idxVersion;

        public String toString() {
            return "server(" + host + ":" + port + ")";
        }

        public AvenueFeedStub(String host, int port, Map<IndexType, Long> idxVersion) {
            this.host = host;
            this.port = port;
            this.idxVersion = idxVersion;
        }

        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + getOuterType().hashCode();
            result = prime * result + ((host == null) ? 0 : host.hashCode());
            result = prime * result + port;
            return result;
        }

        public boolean equals(Object obj) {
            if (this == obj) return true;
            if (obj == null) return false;
            if (getClass() != obj.getClass()) return false;
            AvenueFeedStub other = (AvenueFeedStub) obj;
            if (!getOuterType().equals(other.getOuterType())) return false;
            if (host == null) {
                if (other.host != null) return false;
            } else if (!host.equals(other.host)) return false;
            if (port != other.port) return false;
            return true;
        }

        private AvenueServiceImpl getOuterType() {
            return AvenueServiceImpl.this;
        }

        public AvenueFeedProxy toAvenueFeedProxy() {
            return new AvenueFeedProxy(host, port);
        }
    }

    // push newest version of Avenue to every Avenue feed
    private SequenceQueue<AvenueFeedStub> uploaderQueue = new SequenceQueue<AvenueFeedStub>() {

        protected void handle(AvenueFeedStub server) {

            for (IndexType indexType : IndexType.values()) {
                AvenueIndex avenueIdx = avenueIndexes.get(indexType);
                if (avenueIdx != null) {
                    synchronized (server) {
                        long version = avenueIdx.getVersion();
                        if (server.idxVersion.get(indexType) != null
                                && server.idxVersion.get(indexType) == version) {
                            /*LOG.info(server + " already has newset version of "
                                    + indexType + " index");*/
                            continue;
                        }
                        AvenueFeedProxy proxy = null;
                        try {
                            // old version
//                            if (indexType == IndexType.NEWEST) {
//                                proxy = new AvenueFeedProxy(server.host, server.port);
//                                proxy.push(avenueIdx.getAvenueBlob());
//                                server.idxVersion.put(indexType, version);
//                                LOG.info("upload " + indexType + "[" + version + "] to " + server);
//                            } 

                            // new version
                             proxy = new AvenueFeedProxy(server.host, server.port);
                             proxy.pushIndex(avenueIdx.getAvenueBlob(), indexType);
                             server.idxVersion.put(indexType, version); 
                             LOG.info("upload " + indexType + "[" + version + "] to " + server);
                        } catch (TException e) {
                            LOG.error("upload " + indexType + "[" + version + "] to " + server
                                    + " failed.", e);
                            subs.remove(server);
                        } finally {
                            if (proxy != null) {
                                proxy.cleanUp();
                            }
                        }
                    }
                }
            }
        }
    };

    private List<AvenueFeedStub> subs = new ArrayList<AvenueFeedStub>();

    public AvenueServiceImpl() {
        // first check if any previous built Avenue exists, if any load it to
        // memory first
        /*try {
            avenueIndex = new AvenueWriter()
                    .readIndexFromFile(DEFAULT_INX_PATH);
        } catch (Exception e) {
            LOG.info("local Avenue not found : " + DEFAULT_INX_PATH);
        }*/

        uploaderQueue.start();

        // build Avenue at certain interval
        new Timer().schedule(new TimerTask() {

            public void run() {
            	LOG.info("create aveunue running");
                Map<IndexType, AvenueIndex> avenue = createAvenue();
                for (IndexType type : avenue.keySet()) {
                    if (avenue.get(type) != null) {
                        LOG.info(type + " index size = "
                                + (avenue.get(type).getAvenueBlob().getBlob().length) + " byte");
                      //  avenue.get(type).setVersion(System.currentTimeMillis());
                        // update new index to the avenueIndexes
                        avenueIndexes.put(type, avenue.get(type));
                    } else {
                        LOG.warn(type + " index not exist.");
                    }
                }
            }
        }, 1000, buildInterval);

        // push Avenue to feed at certain interval
        new Timer().schedule(new TimerTask() {

            public void run() {
                checkAndUpload();
            }
        }, 0, 5000);
    }

    /**
     * 检查各个索引服务器是否需要更新
     */
    private void checkAndUpload() {
        LOG.info(subs.size());
        for (AvenueFeedStub server : subs) {
            // 检查每种类型的索引，若新索引存在，则加入更新队列
            for (IndexType indexType : IndexType.values()) {
                AvenueIndex avenueIdx = avenueIndexes.get(indexType);
                if (avenueIdx != null && server.idxVersion.get(indexType) < avenueIdx.getVersion()) {
                    uploaderQueue.addLast(server);
                    break;
                }
            }
        }
    }

    private Map<IndexType, AvenueIndex> createAvenue() {
        try {
            return avenueBuilder.createIndex();
        } catch (Exception e) {
            LOG.error("Create Avenue failed.", e);
            return null;
        }
    }

    public void setAvenueBuilder(AvenueBuilder AvenueBuilder) {
        this.avenueBuilder = AvenueBuilder;
    }

    public long getBuildInterval() {
        return buildInterval;
    }

    public void setBuildInterval(long buildInterval) {
        this.buildInterval = buildInterval;
    }
}
