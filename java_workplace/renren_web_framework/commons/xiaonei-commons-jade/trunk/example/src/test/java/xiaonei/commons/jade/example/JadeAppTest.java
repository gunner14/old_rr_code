package xiaonei.commons.jade.example;

import java.util.List;
import java.util.Map;

import junit.framework.TestCase;
import net.paoding.rose.jade.jadeinterface.app.java.Jade;
import xiaonei.commons.jade.example.dao.ContactInfo;
import xiaonei.commons.jade.example.dao.ExampleDao;
import xiaonei.commons.jade.example.dao.LoggingDao;
import xiaonei.commons.jade.example.dao.TestDao;

import com.xiaonei.commons.jade.impl.cache.EhCacheProvider;
import com.xiaonei.commons.jade.impl.simple.SimpleDataAccessProvider;
import com.xiaonei.commons.jade.impl.xceadapter.provider.LocalDataAccessProvider;

public class JadeAppTest extends TestCase {

    public void testSimpleProvider() {

        Jade jade = new Jade();

        jade.setDataAccessProvider(new SimpleDataAccessProvider());
        // jade.setCacheProvider(new EhCacheProvider());

        System.out.println("======= exampleDao = " + jade.getDao(ExampleDao.class));
        System.out.println("======= loggingDao = " + jade.getDao(LoggingDao.class));
        System.out.println("======= testingDao = " + jade.getDao(TestDao.class));

        ExampleDao exampleDao = jade.getDao(ExampleDao.class);

        List<ContactInfo> list = exampleDao.listContact(0);
        System.out.println("list = " + list.toString());

        ContactInfo contactInfo = exampleDao.getContact(24);
        System.out.println("contactInfo = " + contactInfo);

        boolean exists = exampleDao.isContactExists("123");
        System.out.println("exists = " + exists);

        Map<String, ContactInfo> map = exampleDao.mapContact(0);
        System.out.println("map = " + map);
    }

    public void testLocalProvider() {

        Jade jade = new Jade();

        jade.setDataAccessProvider(new LocalDataAccessProvider());
        jade.setCacheProvider(new EhCacheProvider());

        System.out.println("======= exampleDao = " + jade.getDao(ExampleDao.class));
        System.out.println("======= loggingDao = " + jade.getDao(LoggingDao.class));
        System.out.println("======= testingDao = " + jade.getDao(TestDao.class));

        ExampleDao exampleDao = jade.getDao(ExampleDao.class);

        List<ContactInfo> list = exampleDao.listContact(0);
        System.out.println("list = " + list.toString());

        ContactInfo contactInfo = exampleDao.getContact(24);
        System.out.println("contactInfo = " + contactInfo);

        boolean exists = exampleDao.isContactExists("123");
        System.out.println("exists = " + exists);

        Map<String, ContactInfo> map = exampleDao.mapContact(0);
        System.out.println("map = " + map);
    }
}
