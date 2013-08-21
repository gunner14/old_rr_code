package xiaonei.commons.jade.example.dao;

import java.util.Date;

/**
 * Xiaonei-Commons-Jade 演示: LogInfo - Bean 对象
 * 
 * @author han.liao
 */
public class LogInfo {

    private int userId;

    private String message;

    private Date timestamp;

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Date getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Date timestamp) {
        this.timestamp = timestamp;
    }

    @Override
    public String toString() {
        return "LogInfo [userId=" + userId + ", message=" // NL
                + message + ", timestamp=" + timestamp + "]";
    }
}
