package xce.tripod.monitor;

/**
 * <p>Title: TripodMonitoredObject.java</p>
 * <p>Description: </p>
 * <p>Company: RenRen</p>
 * @author xin.huang@renren-inc.com
 * @date 2012-5-29
 */
public interface TripodMonitoredObject {
	
	public void init();
	
	public boolean isAlive();

	public String getDesc();
	
	public void cleanUp();
}
