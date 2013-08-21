package com.renren.entries.index;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import net.paoding.rose.scanning.context.RoseAppContext;

import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.renren.entries.core.EntryService;
import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Field;

@Ignore
public class EntryHomeTest {

//	private static EntryService entryHome;
//	
//	@BeforeClass
//	public static void beforeClass() {
//		RoseAppContext rose = new RoseAppContext();
//	    entryHome = rose.getBean(EntryService.class);
//	};


//	/*
//	 * 没有索引，没有转义字符
//	 */
//	@Test
//	public void testSave() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(false);
//		Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field7 = new Field().setName("author").setValue("55").setIndex(false);
//		Field field8 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
//	
//	/*
//	 * 有索引，没有转义字符
//	 */
//	@Test
//	public void testSave2() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(true);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field7 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field8 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
//	
//	/*
//	 * 有索引，有转义字符
//	 */
//	@Test
//	public void testSave3() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(true);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field7 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field8 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
//	
//	/*
//	 * key中有非法字符
//	 */
//	@Test(expected = IllegalArgumentException.class)
//	public void testSave4() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("tit4le").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
//	
//	/*
//	 * value中有非法字符
//	 */
//	@Test(expected = NumberFormatException.class)
//	public void testSave5() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("7a7").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
//	TODO: name=null的测试用例待
//	/*
//	 * key = null
//	 */
//	@Test(expected = EntryException.class)
//	public void testSave6() throws NumberFormatException {
//		Field field1 = new Field().setName(null).setValue("77").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
	
//	/*
//	 * key = ""
//	 */
//	@Test(expected = EntryException.class)
//	public void testSave7() throws NumberFormatException {
//		Field field1 = new Field().setName("  ").setValue("77").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
//	
//	/*
//	 * value = null
//	 */
//	@Test(expected = EntryException.class)
//	public void testSave8() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue(null).setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
//	
//	/*
//	 * value = ""
//	 */
//	@Test(expected = EntryException.class)
//	public void testSave9() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue(" ").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//	}
	
//	/*
//	 * fields = null
//	 */
//	@Test(expected = EntryException.class)
//	public void testSave10() throws NumberFormatException {
//		// findbug会报警告，所以先注释掉
//		/*long gid = System.currentTimeMillis();
//		entryHome.saveEntry(gid, null);*/
//	}

//	/*
//	 * 含有body
//	 */
//	@Test
//	public void testSave11() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(true);
//		Field field3 = new Field().setName("body").setValue("my name is body$=").setIndex(false);
//		Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(true);
//		Field field7 = new Field().setName("body").setValue("my name is body$=").setIndex(false);
//		Field field8 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
	
//	/*
//	 * 先save，再delete
//	 */
//	@Test
//	public void testDelete() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("99").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		assertNotNull(entry);
//		entryHome.deleteEntry(gid);
//		entry = entryHome.get(gid);
//		assertNull(entry);
//	}
	
//	/*
//	 * 先save(含有body)，再delete
//	 */
//	@Test
//	public void testDelete2() throws NumberFormatException {
//		Field field1 = new Field().setName("tag").setValue("99").setIndex(true);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("body").setValue("my name is body=$").setIndex(true);
//		Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(false);
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		Entry entry = entryHome.get(gid);
//		assertNotNull(entry);
//		entryHome.deleteEntry(gid);
//		entry = entryHome.get(gid);
//		assertNull(entry);
//	}
//	
//	@Test
//	public void testUpdate() {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("author").setValue("55").setIndex(false);
//		Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field7 = new Field().setName("author").setValue("55").setIndex(false);
//		Field field8 = new Field().setName("title").setValue("ppt");
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		entryHome.updateEntry(gid, new Field[] {new Field().setName("title").setValue("ppt")});
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
//	
//	/*
//	 * 含有body的update，不更新body
//	 */
//	@Test
//	public void testUpdate2() {
//		Field field1 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field3 = new Field().setName("body").setValue("my name is body").setIndex(false);
//		Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//		
//		Field field5 = new Field().setName("tag").setValue("77").setIndex(false);
//		Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//		Field field7 = new Field().setName("body").setValue("my name is body").setIndex(false);
//		Field field8 = new Field().setName("title").setValue("ppt");
//		
//		Field[] fields = new Field[] {field1, field2, field3, field4};
//		long gid = System.currentTimeMillis();
//		entryHome.saveEntry(new Entry(gid, fields));
//		entryHome.updateEntry(new Entry(gid, new Field().setName("title").setValue("ppt")));
//		Entry entry = entryHome.get(gid);
//		Field[] fields2 = entry.getFields();
//		Field[] fields3 = new Field[] {field5, field6, field7, field8};
//		assertArrayEquals(fields3, fields2);
//	}
//	
//	/*
//	 * 为下面的testFind准备数据
//	 */
//	@Test
//	public void testInit() throws NumberFormatException {
//		for (int i = 0; i < 100; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("author").setValue("55").setIndex(true),
//					new Field().setName("title").setValue(String.valueOf(i)) };
//			entryHome.saveEntry(new Entry(i, fields));
//		}
//	}
//	
//	/*
//	 * 简单查询
//	 */
//	@Test
//	public void testFind() {
//		String query = "author:55";
//		EntryPage findResult = entryHome.find(query, Long.MAX_VALUE, 10);
//		long cursor = findResult.getCursor();
//		while (findResult.getGids().length > 0) {
//			for (long gid : findResult.getGids()) {
//				System.out.println(gid);
//			}
//			findResult = entryHome.find(query, cursor, 10);
//			cursor = findResult.getCursor();
//			System.out.println("cursor=" + cursor);
//		}
//	}
//	
//	/*
//	 * 为下面的testFind2准备数据
//	 */
//	@Test
//	public void testInit2() throws NumberFormatException {
//		for (int i = 0; i < 8; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("tag").setValue("55").setIndex(true),
//					new Field().setName("title").setValue(String.valueOf(i)) };
//			entryHome.saveEntry(i, fields);
//		}
//		for (int i = 8; i < 16; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("tag").setValue("66").setIndex(true),
//					new Field().setName("title").setValue(String.valueOf(i)) };
//			entryHome.saveEntry(i, fields);
//		}
//	}
//	
//	/*
//	 * 并集查询
//	 */
//	@Test
//	public void testFind2() {
//		String query = "tag:(9 or 55 or 66 or 77)";
//		EntryPage findResult = entryHome.find(query, Long.MAX_VALUE, 10);
//		long cursor = findResult.getCursor();
//		System.out.println("cursor=" + cursor);
//		while (findResult.getGids().length > 0) {
//			for (long gid : findResult.getGids()) {
//				System.out.println(gid);
//			}
//			findResult = entryHome.find(query, cursor, 10);
//			cursor = findResult.getCursor();
//			System.out.println("cursor=" + cursor);
//		}
//	}
//	
//	/*
//	 * 为下面的testFind3准备数据
//	 */
//	@Test
//	public void testInit3() throws NumberFormatException {
//		for (int i = 0; i < 8; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("tag").setValue("55").setIndex(true),
//					new Field().setName("title").setValue(String.valueOf(i)) };
//			entryHome.saveEntry(i, fields);
//		}
//		for (int i = 8; i < 16; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("author").setValue("66").setIndex(true),
//					new Field().setName("tag").setValue("55").setIndex(true) };
//			entryHome.saveEntry(i, fields);
//		}
//		for (int i = 16; i < 24; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("tag").setValue("55").setIndex(true),
//					new Field().setName("title").setValue(String.valueOf(i)) };
//			entryHome.saveEntry(i, fields);
//		}
//		for (int i = 24; i < 32; i++) {
//			Field[] fields = new Field[] {
//					new Field().setName("author").setValue("66").setIndex(true),
//					new Field().setName("tag").setValue("55").setIndex(true) };
//			entryHome.saveEntry(i, fields);
//		}
//	}
//	
//	/*
//	 * 交集查询
//	 */
//	@Test
//	public void testFind3() {
//		String query = "at: 10 and tag:55 and author:66";
//		EntryPage findResult = entryHome.find(query, Long.MAX_VALUE, 10);
//		long cursor = findResult.getCursor();
//		System.out.println("cursor=" + cursor);
//		while (findResult.getGids().length > 0) {
//			for (long gid : findResult.getGids()) {
//				System.out.println(gid);
//			}
//			findResult = entryHome.find(query, cursor, 10);
//			cursor = findResult.getCursor();
//			System.out.println("cursor=" + cursor);
//		}
//	}
	
}















