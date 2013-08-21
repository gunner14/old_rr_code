package com.renren.entries.xoa.api;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.renren.entries.model.Tag;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.xiaonei.commons.gid.util.Type;

@Ignore
public class TagServiceTest {

    private static TagService tagService;

    @BeforeClass
    public static void initEntryService() {
        ServiceFactory fact = ServiceFactories.getFactory();
        tagService = fact.getService(TagService.class);
    }

    @Test
    public void testGet() {
        
        String word = "美女";
        
        ServiceFuture<Tag> f = tagService.get(Type.XIAOZHAN, word, true).submit()
                .awaitUninterruptibly();
        Assert.assertTrue(f.isSuccess());
        Tag tag = f.getContent();
        Assert.assertEquals(word, tag.getValue());
    }
    
    @Test
    public void testInc() {
        String word = "测试专用";
        ServiceFuture<Tag> f = tagService.get(Type.XIAOZHAN, word, true).submit()
                .awaitUninterruptibly();
        Assert.assertTrue(f.isSuccess());
        Tag tag = f.getContent();
        int count = tag.getTagCount();
        
        ServiceFuture<Tag> f2 = tagService.increaseTagCount(tag.getId(), 2).submit().awaitUninterruptibly();
        Assert.assertTrue(f2.isSuccess());
        
        f = tagService.get(Type.XIAOZHAN, word, true).submit()
        .awaitUninterruptibly();
        Assert.assertTrue(f.isSuccess());
        Tag tag2 = f.getContent();
        
        Assert.assertEquals(count + 2, tag2.getTagCount());
    }
}

