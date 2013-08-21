package xiaonei.commons.jade.example.controllers;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import net.paoding.rose.jade.jadeinterface.Identity;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.ReqMapping;

import org.springframework.beans.factory.annotation.Autowired;

import xiaonei.commons.jade.example.dao.ContactInfo;
import xiaonei.commons.jade.example.dao.ExampleDao;
import xiaonei.commons.jade.example.dao.LogInfo;
import xiaonei.commons.jade.example.dao.LoggingDao;

/**
 * Xiaonei-Commons-Jade 演示: ExampleController 类
 * 
 * @author han.liao
 */
public class ExampleController {

    @Autowired
    protected ExampleDao exampleDao;

    @Autowired
    protected LoggingDao loggingDao;

    /**
     * 查找联系人信息
     */
    public String find(Invocation inv, // NL 
            @Param("userid") String userIdString, // NL 
            @Param("name") String name, // NL
            @Param("phonecode") String phoneCode) {

        ArrayList<Integer> userIds = new ArrayList<Integer>();

        for (String userId : userIdString.split("\\s*,\\s*")) {

            try {
                userIds.add(Integer.valueOf(userId));

            } catch (NumberFormatException e) {
                // Ignoring exception
            }
        }

        if (name != null && !name.isEmpty()) {
            name = '%' + name + '%';
        }

        if (phoneCode != null && !phoneCode.trim().isEmpty()) {
            phoneCode = '%' + phoneCode + '%';
        }

        List<ContactInfo> list = exampleDao.findContact(userIds.toArray(new Integer[0]), // Nl 
                name, phoneCode);

        // 效果相同： inv.addModel("contactInfoList", list);
        inv.addModel(list);

        // 传入列表大小
        inv.addModel("contactInfoSize", list.size());

        // 跳转到: webapp/views/list.jsp
        return "list";
    }

    /**
     * 列出联系人信息
     */
    public String list(Invocation inv, // NL 
            @Param("userid") int userId) {

        // 传入列表大小
        inv.addModel("contactInfoSize", exampleDao.getContactCount(userId));

        List<ContactInfo> list = exampleDao.listContact(userId);

        // 效果相同： inv.addModel("contactInfoList", list);
        inv.addModel(list);

        // 跳转到: webapp/views/list.jsp
        return "list";
    }

    /**
     * 显示联系人信息
     */
    public String info(Invocation inv, // NL 
            @Param("id") int id) {

        ContactInfo contactInfo = exampleDao.getContact(id);

        // 效果相同： inv.addModel("contactInfo", contactInfo);
        inv.addModel(contactInfo);

        return "info";
    }

    /**
     * 添加联系人信息
     */
    public String insert(Invocation inv, // NL 
            @Param("userid") int userId, // NL 
            @Param("name") String name, // NL
            @Param("phonecode") String phoneCode) {

        // 检查记录重复
        if (exampleDao.isContactExists(phoneCode)) {
            inv.addFlash("contactWarning", "提示: 号码 " + phoneCode + " 已经存在!");
        }

        Date currentTime = new Date();

        // 增加新纪录
        // ContactInfo contactInfo = new ContactInfo();
        // contactInfo.setUserId(userId);
        // contactInfo.setName(name);
        // contactInfo.setPhoneCode(phoneCode);
        // contactInfo.setCreateTime(currentTime);
        // int id = exampleDao.insertContact(contactInfo).intValue();

        // 增加新记录
        int id = exampleDao.insertContact(userId, name, phoneCode, currentTime).intValue();

        // 写入日志
        loggingDao.log(userId, "Insert contact, generated id is: " + id, currentTime);

        // 重定向到: webapp/views/list.jsp
        return "r:list?userid=" + userId;
    }

    /**
     * 批量添加联系人的表单
     */
    public String batch(@Param("userid") int userId) {

        return "batch";
    }

    /**
     * 批量添加联系人
     */
    @ReqMapping(path = "/batch_insert")
    public String batchInsert(Invocation inv, // NL 
            @Param("userid") int userId) {

        Date currentTime = new Date();

        List<ContactInfo> contactList = new ArrayList<ContactInfo>();

        for (int i = 1; i <= 5; i++) {
            String name = inv.getRequest().getParameter("name" + i);
            String phoneCode = inv.getRequest().getParameter("phonecode" + i);

            if (!name.trim().isEmpty() && !phoneCode.trim().isEmpty()) {
                ContactInfo contactInfo = new ContactInfo();
                contactInfo.setUserId(userId);
                contactInfo.setName(name);
                contactInfo.setPhoneCode(phoneCode);
                contactInfo.setCreateTime(currentTime);

                contactList.add(contactInfo);
            }
        }

        // 批量增加新记录
        Identity[] identities = exampleDao.batchInsertContact(contactList);

        // 写入日志
        loggingDao.log(userId, "Batch insert contact, generated id is: "
                + Arrays.toString(identities), currentTime);

        // 重定向到: webapp/views/list.jsp
        return "r:list?userid=" + userId;
    }

