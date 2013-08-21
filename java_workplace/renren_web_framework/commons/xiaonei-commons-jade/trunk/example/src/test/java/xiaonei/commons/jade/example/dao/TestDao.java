package xiaonei.commons.jade.example.dao;

import net.paoding.rose.jade.jadeinterface.annotation.Dao;
import net.paoding.rose.jade.jadeinterface.annotation.SQL;

/**
 * 测试用的 Dao 类。
 * 
 * @author han.liao
 */
@Dao(catalog = "example")
public interface TestDao {

    /**
     * 列出所有的数据库。
     */
    @SQL("SHOW DATABASES")
    public String[] listAllDBs();

    /**
     * 列出所有的数据表。
     */
    @SQL("SHOW TABLES")
    public String[] listAllTables();
}
