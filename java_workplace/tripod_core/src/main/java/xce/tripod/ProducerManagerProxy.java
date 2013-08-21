package xce.tripod;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import xce.tripod.util.Continuum;
import Ice.Communicator;
import Ice.InitializationData;
import Ice.Properties;

public class ProducerManagerProxy extends ZNodeListener {

    private String namespaceId;

    private String businessId;

    private List<ProducerManagerPrx> producerManagerPrxList;

    private ReentrantReadWriteLock rwLock;

    private Random random;

    private Communicator communicator;

    public ProducerManagerProxy(String ns, String biz) {
	super(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE + Constants.SEPARATOR
		+ ns);
	this.namespaceId = ns;
	this.businessId = biz;
	producerManagerPrxList = new ArrayList<ProducerManagerPrx>();
	rwLock = new ReentrantReadWriteLock();
	random = new Random(System.currentTimeMillis());
	InitializationData initData = new InitializationData();
	Properties props = Ice.Util.createProperties();
	props.setProperty("Ice.Override.Timeout", "1000");
	initData.properties = props;
	communicator = Ice.Util.initialize(initData);
	try {
	    ZkClient zkClient = ZkClient
		    .getInstance(Constants.DEFAULT_ZK_ADDRESS);
	    zkClient.addZnodeListener(this);
	    List<String> childrenNames = zkClient.getNodes(getZNode());
	    update(childrenNames);
	} catch (TripodException e) {
	    e.printStackTrace();
	}
    }

    private ProducerManagerPrx locateProducerManagerProxy(String key) {
	ReadLock rLock = rwLock.readLock();
	try {
	    rLock.lock();
	    if (producerManagerPrxList.isEmpty()) {
		return null;
	    }
	    int size = producerManagerPrxList.size();
	    int index = random.nextInt(size);
	    return producerManagerPrxList.get(index);
	} finally {
	    rLock.unlock();
	}
    }

    public byte[] produce(String key, boolean useMaster) {
	ProducerManagerPrx proxy = locateProducerManagerProxy(key);
	try {
	    if (proxy == null) {
		System.err
			.println("ProducerManagerProxy.produce() call locateProducerManagerProxy() return null proxy!"
				+ "\tnamespaceId:"
				+ namespaceId
				+ "\tbusinessId:" + businessId);
		return null;
	    }
	    return proxy.produce(key, namespaceId, businessId, true);
	} catch (Exception e) {
	    e.printStackTrace();
	}
	return null;
    }

    @Override
    public boolean update(List<String> childrenNameList) {
	WriteLock wLock = rwLock.writeLock();
	try {
	    wLock.lock();
	    Continuum cm = new Continuum(businessId);
	    List<ProducerManagerPrx> newPrxList = new ArrayList<ProducerManagerPrx>();
	    for (String childName : childrenNameList) {
//		System.out.println("childName:" + childName);
		String[] units = childName.split(":");
		if (units.length == 2) {
		    String address = units[0];
		    int port = 0;
		    try {
			port = Integer.parseInt(units[1]);
		    } catch (Exception e) {
			e.printStackTrace();
			continue;
		    }
		    StringBuffer proxyString = new StringBuffer();
		    proxyString.append("M:tcp -h ").append(address)
			    .append(" -p ").append(port).append(" -t ")
			    .append(" 1000");
		    ProducerManagerPrx proxy = ProducerManagerPrxHelper
			    .uncheckedCast(communicator
				    .stringToProxy(proxyString.toString()));
		    if (proxy != null) {
			newPrxList.add(proxy);
		    }
		}
	    }
	    if (!newPrxList.isEmpty()) {
		producerManagerPrxList.clear();
		producerManagerPrxList.addAll(newPrxList);
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	} finally {
	    wLock.unlock();
	}

	return true;
    }

    // public static void main(String[] args) {
    // String test_key = "test_key";
    // test_key = "13";
    // ProducerManagerProxy proxy = new ProducerManagerProxy(
    // "test_namespace2", "test_biz");
    // for (int i = 1; i < 100; ++i) {
    // byte[] v = proxy.produce(String.valueOf(i), true);
    // System.out.println(i + ":" + v);
    // }
    //
    // System.exit(0);
    // }
}
