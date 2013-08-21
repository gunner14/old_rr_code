package com.renren.entries.index.util;

import org.junit.Assert;
import org.junit.Test;

import com.renren.entries.model.Entry;
import com.renren.entries.model.Field;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-16 下午06:27:02
 */
public class EntryInfoEncoderTest {

    //TODO: 此测试用例请移动到model包去对Entry、Field进行测试
    @Test
    public void testEncode() {
        
        Field field1 = new Field().setName("tag").setValue("231232").setIndex(Field.INDEX_TRUE);
        Field field2 = new Field().setName("tag").setValue("a%fs$d=aba&").setIndex(Field.INDEX_FALSE);
        Field field3 = new Field().setName("author").setValue("53125").setIndex(Field.INDEX_TRUE);
        Field field4 = new Field().setName("title").setValue("he$llo$").setIndex(Field.INDEX_FALSE);
        Field field5 = new Field().setName("at").setValue("123123123").setIndex(Field.INDEX_TRUE);
     
        String ret = Entry.toInfoString(field1, field2, field3, field4, field5);
        
        //TODO完善测试用例
        Assert.assertNotNull(ret);
        
    }
}
