package com.renren.xoa.commons.test;

import com.renren.xoa.commons.test.RizhiProtos.Rizhi;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList.Builder;

public class RizhiFactory {

	private static RizhiFactory instance = new RizhiFactory();

	public static RizhiFactory getInstance() {
		return instance;
	}

	private RizhiFactory() {
		pepareDatas();
	}

	private RizhiList[] rizhis = new RizhiList[257];

	private void pepareDatas() {

	}

	public RizhiList getDataBySize(int k) {

		if (k >= 257) {
			throw new IllegalArgumentException();
		}
		if (rizhis[k] != null) {
			return rizhis[k];
		}
		rizhis[k] = prepareData(k);
		return rizhis[k];
	}

	private RizhiList prepareData(int k) {
		int n = k * 10;
		int blogId = 100;
		int userId = 10000;
		Builder rlb = RizhiList.newBuilder();
		for (int i = 0; i < n; i++) {
			Rizhi r = Rizhi.newBuilder().setOwnerId(userId++).setTitle(
					"Rizhi" + blogId++).setContent("回来的飞机上看书，看到这张照片的时候，很是感动。").build();
			rlb.addRizhis(r);
		}
		return rlb.build();
	}
}
