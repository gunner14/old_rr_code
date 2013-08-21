package com.renren.xoa.commons.test;


public class BlogFactory {

	private static BlogFactory instance = new BlogFactory();
	
	public static BlogFactory getInstance() {
		return instance;
	}
	
	private BlogFactory() {
		pepareDatas();
	}

	private Blog[][] blogs = new Blog[257][];
	
	private void pepareDatas() {
		blogs[0] = new Blog[0];
		blogs[2] = prepareData(2);
		blogs[8] = prepareData(8);
		blogs[32] = prepareData(32);
		blogs[128] = prepareData(128);
		blogs[256] = prepareData(256);
	}
	
	public Blog[] getDataBySize(int k) {
	
		if (k >= 257) {
			throw new IllegalArgumentException();
		}
		
		if (blogs[k] != null) {
			return blogs[k];
		}
		
		blogs[k] = prepareData(k);
		return blogs[k];
	}
	
	private Blog[] prepareData(int k) {
		int n = k * 10;
		int blogId = 100;
		int userId = 10000;
		
		Blog[] blogs = new Blog[n];
		for (int i = 0; i < blogs.length; i++) {
			Blog b = new Blog();
			b.setContent("回来的飞机上看书，看到这张照片的时候。");
			b.setOwnerId(userId++);
			b.setTitle("标题" + blogId++);
			blogs[i] = b;
		}
		return blogs;
	}
}

