package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.util.Arrays;
import java.util.Calendar;
import java.util.List;

import javax.sql.DataSource;

import org.junit.BeforeClass;
import org.junit.Test;
import org.springframework.jdbc.core.JdbcTemplate;

import com.renren.ad.sns.util.db.PoolingDataSourceFactory;

public class SimpleMetadataProviderTest {

    private static SimpleMetadataProvider instance;

    public static final JdbcTemplate getGJJdbcTemplate() {
        DataSource dataSource = PoolingDataSourceFactory.createDataSource0("com.mysql.jdbc.Driver",
                "jdbc:mysql://db.jie.d.xiaonei.com:3306/uranus?user=gj&password=mysql@gj");
        return new JdbcTemplate(dataSource);
    }

    @BeforeClass
    public static void init() {
        instance = new SimpleMetadataProvider();
        instance.setJdbcTemplate(getGJJdbcTemplate());
        instance.setDatastore(CategoryCandidatePickerTest.getGJDataStore());
        instance.setDataPath("./test-data");
        instance.init();
    }

    @Test
    public void testGetUserMetaData() {
        long uid = 100000000L;
        // test a common user
        UserMetadata meta = instance.getUserMetaData(uid);
        assertNotNull(meta);
        assertEquals("100000000,26,0,8611,4,0,0,0,2,0,7,0,0,0,0,0,4,0,0", UserMetadata
                .contents2String(meta.getValues()));
        // test an unknown user
        uid = -1;
        meta = instance.getUserMetaData(uid);
        assertNotNull(meta);
        assertEquals("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0", UserMetadata.contents2String(meta
                .getValues()));
    }

    @Test
    public void testGetGoodsMetaDataLongLong() {
        long uid = 100000000L;
        long gid = 1206011450014284178L;
        // test a common goods
        GoodsMetadata meta = instance.getGoodsMetaData(uid, gid);
        assertNotNull(meta);
        assertEquals("100000001,80401,1880,0,0,0,0,0,1,0,0,0,0,0,1,3,0,0,0,0,0,0,0,0,0,0,1,3,0,0,"
                + Calendar.getInstance().get(Calendar.HOUR_OF_DAY) + ","
                + Calendar.getInstance().get(Calendar.DAY_OF_WEEK), GoodsMetadata
                .contents2String(meta.getValues()));
        // test an unknown goods
        uid = -1;
        gid = -1;
        meta = instance.getGoodsMetaData(uid, gid);
        assertNotNull(meta);
        assertEquals("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
                GoodsMetadata.contents2String(meta.getValues()));
    }

    @Test
    public void testGetGoodsMetaDataLongList() {
        long uid = 100000000L;
        List<Long> gids = Arrays.asList(1206011450014284178L, -1L);
        List<GoodsMetadata> result = instance.getGoodsMetaData(uid, gids);
        assertNotNull(result);
        assertEquals(2, result.size());
        assertEquals("100000001,80401,1880,0,0,0,0,0,1,0,0,0,0,0,1,3,0,0,0,0,0,0,0,0,0,0,1,3,0,0,"
                + Calendar.getInstance().get(Calendar.HOUR_OF_DAY) + ","
                + Calendar.getInstance().get(Calendar.DAY_OF_WEEK), GoodsMetadata
                .contents2String(result.get(0).getValues()));
        assertEquals("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
                GoodsMetadata.contents2String(result.get(1).getValues()));
    }
}
