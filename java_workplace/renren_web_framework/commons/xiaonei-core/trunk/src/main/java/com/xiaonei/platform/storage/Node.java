package com.xiaonei.platform.storage;

import java.text.SimpleDateFormat;
import java.util.Date;

public class Node {

	private int id;

	private String name;

	private String group;

	private String path;

	private double spaceUsed = 0.0;

	private double cpuLoad;

	private int status;

	private String domain;

	// private long stateTime = System.currentTimeMillis() / 1000;

	private Date time;

	private int type;

	public static String COLUMN_NODE_ID = "id";

	public static String COLUMN_NODE_NAME = "node_name";

	public static String COLUMN_GROUP_NAME = "group_name";

	public static String COLUMN_PATH = "path";

	public static String COLUMN_DOMAIN = "domain";

	public static String COLUMN_SPACE_USED = "space_used";

	public static String COLUMN_CPU_LOAD = "cpu_load";

	public static String COLUMN_TIME = "time";

	public static String COLUMN_STATUS = "status";

	public static String COLUMN_TYPE = "type";

	public String getDomain() {
		return domain;
	}

	public void setDomain(String domainName) {
		this.domain = domainName;
	}

	public String getGroup() {
		return group;
	}

	public void setGroup(String name) {
		group = name;
	}

	public String getPath() {
		return path;
	}

	public void setPath(String path) {
		this.path = path;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public synchronized double getSpaceUsed() {
		return spaceUsed;
	}

	public synchronized void setSpaceUsed(double space) {
		this.spaceUsed = space;
	}

	public synchronized double getCpuLoad() {
		return cpuLoad;
	}

	public synchronized void setCpuLoad(double load) {
		this.cpuLoad = load;
	}

	public boolean isReadable() {
		boolean result = ((this.status & Constants.NODE_STATUS_READABLE) != 0)
				&& this.getSpaceUsed() < Constants.THRESHOLD_SPACEUSED;
		return result;
	}

	public boolean isWritable() {
		boolean result = (this.status & Constants.NODE_STATUS_WRITABLE) != 0
				&& this.getSpaceUsed() < Constants.THRESHOLD_SPACEUSED;
		return result;
	}

	public boolean isReadableAndWritable() {
		return isWritable() && isWritable();
	}

	public boolean isTempReable() {
		boolean result = (this.status == 4);
		return result;
	}
	
	public int loadaverage() {
		double load = this.getCpuLoad();
		if (load <= 5) {
			return Constants.RANK_LEVEL_IDLE;
		}
		if (load <= 8) {
			return Constants.RANK_LEVEL_NORMAL;
		}
		if (load <= 12) {
			return Constants.RANK_LEVEL_WARN;
		}
		return Constants.RANK_LEVEL_FATAL;
	}

	public String toString() {
		return this.getName() + "\t" + this.getGroup() + "\t"
				+ this.getDomain() + "\t" + this.getSpaceUsed() + "\t"
				+ this.getCpuLoad() + "\t" + this.isReadable() + "\t"
				+ this.isWritable();
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public synchronized Date getTime() {
		return time;
	}

	public String getTimeDesc() {
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		return formatter.format(getTime());
	}

	public synchronized void setTime(Date time) {
		this.time = time;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}
}
