package com.xiaonei.tools.monitor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Iterator;
import java.util.LinkedList;

import org.apache.log4j.Logger;
import org.jrobin.core.RrdException;

import com.xiaonei.tools.monitor.command.Command;
import com.xiaonei.tools.monitor.command.CommandContext;
import com.xiaonei.tools.monitor.command.RrdCommand;

public class HttpAnalyze implements Runnable {
	public final static Format format = new SimpleDateFormat("yyyy-MM-dd-HH");
	static Logger logger = Logger.getLogger(HttpAnalyze.class);
	private String logDir;
	private LinkedList<Command> commandChain = new LinkedList<Command>();
	private CommandContext context;

	public HttpAnalyze() {

	}

	public HttpAnalyze(String logDir) {
		this.logDir = logDir;
		context = new CommandContext();
	}

	public void addCommand(Command command) {
		if (commandChain.contains(command))
			return;
		commandChain.add(command);
	}

	public String getCurFileStr() {
		Date now = new Date();
		String time = format.format(now);
		return "log." + time;
		// log.2009-07-15-10
	}

	public long analyze(long offset) {// offset是文件的偏移量
		String curFileName = getCurFileStr();
		File root = new File(logDir);
		if (!root.exists() || !root.isDirectory()) {
			logger.info(logDir + " not exists");
			System.exit(0);
		}

		File[] files = root.listFiles(new FilenameFilter() {
			public boolean accept(File dir, String name) {
				File file = new File(dir.getPath() + "/" + name);
				if (file.isFile() && name.startsWith("log"))
					return true;
				return false;
			}
		});
		if (files == null) {
			logger.info("Files is null,IO Error ? ");
			return offset;

		}
		if (files.length > 1) {
			Arrays.sort(files);
		}
		for (File f : files) {
			try {
				offset = parser(f, offset);
				if (!curFileName.equals(f.getName())) {
					offset = mv(f);// 移动文件以后就将offset 标记为0
				}
				// return offset;
			} catch (IOException e) {
				e.printStackTrace();
			} catch (RrdException e) {
				e.printStackTrace();
			}
		}
		return offset;// 只会有一个offset
	}

	private long parser(File file, long offset) throws IOException,
			RrdException {
		logger.info("Start Parser " + file.getName());
		BufferedReader bufferReader = new BufferedReader(new FileReader(file));
		bufferReader.skip(offset);
		String line = null;
		while ((line = bufferReader.readLine()) != null) {
			offset += line.length() + 1;
			// System.out.println("Offset :"+offset+":--->"+line);
			String[] items = line.split(" ");
			if (items.length != 4)
				continue;
			execute(items);
		}
		bufferReader.close();
		return offset;
	}

	private void execute(String[] items) {
		Iterator<Command> iter = commandChain.iterator();
		context.setContext("logDir", logDir);
		// System.out.println("In excute ..........");
		while (iter.hasNext()) {
			Command command = iter.next();
			try {
				long startTime = Long.parseLong(items[0]);
				long cost = Long.parseLong(items[3]);
				String rrdFilePath = Global.JROBIN_RESULT_DIR
						+ Global.JRD_CHILD_DIR_HTTP + "/" + items[1] + ".rrd";// 保存的rrd文件的Path
				
				String rrdFilePath2 = Global.JROBIN_RESULT_DIR
						+ Global.JRD_CHILD_DIR_HTTP + "/" + items[1] + items[2]
						+ ".rrd";// 保存的rrd文件的Path
				command.execute(items, context, Global.DSNAME_HTTP,Global.DSTYPE_DB, startTime, cost, rrdFilePath,rrdFilePath2);
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
		context.reset();
	}

	private long mv(File file) throws IOException {
		String destDirStr = Global.HTTPLOG_BACK;
		File destDir = new File(destDirStr);
		if (!destDir.exists()) {
			destDir.mkdirs();
		}
		String destPath = destDir.getAbsolutePath() + "/" + file.getName();
		File dest = new File(destPath);
		boolean suceese = file.renameTo(dest);
		// logger.info("test");
		logger.info(suceese + ":Rename to ------->:" + file);
		// System.out.println("Move the file "+file+" and Init Offset to 0;");
		return 0;
	}

	public void run() {
		HttpAnalyze http = new HttpAnalyze(Global.HTTPLOG_DIR);
		http.addCommand(new RrdCommand());
		long offset = 0;
		while (true) {
			Date begin = new Date();
			// System.out.println("RUN ... The offset now is "+offset);
			offset = http.analyze(offset);
			Date end = new Date();
			logger
					.info("The Analyze HttpLog cost: "
							+ (end.getTime() - begin.getTime())
							+ "  Milliseconds,Then rest for 1 minutes.Next Analysis begins:"
							+ offset);
			try {
				Thread.currentThread().sleep(60 * 1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

	}
}
