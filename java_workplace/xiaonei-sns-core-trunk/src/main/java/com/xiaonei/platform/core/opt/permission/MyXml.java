package com.xiaonei.platform.core.opt.permission;

import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.net.MalformedURLException;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

public class MyXml {
	public static  void writeDocument(String path,String encod,Document document)throws Exception{
		OutputFormat format = OutputFormat.createPrettyPrint();
		format.setEncoding(encod);
	       
		   XMLWriter writer = new XMLWriter(new FileWriter( path ), format );
		   writer.write( document );
		   writer.close();
		}
		public static  Document read(String fileName) throws MalformedURLException, DocumentException {		
			SAXReader reader = new SAXReader();
			
			Document document = null;
			try
			{
			   document = reader.read(new File(fileName));	
			}catch(Exception e){e.printStackTrace();}
			return document;
		}
		public static  Document read(InputStream stream) throws MalformedURLException, DocumentException {		
			SAXReader reader = new SAXReader();
			
			Document document = null;
			try
			{
			   document = reader.read(stream);	
			}catch(Exception e){e.printStackTrace();}
			return document;
		}
		
}
