package com.xiaonei.platform.storage;

import java.io.File;

public abstract class FileHelper {
	public abstract boolean saveAs(File source, File target);

	public abstract void destroy();
}
