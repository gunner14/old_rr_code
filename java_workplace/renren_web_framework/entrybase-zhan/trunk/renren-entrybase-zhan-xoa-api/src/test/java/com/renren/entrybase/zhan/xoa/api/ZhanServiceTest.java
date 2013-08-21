package com.renren.entrybase.zhan.xoa.api;

import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.entrybase.model.Entry;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;

public class ZhanServiceTest {
	private static EntryBaseService entryBaseService;
	
	@BeforeClass
	public static void initEntryService(){
		ServiceFactory factory = ServiceFactories.getFactory();
		entryBaseService = factory.getService(EntryBaseService.class);
	}

	@Test
	public void testAdd() {
		Entry entry = new Entry();
		entry.addField("author", 44);
		entry.addField("tag", 44);
		entry.addField("tag", 55);
		entry.addField("tag", 66);
		entry.addField("title", "title");
		entry.setId(4);
		System.out.println(entry.getFields());
		//Void ret = entryBaseService.add(100000000, entry).submit().awaitUninterruptibly().getContent();;
//        Assert.assertEquals(1, ret.longValue());
		System.out.println(entry.getFields());
	}
	@Test
	public void testGet() {
		Entry ret = entryBaseService.get(100000000).submit().awaitUninterruptibly().getContent();
		System.out.println(ret);
	}
	@Test
	public void testDelete() {
		Void ret = entryBaseService.delete(100000000).submit().awaitUninterruptibly().getContent();
		System.out.println(ret);
	}
	@Test
	public void testFind() {
		Entry[] ret = entryBaseService.find("author:4", 10).submit().awaitUninterruptibly().getContent();
		for(Entry entry : ret){
			System.out.println(entry);
		}
	}
	
	@Test
	public void testGets() {
		long[] ids = {100000001,6,2};
		Entry[] ret = entryBaseService.gets(ids).submit().awaitUninterruptibly().getContent();
		for(Entry entry : ret){
			System.out.println(entry);
		}
	}
}