    /**
     * 更新联系人信息
     */
    public String update(@Param("id") int id, // NL 
            @Param("userid") int userId, // NL 
            @Param("name") String name, // NL
            @Param("phonecode") String phoneCode) {

        // 更新纪录
        exampleDao.updateContact(id, name, phoneCode);

        // 写入日志
        loggingDao.log(userId, "Update contact #" + id, new Date());

        // 重定向到: webapp/views/list.jsp
        return "r:list?userid=" + userId;
    }

    /**
     * 删除联系人信息
     */
    public String delete(@Param("id") int id, // NL
            @Param("userid") int userId) {

        // 删除纪录
        exampleDao.deleteContact(id);

        // 写入日志
        loggingDao.log(userId, "Delete contact #" + id, new Date());

        // 重定向到: webapp/views/list.jsp
        return "r:list?userid=" + userId;
    }

    /**
     * 写入日志信息
     */
    @ReqMapping(path = "/write_log")
    public String writeLog(@Param("userId") int userId, // NL
            @Param("message") String message, // NL
            @Param("timestamp") String timestamp) throws ParseException {

        Date time = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss").parse(timestamp);

        // 写入日志
        loggingDao.log(userId, message, time);

        return "r:list_log";
    }

    /**
     * 列出当天的日志信息
     */
    @ReqMapping(path = "/list_log")
    public String listLog(Invocation inv, // NL
            @Param("timestamp") String timestamp) throws ParseException {

        Calendar calendar = Calendar.getInstance();

        if (timestamp != null && !timestamp.isEmpty()) {
            calendar.setTime(new SimpleDateFormat("yyyy-MM-dd").parse(timestamp));
        }

        calendar.set(Calendar.HOUR_OF_DAY, 23);
        calendar.set(Calendar.MINUTE, 59);
        calendar.set(Calendar.SECOND, 59);

        List<LogInfo> list = loggingDao.listLogs(calendar.getTime());

        // 效果相同： inv.addModel("logInfoList", list);
        inv.addModel(list);

        return "logs";
    }

    /**
     * 查找当天的日志信息
     */
    @ReqMapping(path = "/find_log")
    public String findLog(Invocation inv, // NL
            @Param("userid") String userIdString, // NL
            @Param("keyword") String keyword, // NL
            @Param("timestamp") String timestamp) throws ParseException {

        Calendar calendar = Calendar.getInstance();

        if (timestamp != null && !timestamp.isEmpty()) {
            calendar.setTime(new SimpleDateFormat("yyyy-MM-dd").parse(timestamp));
        }

        calendar.set(Calendar.HOUR_OF_DAY, 23);
        calendar.set(Calendar.MINUTE, 59);
        calendar.set(Calendar.SECOND, 59);

        ArrayList<Integer> userIds = new ArrayList<Integer>();

        for (String userId : userIdString.split("\\s*,\\s*")) {

            try {
                userIds.add(Integer.valueOf(userId));

            } catch (NumberFormatException e) {
                // Ignoring exception
            }
        }

        if (keyword != null && !keyword.trim().isEmpty()) {
            keyword = '%' + keyword + '%';
        }

        List<LogInfo> list = loggingDao.findLogs(userIds.toArray(new Integer[0]), // NL
                keyword, calendar.getTime());

        // 效果相同： inv.addModel("logInfoList", list);
        inv.addModel(list);

        // 记录时间
        inv.addModel("timestamp", new SimpleDateFormat("yyyy-MM-dd").format(calendar.getTime()));

        return "logs";
    }

    /**
     * 删除当天的日志信息
     */
    @ReqMapping(path = "/clear_log")
    public String clearLog() {

        Calendar calendar = Calendar.getInstance();

        calendar.set(Calendar.HOUR_OF_DAY, 23);
        calendar.set(Calendar.MINUTE, 59);
        calendar.set(Calendar.SECOND, 59);

        // 删除日志
        loggingDao.clearLogs(calendar.getTime());

        return "r:list_log";
    }
}
