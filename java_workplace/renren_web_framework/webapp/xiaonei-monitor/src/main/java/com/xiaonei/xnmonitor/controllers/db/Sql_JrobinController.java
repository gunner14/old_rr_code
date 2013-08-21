package com.xiaonei.xnmonitor.controllers.db;

import java.io.File;
import java.io.FilenameFilter;
import java.util.Arrays;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.var.Model;

import org.jrobin.core.Util;

import com.xiaonei.xnmonitor.utils.Global;
import com.xiaonei.xnmonitor.utils.SortFile;
import com.xiaonei.xnmonitor.utils.UtilTools;

public class Sql_JrobinController {

	public String listBiz(Model model, @Param("dayType") String dayType,
			@Param("dsName") String dsName) {
		final File file = new File(Global.JROBIN_RESULT_DIR
				+ Global.JRD_CHILD_DIR_DB + "/");
		if (null == dayType || "".equals(dayType)) {
			dayType = "day";
		}
		if (null == dsName || "".equals(dsName)) {
			dsName = "DB_LOG";
			model.add("dsName", dsName);
		}
		final long end = Util.getTime();
		final long start = UtilTools.getStartTime(dayType, end);
		String[] rrdFiles = file.list(new FilenameFilter() {

			public boolean accept(File dir, String name) {
				if (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
					// System.out.println(name+"file name");
					return !UtilTools.isNaN(file.getAbsolutePath(),
							Global.DSNAME_DB, name, end, start);
					// return true;
				} else {
					return false;
				}
			}

		});
		// System.out.println("out");
		Arrays.sort(rrdFiles, new SortFile(file.getAbsolutePath(), end,
				UtilTools.getStartTime(dayType, end), Global.DSNAME_DB));
		// int i = 0;
		// for (String s : rrdFiles) {
		// System.out.println(s + "<-------------ssssssssssss------------>:" +
		// (i++));
		// }
		model.add("rrdFiles", rrdFiles);
		model.add("dayType", dayType);
		return "sql_Jrobin-listBiz";
	}

	public String listBizSep(Model model, @Param("dayType") String dayType,
			@Param("dsName") String dsName, @Param("rrdFile") String rrdFile,
			@Param("bigPic") String bigPic) {
		return "sql_Jrobin-listBizSep";

	}

	public String listSql(Model model, @Param("dayType") String dayType,
			@Param("dsName") String dsName, @Param("childDir") String childDir) {
		String path = Global.JROBIN_RESULT_DIR + Global.JRD_CHILD_DIR_DB + "/";
		if (null == dsName || "".equals(dsName)) {
			dsName = "DB_LOG";
			model.add("dsName", dsName);
		}
		if (null != childDir && childDir.length() > 1) {
			if (childDir.startsWith("/")) {
				childDir = childDir.substring(1, childDir.length());
			}
			if (childDir.endsWith("/")) {
				childDir = childDir.substring(0, childDir.length() - 1);
			}
			if (childDir.endsWith(".rrd")) {
				childDir = childDir.substring(0, childDir.length() - 4);
			}
			path += childDir;
		}
		final String filePath = path;
		File file = new File(path);
		final long end = Util.getTime();
		final long start = UtilTools.getStartTime(dayType, end);
		UtilTools tools = new UtilTools();
		List<String> rrdFilesList = tools.getFileList(path, path.length() + 1);
		// for (String str :rrdFilesList){
		// System.out.println(str+"aaaaaaa");
		// }
		// int listSize =rrdFilesList.size();
		for (int i = 0; i < rrdFilesList.size();) {
			// System.out.println("size "+rrdFilesList.size());
			// System.out.println("--beforeremove "+rrdFilesList.get(i));
			// System.out.println("Judge is Double.NaN:"+rrdFilesList.get(i));

			if (UtilTools.isNaN(filePath, Global.DSNAME_DB,
					rrdFilesList.get(i), end, start)) {
				// System.out.println("----remove " + rrdFilesList.get(i));
				rrdFilesList.remove(i);
				continue;
			}
			i++;
		}
		// String[] rrdFiles = file.list(new FilenameFilter() {
		// public boolean accept(File dir, String name) {
		// if (name.endsWith(".rrd")) {
		// return UtilTools.isNaN(filePath, Global.HTTP_DSNAME, name, end,
		// start);
		// } else {
		// return false;
		// }
		// }
		// });
		String[] rrdFiles = rrdFilesList.toArray(new String[0]);
		Arrays.sort(rrdFiles, new SortFile(path, end, start, Global.DSNAME_DB));
		model.add("rrdFiles", rrdFiles);
		return "sql_Jrobin-listSql";
	}

	public String listSingle(Model model, @Param("dayType") String dayType,
			@Param("dsName") String dsName, @Param("childDir") String childDir,
			@Param("rrdFile") String rrdFile, @Param("bigPic") String bigPic) {
		if (null == dsName || "".equals(dsName)) {
			dsName = "DB_LOG";
			model.add("dsName", dsName);
		}
		if ("true".equals(bigPic)) {
			bigPic = "false";
		} else if ("false".equals(bigPic)) {
			bigPic = "true";
		} else {
			bigPic = "false";
		}
		model.add("bigPic", bigPic);
		return "sql_Jrobin-listSingle";
	}
}
