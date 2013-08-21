package com.xiaonei.platform.storage;

import java.sql.Connection;

public abstract class DatabaseHelper {
	public abstract Connection getConnection();
	public abstract void closeConnection(Connection conn);
}