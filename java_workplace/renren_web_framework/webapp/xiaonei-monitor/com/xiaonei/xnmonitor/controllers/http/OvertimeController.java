package com.xiaonei.xnmonitor.controllers.http;

import java.io.File;
import java.io.FilenameFilter;
import java.sql.Timestamp;
import java.util.Arrays;
import java.util.List;

import org.jrobin.core.Util;

import com.xiaonei.xnmonitor.dto.TimeParam;
import com.xiaonei.xnmonitor.utils.Global;
import com.xiaonei.xnmonitor.utils.SortFile;
import com.xiaonei.xnmonitor.utils.UtilTools;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.REST;
import net.paoding.rose.web.var.Model;

@REST(get = "showList")
public class OvertimeController {

	public String showList(@Param("url") String url) {
		return "@${url}";
	}

	public String listOvertime(Model model, @Param("dayType") String dayType,
			@Param("dsName") String dsName) {
		final File file = new File(Global.JROBIN_RESULT_DIR + "httplog/");
		if (null == dayType || "".equals(dayType)) {
			dayType = "day";
		}
		if (null == dsName || "".equals(dsName)) {
			dsName = "HTTP_LOG";
			model.add("dsName", dsName);
		}
		final long end = Util.getTime();
		final long start = UtilTools.getStartTime(dayType, end);
		String[] rrdFiles = file.list(new FilenameFilter() {

			public boolean accept(File dir, String name) {
				if (name.endsWith(".rrd") && !name.contains("127.0.0.1")) {
					return !UtilTools.isNaN(file.getAbsolutePath(),
							Global.DSNAME_HTTP, name, end, start);
				} else {
					return false;
				}
			}
		});
		Arrays.sort(rrdFiles, new SortFile(file.getAbsolutePath(), end,
				UtilTools.getStartTime(dayType, end), Global.DSNAME_HTTP));

		model.add("rrdFiles", rrdFiles);
		model.add("dayType", dayType);
		return "overtime-listOvertime";
	}

	public String listOvertimeSep(@Param("dayType") String dayType,
			@Param("logType") String logType, @Param("rrdFile") String rrdFile,
			@Param("childDir") String childDir, @Param("bigPic") String bigPic,
			@Param("dsName") String dsName) {
		return "overtime-listOvertimeSep";

	}

	public String listOvertimeDetail(Model model,
			@Param("dayType") String dayType, @Param("dsName") String dsName,
			@Param("childDir") String childDir) {
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

	public String  listOvertimeDetailSep(@Param("dayType") String dayType,
			@Param("logType") String logType, @Param("rrdFile") String rrdFile,
			@Param("childDir") String childDir, @Param("bigPic") String bigPic,
			@Param("dsName") String dsName) {
		return "overtime-listOvertimeDetailSep";

	}

}
