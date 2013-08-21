package com.xiaonei.platform.storage;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class FileHelperImpl extends FileHelper {
	public boolean saveAs(File src, File dest) {
		InputStream instream = null;
		OutputStream outstream = null;
		try {
			instream = new FileInputStream(src);
			outstream = new FileOutputStream(dest);
			int bytesRead = 0;
			byte[] buffer = new byte[8192];
			while ((bytesRead = instream.read(buffer, 0, 8192)) != -1) {
				outstream.write(buffer, 0, bytesRead);
			}
			outstream.close();
			instream.close();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				outstream.close();
			} catch (Exception e) {
			}
			try {
				instream.close();
			} catch (Exception e) {
			}
		}
		return true;
	}

	public void destroy() {
	}
}
