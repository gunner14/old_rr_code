namespace java com.renren.xoa2.backdoor
namespace cpp renren.xoa2.backdoor

/**
 * Common status reporting mechanism across all services
 */
enum renren_status {
  DEAD = 0,
  STARTING = 1,
  ALIVE = 2,
  STOPPING = 3,
  STOPPED = 4,
  WARNING = 5,
}

/**
 * Standard base service
 */
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
  renren_status getStatus(),
  
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
  * Sets an option
  */
  void setOption(1: string key, 2: string value),

  /**
   * Gets an option
   */
  string getOption(1: string key),

  /**
   * Gets all options
   */
  map<string, string> getOptions(),

  /**
   * Returns a CPU profile over the given time interval (client and server
   * must agree on the profile format).
   */
  string getCpuProfile(1: i32 profileDurationInSec),

  string getMean(1: string methodName),

  string getMax(1: string methodName, 2: i32 second),
  
  /**
   * Tell the server to reload its configuration, reopen log files, etc
   */
  oneway void reinitialize(),

  /**
   * Suggest a shutdown to the server
   */
  void shutdown(),

  oneway void startMilitiaService(1:string serviceId),

}
