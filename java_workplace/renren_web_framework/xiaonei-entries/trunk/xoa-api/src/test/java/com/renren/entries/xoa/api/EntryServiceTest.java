//package com.renren.entries.xoa.api;
//
//import java.util.ArrayList;
//import java.util.List;
//
//import junit.framework.Assert;
//
//import org.junit.BeforeClass;
//import org.junit.Test;
//
//import com.renren.entries.model.Field;
//import com.renren.xoa.lite.ServiceFactories;
//import com.renren.xoa.lite.ServiceFactory;
//import com.renren.xoa.lite.ServiceFuture;
//
//public class EntryServiceTest {

//	private static EntryService entryService;
//	
//	@BeforeClass
//	public static void initEntryService() {
//		ServiceFactory fact = ServiceFactories.getFactory();
//		entryService = fact.getService(EntryService.class);
//	}
//	
//	@Test
//	public void testNextEntryId() throws Exception{
//		ServiceFuture<Long> future = entryService.nextEntryId(0x320).submit();
//		Long entryId = future.awaitUninterruptibly().getContent();
//		Assert.assertNotNull(entryId);
//		Assert.assertTrue(entryId.longValue() > (1L << 43));
//	}
//	
//	@Test
//	public void testSaveEntry() {
//	    Field field1 = new Field().setName("tag").setValue("77").setIndex(false);
//        Field field2 = new Field().setName("tag").setValue("88").setIndex(false);
//        Field field3 = new Field().setName("author").setValue("55").setIndex(false);
//        Field field4 = new Field().setName("title").setValue("hello").setIndex(false);
//        
//        /*Field field5 = new Field().setName("tag").setValue("77").setIndex(false);
//        Field field6 = new Field().setName("tag").setValue("88").setIndex(false);
//        Field field7 = new Field().setName("author").setValue("55").setIndex(false);
//        Field field8 = new Field().setName("title").setValue("hello").setIndex(false);
//        */
//        List<Field> fields = new ArrayList<Field>();
//        fields.add(field1);
//        fields.add(field2);
//        fields.add(field3);
//        fields.add(field4);
//        
//        ServiceFuture<Void> future = entryService.save(100000000l, fields).submit();
//        future.awaitUninterruptibly();
//        Assert.assertTrue(future.isSuccess());
//	}
	
//}
