package xce.tripod;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import xce.tripod.util.Continuum;
import Ice.Communicator;
import Ice.InitializationData;
import Ice.Properties;

public class CacheManagerProxy extends ZNodeListener {

    private String namespaceId;

    private String businessId;

    private Continuum continuum;

    private Map<String, CacheManagerPrx> cacheManagerPrxMap;

    private ReentrantReadWriteLock rwLock;

    private Communicator communicator;
    
    private int connTimeOutInMilliseconds;

    public CacheManagerProxy(String ns, String biz, int connTimeOutInMilliseconds) {
        super(Constants.DEFAULT_ZK_CACHEMANAGERZNODE + Constants.SEPARATOR + ns);
        this.namespaceId = ns;
        this.businessId = biz;
        this.connTimeOutInMilliseconds = connTimeOutInMilliseconds;
        continuum = new Continuum(biz);
        cacheManagerPrxMap = new HashMap<String, CacheManagerPrx>();
        rwLock = new ReentrantReadWriteLock();
        InitializationData initData = new InitializationData();
        Properties props = Ice.Util.createProperties();
        props.setProperty("Ice.Override.Timeout", String.valueOf(connTimeOutInMilliseconds));
        initData.properties = props;
        communicator = Ice.Util.initialize(initData);
        try {
            ZkClient zkClient = ZkClient.getInstance(Constants.DEFAULT_ZK_ADDRESS);
            zkClient.addZnodeListener(this);
            List<String> childrenNames = zkClient.getNodes(getZNode());
            update(childrenNames);
        } catch (TripodException e) {
            e.printStackTrace();
        }
    }

    private CacheManagerPrx locateCacheManagerProxy(String key) {
        String nickname = continuum.locate(Continuum.hash(key));
        ReadLock rLock = rwLock.readLock();
        try {
            rLock.lock();
            return cacheManagerPrxMap.get(nickname);
        } finally {
            rLock.unlock();
        }
    }

    private String description(String desc) {
        StringBuffer sb = new StringBuffer();
        sb.append("[ns:").append(namespaceId).append(",biz:").append(businessId);
        if (desc != null) {
            sb.append(", msg:").append(desc);
        }
        sb.append("]");
        return sb.toString();
    }

    @Override
    public boolean update(List<String> childrenNameList) {
        WriteLock wLock = rwLock.writeLock();
        try {
            wLock.lock();
            Continuum cm = new Continuum(businessId);
            Map<String, CacheManagerPrx> newPrxMap = new HashMap<String, CacheManagerPrx>();
            for (String childName : childrenNameList) {
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
                    proxyString.append("M:tcp -h ").append(address).append(" -p ").append(port)
                            .append(" -t  ").append(connTimeOutInMilliseconds);
                    CacheManagerPrx proxy = CacheManagerPrxHelper.uncheckedCast(communicator
                            .stringToProxy(proxyString.toString()));
                    if (proxy != null) {
                        newPrxMap.put(childName, proxy);
                        cm.add(childName, 100);
                    }

                }
            }
            if (!newPrxMap.isEmpty()) {
                cm.rebuild();
                continuum = cm;
                cacheManagerPrxMap.clear();
                cacheManagerPrxMap.putAll(newPrxMap);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            wLock.unlock();
        }

        return true;
    }

