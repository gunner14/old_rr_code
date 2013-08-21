package com.xiaonei.platform.scheduleTask;

import java.text.SimpleDateFormat;
import java.util.Date;

import com.xiaonei.platform.storage.Constants;

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
		boolean result = this.getSpaceUsed() < Constants.THRESHOLD_SPACEUSED;
		return result;
	}

	PropertyUtil proper = new PropertyUtil();

	int warnValue = proper.getKeyValueloadFileByIO("NodeStatus.property",
			"LEVEL_WARN");

	public boolean isWritable() {
		boolean result = this.getSpaceUsed() < Constants.THRESHOLD_SPACEUSED
				&& this.getCpuLoad() < warnValue;
		return result;
	}

	public boolean isReadableAndWritable() {
		return isWritable();
	}

	public boolean isTempReable() {
		boolean result = (this.status == 4);
		return result;
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
