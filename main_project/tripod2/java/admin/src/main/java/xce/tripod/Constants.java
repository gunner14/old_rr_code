package xce.tripod;

public class Constants {
	
    static final String DEFAULT_ZK_ADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
    //static final String DEFAULT_ZK_ADDRESS = "10.3.22.25:2181";
    static final String CONFIG_DIR = "/data/xce/CommonCache/etc/";
    //static final String CONFIG_DIR = "/data/xce/CommonCacheTest/etc/";
    //static final String CONFIG_DIR = "/home/lili/";

    static final String DEFAULT_PREFIX = "/tripod2";

    static final String SEPARATOR = "/";

    static final String DEFAULT_ZK_CACHE_ZNODE = DEFAULT_PREFIX + SEPARATOR + "cache";

    static final String DEFAULT_ZK_CACHEMANAGERZNODE = DEFAULT_PREFIX + SEPARATOR + "cachemanager";

    static final String DEFAULT_ZK_PRODUCERMANAGER_ZNODE = DEFAULT_PREFIX + SEPARATOR
            + "producermanager";

    static final String CUTOFF = "CUTOFF";

    static final String ADDON = "ADDON";
}
