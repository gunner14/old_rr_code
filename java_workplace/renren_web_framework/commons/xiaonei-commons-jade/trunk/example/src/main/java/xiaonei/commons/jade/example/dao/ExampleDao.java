package xiaonei.commons.jade.example.dao;

import java.util.Date;
import java.util.List;
import java.util.Map;

import net.paoding.rose.jade.jadeinterface.Identity;
import net.paoding.rose.jade.jadeinterface.annotation.Cache;
import net.paoding.rose.jade.jadeinterface.annotation.CacheDelete;
import net.paoding.rose.jade.jadeinterface.annotation.Dao;
import net.paoding.rose.jade.jadeinterface.annotation.MapKey;
import net.paoding.rose.jade.jadeinterface.annotation.SQL;
import net.paoding.rose.jade.jadeinterface.annotation.SQLParam;
import net.paoding.rose.jade.jadeinterface.annotation.UseMaster;

/**
 * Xiaonei-Commons-Jade 演示: ExampleDao 类
 * 
 * <pre>
 * +------------------+--------------+------+-----+-------------------+----------------+
 * | Field            | Type         | Null | Key | Default           | Extra          |
 * +------------------+--------------+------+-----+-------------------+----------------+
 * | id               | int(11)      | NO   | PRI | NULL              | auto_increment |
 * | user_id          | int(11)      | NO   |     |                   |                |
 * | name             | varchar(40)  | NO   |     |                   |                |
 * | phone_code       | varchar(255) | YES  |     | NULL              |                |
 * | create_timestamp | timestamp    | NO   |     | CURRENT_TIMESTAMP |                |
 * +------------------+--------------+------+-----+-------------------+----------------+
 * 
 * CREATE TABLE contact_info (
 *     id                   INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
 *     user_id              INTEGER NOT NULL,
 *     name                 VARCHAR(40) NOT NULL,
 *     phone_code           VARCHAR(255),
 *     create_timestamp     TIMESTAMP NOT NULL
 * );
 * </pre>
 * 
 * @author han.liao
 */
@Dao(catalog = "example")
public interface ExampleDao {

    /**
     * 检查联系人存在
     */
    @UseMaster
    @SQL("SELECT id FROM contact_info WHERE phone_code = #(:phoneCode.trim())")
    public boolean isContactExists(@SQLParam("phoneCode") String phoneCode);

    /**
     * 添加联系人信息
     */
    @SQL("INSERT INTO contact_info (user_id, name, phone_code, create_timestamp) "
            + "VALUES (:userId, :name, :phoneCode, :createTime)")
    public Identity insertContact( // NL
            @SQLParam("userId") int userId, // NL
            @SQLParam("name") String name, // NL
            @SQLParam("phoneCode") String phoneCode, // NL
            @SQLParam("createTime") Date createTime);

    /**
     * 添加联系人信息
     */
    @SQL("INSERT INTO contact_info (user_id, name, phone_code, create_timestamp) "
            + "VALUES (:contactInfo.userId, :contactInfo.name, "
            + ":contactInfo.phoneCode, :contactInfo.createTime)")
    public Identity insertContact(@SQLParam("contactInfo") ContactInfo contactInfo);

    /**
     * 批量添加联系人信息
     */
    @SQL("INSERT INTO contact_info (user_id, name, phone_code, create_timestamp) "
            + "VALUES (:contactInfo.userId, :contactInfo.name, "
            + ":contactInfo.phoneCode, :contactInfo.createTime)")
    public Identity[] batchInsertContact( // NL
            @SQLParam("contactInfo") List<ContactInfo> contactList);

    /**
     * 删除联系人信息
     */
    @SQL("DELETE FROM contact_info WHERE id = :id")
    @CacheDelete(pool = "contact_pool", key = "contact-:id")
    public void deleteContact(@SQLParam("id") int id);

    /**
     * 更新联系人信息
     */
    @SQL("UPDATE contact_info SET name = :name, phone_code = :phoneCode WHERE id = :id")
    @CacheDelete(pool = "contact_pool", key = "contact-:id")
    public void updateContact( // NL
            @SQLParam("id") int id, // NL
            @SQLParam("name") String name, // NL
            @SQLParam("phoneCode") String phoneCode);

    /**
     * 获取联系人信息
     */
    @UseMaster
    @SQL("SELECT id, user_id, name, phone_code, create_timestamp FROM contact_info WHERE id = :id")
    @Cache(pool = "contact_pool", key = "contact-:id", expiry = 600)
    public ContactInfo getContact(@SQLParam("id") int id);

    /**
     * 列出联系人信息
     */
    @SQL("SELECT id, user_id, name, phone_code, create_timestamp FROM contact_info WHERE user_id = :userId")
    @Cache(pool = "contact_pool", key = "contact-list-:userId", expiry = 600)
    public List<ContactInfo> listContact(@SQLParam("userId") int userId);

    /**
     * 获取联系人数量
     */
    @SQL("SELECT COUNT(*) FROM contact_info WHERE user_id = :userId")
    public int getContactCount(@SQLParam("userId") int userId);

    /**
     * 映射联系人信息
     */
    @SQL("SELECT id, user_id, name, phone_code, create_timestamp FROM contact_info WHERE user_id = :userId")
    @Cache(pool = "contact_pool", key = "contact-map-:userId", expiry = 600)
    @MapKey("phone_code")
    public Map<String, ContactInfo> mapContact(@SQLParam("userId") int userId);

    /**
     * 列出联系人信息
     */
    @SQL("SELECT id, user_id, name, phone_code, create_timestamp FROM contact_info WHERE id > 0"
            + "{ AND user_id IN (:userIds)}? { AND name LIKE #(:name)}?"
            + "{ AND phone_code LIKE #(:phoneCode.trim())}?")
    public List<ContactInfo> findContact( // NL
            @SQLParam("userIds") Integer[] userIds, // NL
            @SQLParam("name") String name, // NL
            @SQLParam("phoneCode") String phoneCode);
}
