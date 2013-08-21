package com.xiaonei.tools.logger;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItemIterator;
import org.apache.commons.fileupload.FileItemStream;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

public class IMReportServletCommand extends javax.servlet.http.HttpServlet {

	private static final long serialVersionUID = 7162749733606989081L;

	private static final SimpleDateFormat format = new SimpleDateFormat(
			"yyyy-MM-dd");

	private static final String DIR_PREFIX = "/data/httperror/imCommand/";

	@Override
	protected void service(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		boolean isMultipart = ServletFileUpload.isMultipartContent(request);
		if (!isMultipart)
			return;
		ServletFileUpload upload = new ServletFileUpload();
		try {
			FileItemIterator iter = upload.getItemIterator(request);
			while (iter.hasNext()) {
				FileItemStream item = iter.next();
				String name = item.getFieldName();
				if (item.isFormField()) {
					// System.out.println(name);
				} else {
					String fileParentPath = getCurrentAbsoluteDir();
					String filePath = fileParentPath + "/"
							+ System.currentTimeMillis() + ".log";
					InputStream stream = item.openStream();
					OutputStream bos = new FileOutputStream(filePath);
					byte[] buffer = new byte[8192];
					int bytesRead = 0;
					byte[] remoteHost = ("remotehost:"
							+ request.getRemoteHost() + "\n").getBytes();
//					byte[] userId = ("uid:" + request.getParameter("uid") + "\n")
//							.getBytes();
					bos.write(remoteHost);
//					bos.write(userId);
					while ((bytesRead = stream.read(buffer, 0, 8192)) != -1) {
						bos.write(buffer, 0, bytesRead);
					}

					bos.close();
					stream.close();
					// System.out.println("this is file " + name);
				}
			}
		} catch (FileUploadException ex) {
			ex.printStackTrace();
		}
	}

	public String getCurrentAbsoluteDir() {
		String dir = "IMLog." + format.format(new Date());
		String absolutePath = DIR_PREFIX + dir;
		isDirExist(absolutePath);
		return absolutePath;
	}

	public boolean isDirExist(String path) {
		File f = new File(path);
		if (!f.exists()) {
			f.mkdirs();
		}
		return true;

	}

}
