package com.renren.entries.xoa;


/**
 * xoa接口单元测试
 * 
 * @author yanghe.liang@opi-corp.com
 */
public class Client {

//	private static XoaClient client;
//	
//	@BeforeClass
//	public static void beforeClass() {
//		SimpleXoaRegistry reg = new SimpleXoaRegistry();
//		String serviceId = "page.xoa.renren.com";
//		reg.register(serviceId, "10.3.18.181", 8188);
//		client = new XoaClient(reg);
//	};
//
//	@Test
//	public void testSave() {
//		String url = "xoa://page.xoa.renren.com/entry";
//		Method method = Method.post(url);
//		
//		method.setParam("gid", "5999");
//		
//		Field f1 = new Field().setName("author").setValue("999").setIndex(true);
//		Field f2 = new Field().setName("tag").setValue("4999").setIndex(true);
//		Field f3 = new Field().setName("tag").setValue("3999").setIndex(true);
//		Field[] ff = new Field[] {f1, f2, f3};
//		
//		method.setParamAsJson("fields", ff);
//		client.execute(method);
//		System.out.println("over");
//	}
//	
//	@Test
//	public void testUpdate() {
//		String url = "xoa://page.xoa.renren.com/entry/999";
//		Method method = Method.put(url);
//		
//		Field f1 = new Field().setName("tag").setValue("5999").setIndex(false);
//		Field f2 = new Field().setName("tag").setValue("4999").setIndex(false);
//		Field f3 = new Field().setName("tag").setValue("3999").setIndex(false);
//		Field[] ff = new Field[] {f1, f2, f3};
//		
//		method.setParamAsJson("fields", ff);
//		client.execute(method);
//		System.out.println("over");
//	}
//	
//	@Test
//	public void testDelete() {
//		String url = "xoa://page.xoa.renren.com/entry/999";
//		Method method = Method.delete(url);
//		client.execute(method);
//		System.out.println("over");
//	}
//	
//	@Test
//	public void testGet() {
//		String url = "xoa://page.xoa.renren.com/entry/5999";
//		Method method = Method.get(url);
//		XoaResponse response =  client.execute(method);
//		Entry entry = response.getContentAs(Entry.class);
//		Field[] fields = entry.getFields();
//		for (Field field : fields) {
//			System.out.println(field);
//		}
//	}
//	
//	@Test
//	public void testGets() {
//		String url = "xoa://page.xoa.renren.com/entry";
//		Method method = Method.get(url);
//		method.setParam("gids", "999");
//		method.setParam("gids", "1999");
//		method.setParam("gids", "2999");
//		XoaResponse response =  client.execute(method);
//		Entry[] entries = response.getContentAs(Entry[].class);
//		for (Entry entry : entries) {
//			Field[] fields = entry.getFields();
//			for (Field field : fields) {
//				System.out.println(field);
//			}
//			System.out.println("**");
//		}
//	}
//	
//	@Test
//	public void testFind() {
//		String url = "xoa://page.xoa.renren.com/entry/find";
//		Method method = Method.get(url);
//		method.setParam("query", "tag:4999 and author:1999");
//		method.setParam("limit", "1");
//		XoaResponse response =  client.execute(method);
//		EntryPage findResult = response.getContentAs(EntryPage.class);
//		System.out.println(findResult.getUpperCursor());
//		for (long gid : findResult.getEntries()) {
//			System.out.println(gid);
//		}
//	}
	
}

