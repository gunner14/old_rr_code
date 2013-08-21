package xiaonei.commons.jade.example;

import java.util.List;
import java.util.Map;

import junit.framework.TestCase;
import net.paoding.rose.app.RoseAppContext;
import xiaonei.commons.jade.example.dao.ContactInfo;
import xiaonei.commons.jade.example.dao.ExampleDao;
import xiaonei.commons.jade.example.dao.LoggingDao;
import xiaonei.commons.jade.example.dao.TestDao;

public class RoseAppContextTest extends TestCase {

    public void testRoseAppContext() {

        RoseAppContext rose = new RoseAppContext();

        System.out.println("======= exampleDao = " + rose.getBean(ExampleDao.class));
        System.out.println("======= loggingDao = " + rose.getBean(LoggingDao.class));
        System.out.println("======= testingDao = " + rose.getBean(TestDao.class));

        ExampleDao exampleDao = rose.getBean(ExampleDao.class);

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
