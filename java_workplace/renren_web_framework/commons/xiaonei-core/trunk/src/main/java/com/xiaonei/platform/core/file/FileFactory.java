package com.xiaonei.platform.core.file;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Random;

import org.apache.struts.upload.FormFile;

import xce.util.tools.IPAddress;

import com.xiaonei.platform.core.opt.OpiConstants;


public class FileFactory {

	private static FileFactory instance;

	private List<FileCounter> quie = new ArrayList();

	private static String seed[] = { "a", "b", "c", "d", "e", "f", "g", "h",
			"i", "j", "k", "l", "m", "n", "o", "p" };

	private String addr3 = "";
	private String addr4 = "";

	private Random rand = new Random();

	static {
		instance = new FileFactory();
		instance.initPath();
		instance.initAddr34();
	}

	public static FileFactory getInstance() {

		return instance;
	}

	private void initAddr34() {
		try {
			/*InetAddress hostIP = InetAddress.getLocalHost();
			String ip = hostIP.getHostAddress();*/
			String ip = IPAddress.getLocalAddress();
			int i = ip.lastIndexOf(".");
			this.addr4 = fix3Width(ip.substring(i + 1, ip.length()));
			System.out.println("-------addr4--:" + this.addr4);
			ip = ip.substring(0, i);
			i = ip.lastIndexOf(".");
			this.addr3 = fix3Width(ip.substring(i + 1));
			System.out.println("-------addr3--:" + this.addr3);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

	}
	private String fix3Width(String str) {
		return str.length() == 1 ? "00" + str : 
			str.length() == 2 ? "0" + str : str; 
	}
	
	private void initPath() {
		try {
			File f = new File(OpiConstants.tempPath);
			if (!f.exists())
				f.mkdir();
			File f2 = new File(OpiConstants.tempPhotoPath);
			if (!f2.exists())
				f2.mkdir();
			File f3 = new File(OpiConstants.tempHeadPath);
			if (!f3.exists())
				f3.mkdir();
			File f4 = new File(OpiConstants.tempBlogPath);
			if (!f4.exists())
				f4.mkdir();
			File f5 = new File(OpiConstants.tempTribePath);
			if (!f5.exists())
				f5.mkdir();
			File f6 = new File(OpiConstants.tempPic001Path);
			if (!f6.exists())
				f6.mkdir();
			File f7 = new File(OpiConstants.tempShoolMatePath);
			if (!f7.exists())
				f7.mkdir();
			File f8 = new File(OpiConstants.tempTmpPath);
			if (!f8.exists())
				f8.mkdir();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public FileFactory() {
		for (int i = 0; i < seed.length; i++) {
			quie.add(new FileCounter());
		}
	}

	public String getRandName() {
		int qid = rand.nextInt(seed.length);

		return this.quie.get(qid).getCounter() + "" + seed[qid] + this.addr3 + this.addr4;
	}

	public String getPathByTime() {
		return null;
	}
	
	/**
	 * 返回group后使用的新规则短路径<br/>
	 * 按照 年月日/小时5分钟的倍数处理
	 * 
	 * @return 类似 20090402/0200，不包含前后的/
	 */
	public String getPathShort(Calendar cl) {
		String path = String.format("%04d%02d%02d/%02d%02d", 
				cl.get(Calendar.YEAR), 
				cl.get(Calendar.MONTH)+1,
				cl.get(Calendar.DAY_OF_MONTH),
				cl.get(Calendar.HOUR_OF_DAY),
				cl.get(Calendar.MINUTE) / 5 * 5);
		return path;
	}
	/**
	 * 返回group后使用的新规则短路径<br/>
	 * 按照 年月日/小时5分钟的倍数处理
	 * 
	 * @return 类似 20090402/0200，不包含前后的/
	 */
	public String getPathShort() {
		return getPathShort(Calendar.getInstance());
	}
	
	public String getDiv1(Calendar cl) {
		String ys, ms;
		ys = cl.get(Calendar.YEAR) + "";
		int month = cl.get(Calendar.MONTH) + 1;
		if (month < 10) {
			ms = "0" + month;
		} else {
			ms = month + "";
		}
		int day = cl.get(Calendar.DATE);
		if (day < 10) {
			return ys + ms + "0" + day;
		} else {
			return ys + ms + "" + day;
		}
	}
    public boolean isText(String  fp){
    	
    	try {
			String cmd = "file -b ";
			
			Process pr = Runtime.getRuntime().exec(cmd + fp); // Calls "dir"
																// command
			StringBuffer sbOut = new StringBuffer(1000);
			BufferedReader br = new BufferedReader(new InputStreamReader(pr
					.getInputStream()));
			while (true) {
				String s = br.readLine();
				if (s == null)
					break;
				sbOut.append(s);
			}
			br.close();
			pr.waitFor();
			String res = sbOut.toString();
			if(res !=null&&(res.indexOf("text")>0))
				return true ;

		} catch (Exception e) {
			e.printStackTrace() ;
		}
		return false ;
    }
	public String getDiv2(Calendar cl) {
		String div2;
		if (cl.get(Calendar.HOUR_OF_DAY) < 10) {
			div2 = "0" + cl.get(Calendar.HOUR_OF_DAY) + "";
		} else {
			div2 = cl.get(Calendar.HOUR_OF_DAY) + "";
		}

		return div2;
	}

	public String getDiv3(Calendar cl) {
		String div3 = "";
		int min = cl.get(Calendar.MINUTE);
		
		min = min / 5 * 5;
		if (min < 10)
			div3 += "0";
		div3 += min;
		return div3;
	}

	public File createTempFile(String path, String perfx, String sufx) {
		return new File(path + "/" + perfx + "_" + this.getRandName() + sufx);
	}

	public byte[] readFile(String file) {
		File f = new File(file);
		if (!f.exists())
			return null;
		long length = f.length();
		byte[] bt = new byte[(int) length];
		FileInputStream is = null;
		BufferedInputStream bis = null;
		try {
			is = new FileInputStream(f);
			bis = new BufferedInputStream(is);
			int l = bis.read(bt);
	        return bt ;
		} catch (Exception e) {
            e.printStackTrace(System.err) ;
		} finally {
			try {
               if(bis !=null)
            	   bis.close() ;
			} catch (Exception e) {

			}
			try {
               if (is !=null)
                    is.close() ;
			} catch (Exception e) {

			}
		}

		return null;

	}
    public void writeFile(String path,byte bt[])throws IOException{
    	FileOutputStream os = null;
		BufferedOutputStream bos = null;
		//System.out.println(path) ;
		try{
			os = new FileOutputStream(path) ;
			bos = new BufferedOutputStream(os,bt.length) ;
			bos.write(bt) ;
			
		}finally{
			if(bos !=null)
				bos.close() ;
			if(os !=null)
				os.close() ;
		}
    }
	public void dump(FormFile ff, File dest) throws IOException {
		InputStream is = null;
		OutputStream os = null;
		try {
			is = ff.getInputStream();

			os = new FileOutputStream(dest);
			int bytesRead = 0;
			byte[] buffer = new byte[8192];
			while ((bytesRead = is.read(buffer, 0, 8192)) != -1) {
				os.write(buffer, 0, bytesRead);
			}
		} finally {
			try {
				os.close();
			} catch (Exception e) {
			}
			try {
				is.close();
			} catch (Exception e) {
			}
			try {
				ff.destroy();
			} catch (Exception e) {
			}
		}
	}

	public static void main(String args[]) throws Exception {
		// for(int i =0;i<100 ;i++)
		// System.out.println((FileFactory.getInstance().createTempFile("E:/",
		// "head", "jpg")).getAbsolutePath()) ;
		System.out.println(getInstance().getPathShort());
	}

}

