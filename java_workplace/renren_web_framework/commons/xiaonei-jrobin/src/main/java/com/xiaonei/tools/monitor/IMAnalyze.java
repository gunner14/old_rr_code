package com.xiaonei.tools.monitor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import org.apache.log4j.Logger;
import org.jrobin.core.RrdException;

import com.xiaonei.tools.monitor.command.Command;
import com.xiaonei.tools.monitor.command.CommandContext;
import com.xiaonei.tools.monitor.command.IMDBCommand;
import com.xiaonei.tools.monitor.command.RrdCommand;
import com.xiaonei.tools.monitor.entity.HttpErrEntity;
import com.xiaonei.tools.monitor.entity.IMEntity;

public class IMAnalyze implements Runnable {

	private final static SimpleDateFormat folderFormat = new SimpleDateFormat(
			"yyyy-MM-dd");// IMLog.2009-07-31

	private final static SimpleDateFormat dateFormat = new SimpleDateFormat(
			"yyyy-MM-dd HH:mm:ss");

	static Logger logger = Logger.getLogger(IMAnalyze.class);

	private String logDir;

	private LinkedList<Command> commandChain = new LinkedList<Command>();

	private CommandContext context;

	private List<File> files = new LinkedList<File>();

	private LinkedList<IMEntity> imEntityList = new LinkedList<IMEntity>();

	private final int IMLOGARRLEN_URL = 3;

	private final int IMLOGARRLEN_DNS = 4;

	private final String SEPRATOR = " ";

	private String latestDBTimeStr = null;

	public IMAnalyze() {

	}

	public IMAnalyze(String logDir) {
		this.logDir = logDir;
		context = new CommandContext();
	}

	public void addCommand(Command command) {
		if (commandChain.contains(command))
			return;
		commandChain.add(command);
	}

	public String getCurFileDir() {
		Date now = new Date();
		String time = folderFormat.format(now);
		return "IMLog." + time;
		// log.2009-07-15-10
	}

	public List<File> getFiles(String fileDir) {
		File dir = new File(fileDir);
		File[] fileArr = dir.listFiles(new FilenameFilter() {

			@Override
			public boolean accept(File dir, String name) {
				if (name.endsWith(".log")
						|| (new File(dir.getAbsolutePath() + "/" + name)
								.isDirectory() && !name
								.equalsIgnoreCase("imbak")))
					return true;
				else
					return false;
			}
		});
		for (File f : fileArr) {
			addFiles(f);
		}
		return files;
	}

	public void addFiles(File file) {
		if (file.isDirectory()) {
			File[] fileDir = file.listFiles(new FilenameFilter() {

				@Override
				public boolean accept(File dir, String name) {
					if (name.endsWith(".log")
							|| (new File(dir.getAbsolutePath() + "/" + name)
									.isDirectory() && !name
									.equalsIgnoreCase("IMErrBak")))
						return true;
					else
						return false;
				}
			});
			for (File f : fileDir) {
				addFiles(f);
			}
		} else {
			files.add(file);
		}
	}

	public long analyze(long offset) {// offset是文件的偏移量
		File root = new File(logDir);
		if (!root.exists() || !root.isDirectory()) {
			logger.info(logDir + " not exists");
			System.exit(0);
		}
		getFiles(logDir);
		File[] fileArr = files.toArray(new File[0]);
		if (fileArr == null) {
			logger.info("Files is null,IO Error ? ");
			return offset;

		}
		if (fileArr.length > 1) {
			Arrays.sort(fileArr);
		}
		for (File f : fileArr) {
			try {
				// if (curFileName.equals(f.getName())||offset!=0) {
				// System.out.print("Analysis "+
				// f+"  and the Offset before Parse is "+ offset);
				offset = parser(f, offset);
				// System.out.println("  and after Parse the offset is "+offset);
				// System.out.println("!curFileName.equals(f.getName()):<-->"+curFileName+"<<>>"+!curFileName.equals(f.getName()));
				// }
				offset = mv(f);// 移动文件以后就将offset 标记为0
				// return offset;
			} catch (IOException e) {
				e.printStackTrace();
			} catch (RrdException e) {
				e.printStackTrace();
			}
		}
		files.clear();
		return offset;// 只会有一个offset
	}

