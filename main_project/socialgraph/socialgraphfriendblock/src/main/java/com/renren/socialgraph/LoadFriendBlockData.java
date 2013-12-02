package com.renren.socialgraph;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import xce.socialgraph.util.Clogging;

/**
 * Load block data asynchronously
 * @author zhangnan
 *
 */
public class LoadFriendBlockData {
	public static void createFriendBlockData(List<BlockDataItem> blockList) {
		ThreadPoolExecutor exec = new ThreadPoolExecutor(100, 200, 10,
				TimeUnit.SECONDS, new LinkedBlockingDeque<Runnable>());
		
		for (BlockDataItem item : blockList) {
			exec.execute(new CreateBlockDataThread(item));
		}
		
		while (exec.getActiveCount() > 0) {
			try {
				Thread.sleep(20000);
				Clogging.Debug("activeCount : " + exec.getActiveCount());
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		try {
			Thread.sleep(200000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		exec.shutdown();
	}
	
	/**
	 * Get block data list from file
	 * @param filePath
	 * @return
	 */
	public static List<BlockDataItem> getFriendBlockData(String filePath) {
		List<BlockDataItem> blockList = new ArrayList<BlockDataItem>();
		
		File file = new File(filePath);
		if (!file.exists()) {
			Clogging.Error(filePath + " doesn't exist!");
			return blockList;
		}
		try {
			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
			String line = reader.readLine();
			while (null != line) {
				String[] itemList = line.split("	");
				if (2 == itemList.length) {					//userid	blockid		ex:100002372       281912230
					BlockDataItem it = new BlockDataItem(Integer.valueOf(itemList[0]), Integer.valueOf(itemList[1]));
					blockList.add(it);
				}
				line = reader.readLine();
			}
			reader.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
		return blockList;
	}
	
	public static void main(String[] args) {
		Clogging.InitLogger("LoadFriendBlockData");
		Clogging.Debug("LoadFriendBlockData");
		
		if (1 != args.length) {
			Clogging.Error("LoadFriendBlockData args length : " + args.length);
			System.exit(0);
		}
		
		List<BlockDataItem> blockDataList = LoadFriendBlockData.getFriendBlockData(args[0]);
		Clogging.Debug("blockDataList size : " + blockDataList.size());

		LoadFriendBlockData.createFriendBlockData(blockDataList);
		
		System.exit(0);
	}
}
