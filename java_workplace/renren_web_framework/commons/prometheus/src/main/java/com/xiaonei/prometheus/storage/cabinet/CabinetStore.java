package com.xiaonei.prometheus.storage.cabinet;

import java.util.Map;

import tokyocabinet.TDB;

import com.xiaonei.prometheus.storage.Store;

public class CabinetStore implements Store{

	private TDB db;
	public CabinetStore(String path){
		this(path,TDB.OWRITER|TDB.OCREAT);
	}
	
	public CabinetStore(String path,int omode){
		db = new TDB();
		db.open(path,omode);
	}
	/**
	 * just for test
	 * @return
	 */
	public TDB getTDB(){
		return db;
	}
	
	public Map get(String key) {
		return db.get(key);
	}

	public boolean put(String key, Map cols) {
		return db.put(key, cols);
	}
	
}
