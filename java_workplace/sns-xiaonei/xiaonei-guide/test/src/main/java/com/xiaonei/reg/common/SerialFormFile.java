package com.xiaonei.reg.common;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;

import org.apache.struts.upload.FormFile;

public abstract class SerialFormFile implements Serializable, FormFile {

	private static final long serialVersionUID = 7359227654327192812L;

	@Override
	public abstract void destroy();

	@Override
	public abstract String getContentType();

	@Override
	public abstract byte[] getFileData() throws FileNotFoundException,
			IOException;

	@Override
	public abstract String getFileName();

	@Override
	public abstract int getFileSize();

	@Override
	public abstract InputStream getInputStream() throws FileNotFoundException,
			IOException;

	@Override
	public abstract void setContentType(String contentType) ;

	@Override
	public abstract void setFileName(String fileName) ;
	
	@Override
	public abstract void setFileSize(int fileSize) ;
	
}
