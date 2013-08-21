package xiaonei.commons.jade.example.dao;

import java.util.Date;
import java.util.List;

import net.paoding.rose.jade.jadeinterface.annotation.Dao;
import net.paoding.rose.jade.jadeinterface.annotation.SQL;
import net.paoding.rose.jade.jadeinterface.annotation.SQLParam;

/**
 * Xiaonei-Commons-Jade 日志演示: LoggingDao 类
 * 
 * <pre>
 * +-----------+-----------+------+-----+-------------------+-------+
 * | Field     | Type      | Null | Key | Default           | Extra |
 * +-----------+-----------+------+-----+-------------------+-------+
 * | user_id   | int(11)   | NO   |     |                   |       |
 * | message   | text      | NO   |     |                   |       |
 * | timestamp | timestamp | NO   |     | CURRENT_TIMESTAMP |       |
 * +-----------+-----------+------+-----+-------------------+-------+
 * 
 * CREATE TABLE contact_log (
 *     user_id              INTEGER NOT NULL,
 *     message              TEXT NOT NULL,
 *     timestamp            TIMESTAMP NOT NULL
 * );
 * </pre>
 * 
 * @author han.liao
 */
@Dao(catalog = "example")
public interface LoggingDao {

    /**
     * 添加日志信息
     */
    @SQL("INSERT INTO contact_log (user_id, message, timestamp) "
            + "VALUES (:userId, :message, :timestamp)")
    public void log( // NL
            @SQLParam("userId") int userId, // NL
            @SQLParam("message") String message, // NL
            @SQLParam("timestamp") Date timestamp);

    /**
     * 列出某天的日志信息
     */
    @SQL("SELECT user_id, message, timestamp FROM contact_log WHERE timestamp < :currentTime "
            + "ORDER BY timestamp DESC")
    public List<LogInfo> listLogs(@SQLParam("currentTime") Date currentTime);

    /**
     * 列出某天的日志信息
     */
    @SQL("SELECT user_id, message, timestamp FROM contact_log WHERE timestamp < :currentTime "
            + "{AND user_id IN (:userIds)}? {AND message LIKE #(:keyword.trim())}? "
            + "ORDER BY timestamp DESC")
    public List<LogInfo> findLogs( // NL
            @SQLParam("userIds") Integer[] userIds, // NL
            @SQLParam("keyword") String keyword, // NL
            @SQLParam("currentTime") Date currentTime);

    /**
     * 删除某天的日志信息
     */
    @SQL("DELETE FROM contact_log WHERE timestamp < :currentTime")
    public void clearLogs(@SQLParam("currentTime") Date currentTime);
}
