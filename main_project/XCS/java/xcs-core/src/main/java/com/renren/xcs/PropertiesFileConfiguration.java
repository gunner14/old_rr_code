package com.renren.xcs;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.StringTokenizer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 基于树形文件的Configuration实现
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class PropertiesFileConfiguration implements Configuration {

    /**
     * root字段的字面量
     * */
    public final static String ROOT_FIELD = "root";

    /**
     * server username字段的字面量
     * */
    public final static String SERVER_USERNAME_FIELD = "server.username";

    /**
     * server password字段的字面量
     * */
    public final static String SERVER_PASSWORD_FIELD = "server.password";

    /**
     * server 接受报警的email字段的字面量
     * */
    public final static String SERVER_ALARMEMAILS_FIELD = "server.alarm.emails";

    /**
     * server 接受报警的电话字段的字面量
     * */
    public final static String SERVER_ALARMPHONES_FIELD = "server.alarm.phones";

    /**
     * client username字段的字面量
     * */
    public final static String CLIENT_USERNAME_FIELD = "client.username";

    /**
     * client password字段的字面量
     * */
    public final static String CLIENT_PASSWORD_FIELD = "client.password";

    /**
     * client 接受报警的email列表的字段的字面量
     * */
    public final static String CLIENT_ALARMEMAILS_FIELD = "client.alarm.emails";

    /**
     * client 接受报警电话字段的字面量
     * */
    public final static String CLIENT_ALARMPHONES_FIELD = "client.alarm.phones";

    /**
     * cluster字段的字面量
     * */
    public final static String CLUSTER_FIELD = "cluster";

    private static Log logger = LogFactory.getLog(PropertiesFileConfiguration.class.getName());

    private String root;

    private String clientUsername;

    private String clientPassword;

    private List<String> clientAlarmPhones;

    private List<String> clientAlarmEmails;

    private String serverUsername;

    private String serverPassword;

    private List<String> serverAlarmPhones;

    private List<String> serverAlarmEmails;

    private String cluster;

    public PropertiesFileConfiguration(String filePath) throws InvalidConfigurationException {
        Properties props = new Properties();
        URI fileUri = new File(filePath).toURI();
        try {
            props.load(new FileInputStream(new File(fileUri)));
        } catch (FileNotFoundException e) {
            throw new InvalidConfigurationException(fileUri.toString() + " is not found.", e);
        } catch (IOException e) {
            throw new InvalidConfigurationException("Can not load config file: "
                    + fileUri.toString(), e);
        }
        initConfiguration(props);
    }

    public PropertiesFileConfiguration(Properties props) throws InvalidConfigurationException {
        initConfiguration(props);
    }

    private void initConfiguration(Properties props) throws InvalidConfigurationException {
        if (logger.isInfoEnabled()) {
            StringBuffer sb = new StringBuffer();
            sb.append("Loaded config: [ ");
            for (Entry<?, ?> e : props.entrySet()) {
                if (CLIENT_PASSWORD_FIELD.equals(e.getKey().toString())
                        || SERVER_PASSWORD_FIELD.equals(e.getKey().toString())) {
                    sb.append(e.getKey() + ":***").append(", ");
                } else {
                    sb.append(e.getKey() + ":" + e.getValue()).append(", ");
                }
            }
            sb.append("]");
            logger.info(sb.toString());
        }

        if (!props.containsKey(ROOT_FIELD)) {
            throw new InvalidConfigurationException("Config is missing required field '"
                    + ROOT_FIELD + "'");
        }
        if (!props.containsKey(CLUSTER_FIELD)) {
            throw new InvalidConfigurationException("Config is missing required field '"
                    + CLUSTER_FIELD + "'");
        }

        this.root = props.getProperty(ROOT_FIELD);
        this.clientUsername = props.getProperty(CLIENT_USERNAME_FIELD);
        this.clientPassword = props.getProperty(CLIENT_PASSWORD_FIELD);
        this.serverUsername = props.getProperty(SERVER_USERNAME_FIELD);
        this.serverPassword = props.getProperty(SERVER_PASSWORD_FIELD);

        this.clientAlarmPhones = tokenizeToStringList(props.getProperty(CLIENT_ALARMPHONES_FIELD));
        this.clientAlarmEmails = tokenizeToStringList(props.getProperty(CLIENT_ALARMEMAILS_FIELD));

        this.serverAlarmPhones = tokenizeToStringList(props.getProperty(SERVER_ALARMPHONES_FIELD));
        this.serverAlarmEmails = tokenizeToStringList(props.getProperty(SERVER_ALARMEMAILS_FIELD));

        this.cluster = props.getProperty(CLUSTER_FIELD);
    }

    public String getRoot() {
        return root;
    }

    public String getClientUsername() {
        return clientUsername;
    }

    public String getClientPassword() {
        return clientPassword;
    }

    public String getServerUsername() {
        return serverUsername;
    }

    public String getServerPassword() {
        return serverPassword;
    }

    public String getCluster() {
        return cluster;
    }

    public List<String> getClientAlarmPhones() {
        return clientAlarmPhones;
    }

    public List<String> getClientAlarmEmails() {
        return clientAlarmEmails;
    }

    public List<String> getServerAlarmPhones() {
        return serverAlarmPhones;
    }

    public List<String> getServerAlarmEmails() {
        return serverAlarmEmails;
    }

    private List<String> tokenizeToStringList(String str) {

        List<String> tokens = Collections.emptyList();
        if (str == null) {
            return tokens;
        }
        StringTokenizer st = new StringTokenizer(str, ",");
        tokens = new ArrayList<String>();
        while (st.hasMoreTokens()) {
            tokens.add(st.nextToken());
        }
        return tokens;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((clientPassword == null) ? 0 : clientPassword.hashCode());
        result = prime * result + ((clientUsername == null) ? 0 : clientUsername.hashCode());
        result = prime * result + ((cluster == null) ? 0 : cluster.hashCode());
        result = prime * result + ((root == null) ? 0 : root.hashCode());
        result = prime * result + ((serverPassword == null) ? 0 : serverPassword.hashCode());
        result = prime * result + ((serverUsername == null) ? 0 : serverUsername.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null) return false;
        if (getClass() != obj.getClass()) return false;
        PropertiesFileConfiguration other = (PropertiesFileConfiguration) obj;
        if (clientPassword == null) {
            if (other.clientPassword != null) return false;
        } else if (!clientPassword.equals(other.clientPassword)) return false;
        if (clientUsername == null) {
            if (other.clientUsername != null) return false;
        } else if (!clientUsername.equals(other.clientUsername)) return false;
        if (cluster == null) {
            if (other.cluster != null) return false;
        } else if (!cluster.equals(other.cluster)) return false;
        if (root == null) {
            if (other.root != null) return false;
        } else if (!root.equals(other.root)) return false;
        if (serverPassword == null) {
            if (other.serverPassword != null) return false;
        } else if (!serverPassword.equals(other.serverPassword)) return false;
        if (serverUsername == null) {
            if (other.serverUsername != null) return false;
        } else if (!serverUsername.equals(other.serverUsername)) return false;
        return true;
    }

}