	private String getRemoteHost(String remoteHostString) {
		return remoteHostString.substring(remoteHostString.indexOf(":") + 1);
	}

	private long parser(File file, long offset) throws IOException,
			RrdException {
		logger.info("Start Parser " + file.getName());
		BufferedReader bufferReader = new BufferedReader(new FileReader(file));
		bufferReader.skip(offset);
		String line = null;
		String remoteHost = getRemoteHost(bufferReader.readLine());// 加入用户的IP
		while ((line = bufferReader.readLine()) != null) {
			offset += line.length() + 1;
			// System.out.println("Offset :"+offset+":--->"+line);
			if (validateLog(line, file) == false) {
				// System.out.println(line + ":" + file.getAbsolutePath());
				continue;
			}
			String[] items = line.split(SEPRATOR);
			execute(items, line, file, remoteHost);
		}
		bufferReader.close();
		return offset;
	}

	private boolean validateLog(String line, File file) {
		if (!line.startsWith("dns") && !line.startsWith("url")) {
			return false;
		}
		String[] items = line.split(SEPRATOR);
		if (line.startsWith("dns") && items.length != IMLOGARRLEN_DNS)
			return false;
		if (line.startsWith("url") && items.length != IMLOGARRLEN_URL)
			return false;
		if (getCostTime(items, file) == -1)
			return false;
		String urlOrDns = "";
		if ("dns".equals(items[0].trim())
				&& null == (urlOrDns = getUrl(items[3]))) {
			return false;
		}
		if ("url".equals(items[0].trim())
				&& null == (urlOrDns = getUrl(items[2]))) {
			return false;
		}
		if (urlOrDns == null || IMDBCommand.hasEvilCode(urlOrDns.toCharArray())) {
			return false;
		}
		if (urlOrDns.contains("%") || urlOrDns.contains("'"))
			return false;
		if (!urlOrDns.contains("renren") && !urlOrDns.contains("xnimg")
				&& !urlOrDns.contains("xiaonei"))
			return false;
		return true;
	}

	private String[] getRrdFilePathArr(String[] items, File file) {
		String[] rrdFilePathArr = new String[2];
		if ("dns".equals(items[0].trim())) {
			rrdFilePathArr[0] = Global.JROBIN_RESULT_DIR
					+ Global.JRD_CHILD_DIR_IM + "/dns.rrd";
			rrdFilePathArr[1] = Global.JROBIN_RESULT_DIR
					+ Global.JRD_CHILD_DIR_IM + "/dns/" + getUrl(items[3])
					+ ".rrd";
		}
		if ("url".equals(items[0].trim())) {
			rrdFilePathArr[0] = Global.JROBIN_RESULT_DIR
					+ Global.JRD_CHILD_DIR_IM + "/url.rrd";
			rrdFilePathArr[1] = Global.JROBIN_RESULT_DIR
					+ Global.JRD_CHILD_DIR_IM + "/url/" + getUrl(items[2])
					+ ".rrd";
		}
		return rrdFilePathArr;
	}

	private double strToDouble(String doubleStr) {
		return Double.parseDouble(doubleStr);
	}

	private double getCostTime(String[] items, File file) {
		try {
			String timeStr = items[1].trim();
			String doubleStr = timeStr.substring(timeStr.indexOf("time:") + 5);
			return strToDouble(doubleStr);
		} catch (Exception e) {
			return -1;
		}
	}