    /**
     * remove接口
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    public boolean remove(String key) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.remove() call locateCacheManagerProxy return NULL prx!");
            return false;
        }
        try {
            return proxy.remove(key, namespaceId, businessId);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * get 接口<br/>
     * secs为过期时间， 如果secs等于0,则永远不过期 useMaster 是否有主库
     * 
     * @param key
     * @param secs
     * @param useMaster
     * @return
     * @throws TripodException
     */
    public byte[] get(String key, int secs, boolean useMaster) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.get() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        try {
            return proxy.get(key, namespaceId, businessId, secs, useMaster);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * load 接口 <br/>
     * 
     * secs为过期时间， 如果secs等于0,则永远不过期 useMaster 是否有主库
     * 
     * @param key
     * @param secs
     * @param useMaster
     * @return
     * @throws TripodException
     */
    public boolean load(String key, int secs, boolean useMaster) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.load() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.load(key, namespaceId, businessId, secs, useMaster);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * set 接口<br/>
     * secs为过期时间， 如果secs等于0,则永远不过期 useMaster 是否有主库
     * 
     * @param key
     * @param secs
     * @return
     * @throws TripodException
     */
    public boolean set(String key, byte[] value, int secs) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.set() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.set(key, value, namespaceId, businessId, secs);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * inc 接口
     * 
     * @param step 增量，可以是负数，值域为long
     * @return 成功返回inc后的String值，失败返回空字符串
     * @throws TripodException
     */
    public String inc(String key, String step) throws TripodException {
        String result = "";
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.inc() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return result;
        }
        try {
            return proxy.inc(key, step, namespaceId, businessId);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return result;
        }
    }
    
    /**
     * getList 接口
     * 
     * @param key
     * @param begin
     *            支持分段选取的开始位置
     * @param limit
     *            选取大小，为负值时返回到结尾位置
     * @param secs
     * @param useMaster
     * @return
     */
    public List<String> getList(String key, int begin, int limit, int secs,
	    boolean useMaster) throws TripodException {
	List<String> result = new LinkedList<String>();
	CacheManagerPrx proxy = locateCacheManagerProxy(key);
	if (proxy == null) {
	    System.err
		    .println("CacheManagerProxy.getList() call locateCacheManagerProxy return NULL prx!"
			    + "\tnamespaceId:"
			    + namespaceId
			    + "\tbusinessId:"
			    + businessId);
	    return null;
	}
	try {
	    result.addAll(Arrays.asList(proxy.getList(key, namespaceId,
		    businessId, begin, limit, secs, useMaster)));
	} catch (Ice.TimeoutException e) {
		throw new TripodException(description(e.getMessage()), e);
	} catch (Exception e) {
	    e.printStackTrace();
	    return null;
	}
	return result;
    }
    
    /**
     * loadList 接口
     * 
     * @param key
     * @param secs
     * @param useMaster
     * @return
     */
    public boolean loadList(String key, int secs, boolean useMaster) throws TripodException {
	CacheManagerPrx proxy = locateCacheManagerProxy(key);
	if (proxy == null) {
	    System.err
		    .println("CacheManagerProxy.loadList() call locateCacheManagerProxy return NULL prx!"
			    + "\tnamespaceId:"
			    + namespaceId
			    + "\tbusinessId:"
			    + businessId);
	    return false;
	}
	try {
	    return proxy
		    .loadList(key, namespaceId, businessId, secs, useMaster);
	} catch (Ice.TimeoutException e) {
		throw new TripodException(description(e.getMessage()), e);
	} catch (Exception e) {
	    e.printStackTrace();
	    return false;
	}
    }
    
