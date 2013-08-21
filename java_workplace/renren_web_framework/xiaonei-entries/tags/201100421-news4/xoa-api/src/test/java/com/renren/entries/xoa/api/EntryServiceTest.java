package com.renren.entries.xoa.api;

import java.util.ArrayList;
import java.util.List;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.renren.entries.model.Entry;
import com.renren.entries.model.Field;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;

@Ignore
public class EntryServiceTest {

    private static EntryService entryService;

    @BeforeClass
    public static void initEntryService() {
        ServiceFactory fact = ServiceFactories.getFactory();
        entryService = fact.getService(EntryService.class);
    }

    @Test
    public void testNonExistsFieldIncr() {
        
        long entryId = 1000000002;
        
        Field field1 = new Field().setName("author").setValue("55").setIndex(2);
        Field field2 = new Field().setName("title").setValue("hello").setIndex(2);
        Field field3 = new Field().setName("count").setValue(null);
        List<Field> fields = new ArrayList<Field>();
        fields.add(field1);
        fields.add(field2);
        fields.add(field3);
        
        ServiceFuture<Void> future = entryService.save(entryId, fields).submit();
        future.awaitUninterruptibly();
        Assert.assertTrue(future.isSuccess());
        
        ServiceFuture<Long> f = entryService.increaseField(entryId, "count");
        long ret = f.submit().awaitUninterruptibly().getContent();
        Assert.assertEquals(1, ret);
        
        Entry e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        long count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(1, count);
        
        //Entry e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        //System.out.println(e);
        
        f = entryService.increaseField(entryId, "count");
        f.setParam("dist", 2);
        ret = f.submit().awaitUninterruptibly().getContent();
        
        Assert.assertEquals(3, ret);
        
        e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(3, count);
        
    }
    
    @Test
    public void testIncr() {
        
        long entryId = 1000000001;
        
        Field field1 = new Field().setName("author").setValue("55").setIndex(2);
        Field field2 = new Field().setName("title").setValue("hello").setIndex(2);
        Field field3 = new Field().setName("count").setValue("1");
        
        List<Field> fields = new ArrayList<Field>();
        fields.add(field1);
        fields.add(field2);
        fields.add(field3);
        
        ServiceFuture<Void> future = entryService.save(entryId, fields).submit();
        future.awaitUninterruptibly();
        Assert.assertTrue(future.isSuccess());
        
        ServiceFuture<Long> f = entryService.increaseField(entryId, "count").submit();
        long ret = f.awaitUninterruptibly().getContent();
        Assert.assertEquals(2, ret);
        
        Entry e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        long count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(2, count);
        
        f = entryService.increaseField(entryId, "count");
        f.setParam("dist", 2);
        ret = f.submit().awaitUninterruptibly().getContent();
        
        Assert.assertEquals(4, ret);
        e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(4, count);
    }
    
    @Test
    public void testDecr() {
        
        long entryId = 100000000l;
        
        Field field1 = new Field().setName("author").setValue("55").setIndex(2);
        Field field2 = new Field().setName("title").setValue("hello").setIndex(2);
        Field field3 = new Field().setName("count").setValue("10");
        
        List<Field> fields = new ArrayList<Field>();
        fields.add(field1);
        fields.add(field2);
        fields.add(field3);
        
        ServiceFuture<Void> future = entryService.save(entryId, fields).submit();
        future.awaitUninterruptibly();
        Assert.assertTrue(future.isSuccess());
        
        ServiceFuture<Long> f = entryService.decreaseField(entryId, "count").submit();
        long ret = f.awaitUninterruptibly().getContent();
        Assert.assertEquals(9, ret);
        
        Entry e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        long count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(9, count);
        
        f = entryService.decreaseField(entryId, "count");
        f.setParam("dist", 2);
        ret = f.submit().awaitUninterruptibly().getContent();
        
        Assert.assertEquals(7, ret);
        
        e = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        count = e.getFieldValue("count", Long.class);
        Assert.assertEquals(7, count);
    }
    
    /*@Test
    public void testDeleteField() {
        
        long entryId = 3602888497993947338l;
        //long entryId = 3530830903956019943l;
        Entry entry = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        
        System.out.println(entry.toInfoString(entry.getFields()));
        List<Field> fields = entry.getFields();
        
        
        
        for (Field f : fields) {
            if(f.getName().equals("tag")) {
                System.out.println(f);
                f.setValue(null);
            }
        }
        
        //fields.add(new Field("tag", "18599", Field.INDEX_TRUE));
        //fields.add(new Field("tag", "19205", Field.INDEX_TRUE));
        entryService.save(entryId, fields).submit().awaitUninterruptibly();
        entry = entryService.get(entryId).submit().awaitUninterruptibly().getContent();
        System.out.println(entry.toInfoString(entry.getFields()));
    }*/
    
}

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
