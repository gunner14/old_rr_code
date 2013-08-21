package com.renren.entrybase.zhan.datasource;

import com.renren.entrybase.EntryBase;
import com.renren.entrybase.mysql.EntryBaseBuilder;

public class ZhanBase {

	protected String catalog = "entrybase_zhan_contributer";

	public ZhanBase() {
	}

	public ZhanBase(String catalog) {
		this.catalog = catalog;
	}

	public void setCatalog(String catalog) {
		this.catalog = catalog;
	}

	public EntryBase createEntryBase() {
		EntryBaseBuilder builder = new EntryBaseBuilder();
		builder.setDataSource(new XnDataSourceFactory(catalog));
		builder.setIndexFields("author",
				new String[] { "tag", "author" });
		builder.setEntryPartitions(100); // 1w
		builder.setIndexPartitions("default", 100); // 1w
		return builder.build();
	}

}