    /**
     * setList接口
     * @param key
     * @param value
     * @param secs
     * @return
     * @throws TripodException
     */
    public boolean setList(String key, List<String> value, int secs) throws TripodException {
    	CacheManagerPrx proxy = locateCacheManagerProxy(key);
    	if (proxy == null) {
    	    System.err
    		    .println("CacheManagerProxy.setList() call locateCacheManagerProxy return NULL prx!"
    			    + "\tnamespaceId:"
    			    + namespaceId
    			    + "\tbusinessId:"
    			    + businessId);
    	    return false;
    	}
    	try {
    	    return proxy.setList(key, value.toArray(new String[0]),
    		    namespaceId, businessId, secs);
    	} catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
    	    e.printStackTrace();
    	    return false;
    	}
        }

    /**
     * getAllHash接口
     * @param key
     * @param secs
     * @param useMaster
     * @return
     */
    public Map<String, String> getAllHash(String key, int secs, boolean useMaster) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.getAllHash() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        try {
            return proxy.getAllHash(key, namespaceId, businessId, secs, useMaster);
        } catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * setPartHash接口
     * @param key
     * @param hash_value
     * @return
     */
    public boolean setPartHash(String key, Map<String, String> hash_value) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.setPartHash() call locateCacheManagerProxy return null prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.setPartHash(key, hash_value, namespaceId, businessId);
        } catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * loadHash接口
     * @param key
     * @param secs
     * @param useMaster
     * @return
     */
    public boolean loadHash(String key, int secs, boolean useMaster) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.loadHash() call locateCacheManagerProxy return NULL prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.loadHash(key, namespaceId, businessId, secs, useMaster);
        } catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * deletePartHash接口
     * @param key
     * @param hash_fields
     * @return
     */
    public boolean deletePartHash(String key, String[] hash_fields) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.deletePartHash() call locateCacheManagerProxy return null prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.deletePartHash(key, hash_fields, namespaceId, businessId);
        } catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
    
    /**
     * zadd接口
     * @param key
     * @param set_with_scores
     * @return
     * @throws TripodException
     */
    public boolean zAdd(String key, Map<String, String> set_with_scores) throws TripodException {
    	CacheManagerPrx proxy = locateCacheManagerProxy(key);
    	if (proxy == null) {
    		System.err
            .println("CacheManagerProxy.zAdd() call locateCacheManagerProxy return null prx!"
                    + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
    		return false;
    	}
    	try{
    		return proxy.zAdd(key, namespaceId, businessId, set_with_scores);
    	} catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
    		e.printStackTrace();
    		return false;
    	}
    }
    
    /**
     * zSet接口
     * @param key
     * @param set_with_scores
     * @param secs
     * @return
     * @throws TripodException
     */
    public boolean zSet(String key, Map<String, String> set_with_scores, int secs)
            throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err
                    .println("CacheManagerProxy.zset() call locateCacheManagerProxy return null prx!"
                            + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.zSet(key, namespaceId, businessId, set_with_scores, secs);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
    
    /**
     * zRemRangeByRank接口
     * @param key
     * @param begin
     * @param end
     * @return
     * @throws TripodException
     */
    public boolean zRemRangeByRank(String key, int begin, int end) throws TripodException {
    	CacheManagerPrx proxy = locateCacheManagerProxy(key);
    	if (proxy == null) {
    		System.err.println("CacheManagerProxy.zAdd() call locateCacheManagerProxy return null prx!"
                    + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
    		return false;
    	}
    	try {
    		return proxy.zRemRangeByRank(key, namespaceId, businessId, begin, end);
    	} catch (Ice.TimeoutException e) {
    		throw new TripodException(description(e.getMessage()), e);
    	} catch (Exception e) {
    		e.printStackTrace();
    		return false;
    	}
    }
    
    /**
     * zRemRangeByScore 接口
     * @param key
     * @param min
     * @param max
     * @return
     * @throws TripodException
     */
    public boolean zRemRangeByScore(String key, String min, String max) throws TripodException {
        CacheManagerPrx proxy = locateCacheManagerProxy(key);
        if (proxy == null) {
            System.err.println("CacheManagerProxy.zRemRangeByScore() call locateCacheManagerProxy return null prx!"
                    + "\tnamespaceId:" + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        try {
            return proxy.zRemRangeByScore(key, namespaceId, businessId, min, max);
        } catch (Ice.TimeoutException e) {
            throw new TripodException(description(e.getMessage()), e);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    // public static void main(String[] args) {
    // BasicConfigurator.configure();
    // String test_key = "test_key";
    // CacheManagerProxy proxy = new CacheManagerProxy("2test_namespace",
    // "test_biz");
    // System.out.println(proxy.load(test_key, 0, true));
    // System.out.println(proxy.get(test_key, 0, true));
    // System.out.println(proxy.remove(test_key));
    // System.out.println(proxy.set(test_key, "fuck world".getBytes(), 100));
    // System.exit(0);
    // }
}