	public String getUrl(String url) {
		try {
			if (url.startsWith("url")) {
				if (url.indexOf("?") == -1) {
					return url.substring(url.indexOf("url:") + 11).trim();
				} else {
					return url.substring(url.indexOf("url:") + 11,
							url.indexOf("?")).trim();
				}
			} else if (url.startsWith("host")) {
				return url.substring(url.indexOf("host") + 5).trim();
			} else {
				return null;
			}
		} catch (Exception e) {
			// System.out.println("cutURL:" + url + ":" +
			// file.getAbsolutePath());
			return null;
		}
	}

	private boolean needSaveToDB(String timeStr) throws SQLException,
			ParseException {
		if (latestDBTimeStr == null) {
			latestDBTimeStr = IMDBCommand.getDBLatestTime();
			if (latestDBTimeStr == null) {
				latestDBTimeStr = timeStr;
				return true;
			}
		}
		long latestDBTimeLong = getTime(latestDBTimeStr);
		long timeStrLong = getTime(timeStr);
		if (timeStrLong < latestDBTimeLong) {
			return false;
		} else if (timeStrLong == latestDBTimeLong) {
			return true;
		} else {
			latestDBTimeStr = timeStr;
			return true;
		}
	}

	private long getTime(String timeStr) throws ParseException {

		return dateFormat.parse(timeStr).getTime();
	}

	private void execute(String[] items, String line, File file,
			String remoteHost) {
		Iterator<Command> iter = commandChain.iterator();
		context.setContext("logDir", logDir);
		// System.out.println("In excute ..........");
		while (iter.hasNext()) {
			Command command = iter.next();
			try {
				// long cost = (long) getCostTime(items, file);
				IMDBCommand imDBCommand = new IMDBCommand();
				IMEntity imEntity = imDBCommand.getIMEntity(items, line, file,
						remoteHost);
				if (null != imEntity) {
				 
					/*
					 * 因为im的log是一个文件一个文件的。所以可以除去 needSaveToDb的判断。
					 */
					// if (needSaveToDB(imEntity.getHappenTime())) {
					imDBCommand.excute(imEntity, imEntityList);
					// }
				}

			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
		context.reset();
	}

	private long mv(File file) throws IOException {
		String parentDir = file.getParent()
				.substring(Global.IMLOG_DIR.length());// IM*****
		String destDirStr = Global.IMLOG_BACK + parentDir;
		// System.out.println("MV DESTDIRSTR:" + file.getAbsolutePath());
		File destDir = new File(destDirStr);
		if (!destDir.exists()) {
			destDir.mkdirs();
		}
		String destPath = destDir.getAbsolutePath() + "/" + file.getName();
		File dest = new File(destPath);
		boolean suceese = file.renameTo(dest);
		logger.info(suceese + ":Rename to ------->:" + file);
		// System.out.println(file.getAbsolutePath() + "::" + getCurFileDir());
		if (!file.getAbsolutePath().contains(getCurFileDir())) {
			File parentAbsoluteDir = new File(file.getParent());
			File[] files = parentAbsoluteDir.listFiles();
			if (files.length < 1) {
				parentAbsoluteDir.delete();
				logger.info(suceese + ":Rename to ------->:" + file);
			}
		}
		return 0;
	}

	public void run() {
		IMAnalyze im = new IMAnalyze(Global.IMLOG_DIR);
		im.addCommand(new RrdCommand());
		long offset = 0;
		while (true) {
			Date begin = new Date();
			// System.out.println("RUN ... The offset now is "+offset);
			offset = im.analyze(offset);
			Date end = new Date();
			logger
					.info("The Analyze IMLOG cost: "
							+ (end.getTime() - begin.getTime())
							+ "  Milliseconds,Then rest for 1 minutes.Next Analysis begins:"
							+ offset);
			System.out
					.println("The Analyze IMLOG cost: "
							+ (end.getTime() - begin.getTime())
							+ "  Milliseconds,Then rest for 1 minutes.Next Analysis begins:"
							+ offset);
			try {
				Thread.currentThread().sleep(60 * 1000);
				System.out.println("IMAnalyze Thread is still alive ");
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

	}
}
