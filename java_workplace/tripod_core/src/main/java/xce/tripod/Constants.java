package xce.tripod;

public interface Constants {

    String VERSION = "#2.1.0";

    String DEFAULT_ZK_ADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";

    final String DEFAULT_PREFIX = "/tripod2";

    final String SEPARATOR = "/";

    final String DEFAULT_ZK_CACHE_ZNODE = DEFAULT_PREFIX + SEPARATOR + "cache";

    final String DEFAULT_ZK_CACHEMANAGERZNODE = DEFAULT_PREFIX + SEPARATOR
	    + "cachemanager";

    final String DEFAULT_ZK_PRODUCERMANAGER_ZNODE = DEFAULT_PREFIX + SEPARATOR
	    + "producermanager";

    final String REDIS_STRING_TYPE = "string";

    final String REDIS_LIST_TYPE = "list";

    final String REDIS_HASH_TYPE = "hash";

    final String REDIS_NONE_TYPE = "none";

    final int REDIS_BATCH_SIZE = 900;
}
