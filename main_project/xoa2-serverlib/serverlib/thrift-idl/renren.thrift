namespace java com.renren.xoa2.base
namespace cpp xoa2.base

/**
 * Common status reporting mechanism across all services
 */
enum RenrenBaseStatus {
  DEAD = 0,
  STARTING = 1,
  ALIVE = 2,
  STOPPING = 3,
  STOPPED = 4,
  WARNING = 5,
}

service RenrenService {

  /**
   * Returns a descriptive name of the service
   */
  string getName(),

  /**
   * Returns a version of the service
   */
  string getVersion(),

   /**
   * Returns the unix time that the server has been running since
   */
  i64 aliveSince(),

  /**
   * Returns the server's status
   */
  RenrenBaseStatus getStatus(),
  
  /** 
   * Returns a CPU profile over the given time interval (client and server
   * must agree on the profile format).
   */  
  string getCpuProfile(1: i32 profileDurationInSec),

  /**
   * User friendly description of status, such as why the service is in
   * the dead or warning state, or what is being started or stopped.
   */
  string getStatusDetails(),

  /** 
   * Gets the counters for this service
   */  
  map<string, i64> getCounters(),

  /** 
   * Gets the value of a single counter
   */  
  i64 getCounter(1: string key),
 
  /** 
   * 得到某个方法的平均调用时间  
   */  
  string getMean(1: string methodName),
  
  /** 
   * 得到某个方法的最长调用时间  
   */  
  string getMax(1: string methodName, 2: i32 second),

  /** 
   * Tell the server to reload its configuration, reopen log files, etc 
   */  
  oneway void reinitialize(),
   
  /**
   * Gets the counters for this service
   */
  oneway void shutdown()
}
