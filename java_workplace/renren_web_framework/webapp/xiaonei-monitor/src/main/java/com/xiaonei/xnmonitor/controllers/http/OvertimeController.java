package com.xiaonei.xnmonitor.controllers.http;

import java.io.File;
import java.io.FilenameFilter;
import java.sql.Timestamp;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.jrobin.core.Util;

import com.xiaonei.xnmonitor.dto.RenRenGroup;
import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.Global;
import com.xiaonei.xnmonitor.utils.SortFile;
import com.xiaonei.xnmonitor.utils.UtilTools;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.REST;
import net.paoding.rose.web.var.Model;

@REST(get = "showList")
public class OvertimeController {

	public String showList(@Param("url") String url) {
		return "@${url}";
	}

	public String[] getFilesArray(final File file, final long end,
			final long start, String groupName) {
		String[] rrdFiles = file.list(new FilenameFilter() {
			public boolean accept(File dir, String name) {
				if (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
					return true;
				} else {
					return false;
				}
			}
		});
		List<String> filesStringList =new LinkedList<String>();
		for (String str :rrdFiles ){
			filesStringList.add(str);
		}
		List<String> domainsList = UtilTools.listGroupedDomains(groupName);
		filesStringList = filterFiles(filesStringList, domainsList);
		filesStringList = rrdNaNfilter(filesStringList, file, end, start);
		String[] corectFiles = filesStringList.toArray(new String[1]);
		return corectFiles;
	}

	public List<String> filterFiles(List<String> fileStringList,
			List<String> domainsList) {
		// TODO filter the domains
		/**
		 * 判断 Files［］ 里面的文件在 domains里面是不是存在。
		 */
		for (int i = 0; i < fileStringList.size();) {
			for (int j = 0; j < domainsList.size(); j++) {
				if (fileStringList.get(i).trim().equals(
						domainsList.get(j).trim()+".rrd")) {
					i++;
					break;
				} else if (j == (domainsList.size()-1)) {
					fileStringList.remove(i);
				}
			}
		}
		return fileStringList;

	}

	public List<String> rrdNaNfilter(List<String> fileStringList, File file,
			long end, long start) {
		/**
		 * 过滤掉rrd文件中数据是空的文件。
		 * 
		 */
		for (int i = 0; i < fileStringList.size();) {
			boolean isNaN = UtilTools.isNaN(file.getAbsolutePath(),
					Global.DSNAME_HTTP, fileStringList.get(i).trim(), end,
					start);
			if (isNaN) {
				fileStringList.remove(i);
			} else {
				i++;
			}

		}
		return fileStringList;

	}

	public String listOvertime( Invocation inv, @Param("dayType") String dayType,
			@Param("groupName") String groupName, @Param("dsName") String dsName) {
		final File file = new File(Global.JROBIN_RESULT_DIR + "httplog/");
		if (null == dayType || "".equals(dayType)) {
			dayType = "day";
		}
		if (null == dsName || "".equals(dsName)) {
			dsName = "HTTP_LOG";
			inv.addModel("dsName", dsName);
		}
		if (groupName == null || "".equals(groupName)) {
			groupName = "all";
		}
		final long end = Util.getTime();
		final long start = UtilTools.getStartTime(dayType, end);
		// String[] rrdFiles = file.list(new FilenameFilter() {
		//
		// public boolean accept(File dir, String name) {
		// if (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
		// return !UtilTools.isNaN(file.getAbsolutePath(),
		// Global.DSNAME_HTTP, name, end, start);
		// } else {
		// return false;
		// }
		// }
		// });
		String[] rrdFiles = getFilesArray(file, end, start, groupName);
		for(String str:rrdFiles){
			System.out.println(str+":)))))");
		}
		Arrays.sort(rrdFiles, new SortFile(file.getAbsolutePath(), end,
				UtilTools.getStartTime(dayType, end), Global.DSNAME_HTTP));
		List<RenRenGroup> groupsList = UtilTools.listRenRenGroups();

		inv.addModel("rrdFiles", rrdFiles);
		inv.addModel("dayType", dayType);
		inv.addModel("groupsList",groupsList);
		return "overtime-listOvertime";
	}

	public String listOvertimeSep(@Param("dayType") String dayType,
			@Param("logType") String logType, @Param("rrdFile") String rrdFile,
			@Param("childDir") String childDir, @Param("bigPic") String bigPic,
			@Param("dsName") String dsName,@Param("groupName")String groupName) {
		return "overtime-listOvertimeSep";

	}

	public String listOvertimeDetail(Model model,
			@Param("dayType") String dayType, @Param("dsName") String dsName,
			@Param("childDir") String childDir,@Param("groupName")String groupName) {
		String path = Global.JROBIN_RESULT_DIR + "httplog/";
		if (null == dsName || "".equals(dsName)) {
			dsName = "HTTP_LOG";
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
		for (int i = 0; i < rrdFilesList.size();) {
			if (rrdFilesList.get(i).contains("127.0.0")) {
				rrdFilesList.remove(i);

			}
			if (UtilTools.isNaN(filePath, Global.DSNAME_HTTP, rrdFilesList
					.get(i), end, start)) {
				rrdFilesList.remove(i);
				continue;
			}
			i++;
		}
		String[] rrdFiles = rrdFilesList.toArray(new String[0]);
		Arrays.sort(rrdFiles,
				new SortFile(path, end, start, Global.DSNAME_HTTP));
		model.add("dsName", dsName);
		model.add("rrdFiles", rrdFiles);
		return "overtime-listOvertimeDetail";
	}

	public String listOvertimeDetailSep(@Param("dayType") String dayType,
			@Param("logType") String logType, @Param("rrdFile") String rrdFile,
			@Param("childDir") String childDir, @Param("bigPic") String bigPic,
			@Param("dsName") String dsName,@Param("groupName")String groupName) {
		return "overtime-listOvertimeDetailSep";
	}

}
