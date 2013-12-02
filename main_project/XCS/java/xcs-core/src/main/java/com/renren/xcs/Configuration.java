package com.renren.xcs;

import java.util.List;

/**
 * XCS配置的接口，其中存储了配置的root、username、password、集群连接串
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public interface Configuration {

    /**
     * 获取ServiceRegistry集群的连接串
     * 
     * @return 集群的连接串
     */
    public String getCluster();

    /**
     * 获取配置中的root
     * 
     * @return 获取配置中的root
     */
    public String getRoot();

    /**
     * 获取配置中的server username
     * 
     * @return 获取配置中的server username
     */
    public String getServerUsername();

    /**
     * 获取配置中的server password
     * 
     * @return 获取配置中的server password
     */
    public String getServerPassword();
    
    /**
     * 获取配置中的server接受报警的电话
     * 
     * @return 获取配置中的server password
     */
    public List<String> getServerAlarmPhones();

    /**
     * 获取配置中的server接受报警的邮箱
     * 
     * @return 获取配置中的server password
     */
    public List<String> getServerAlarmEmails();

    /**
     * 获取配置中的client username
     * 
     * @return 获取配置中的client username
     */
    public String getClientUsername();

    /**
     * 获取配置中的client password
     * 
     * @return 获取配置中的client password
     */
    public String getClientPassword();
    
    /**
     * 获取配置中的client接受报警的电话
     * 
     * @return 获取配置中的client password
     */
    public List<String> getClientAlarmPhones();

    /**
     * 获取配置中的client接受报警的邮箱
     * 
     * @return 获取配置中的client password
     */
    public List<String> getClientAlarmEmails();
}
