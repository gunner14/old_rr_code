package com.renren.ad.sns.services.antispam;

import java.net.UnknownHostException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.mongodb.Mongo;
import com.mongodb.MongoException;

public class GJDataStore {

    private static final Log LOG = LogFactory.getLog(GJDataStore.class);

    private Datastore datastore;

    private static GJDataStore instance = new GJDataStore(MongoConfig.getGJConfig());

    public GJDataStore(MongoConfig config) {
        Mongo mongo = null;
        try {
            mongo = new Mongo(config.getHost(), config.getPort());
        } catch (UnknownHostException e) {
            LOG.error(e, e);
        } catch (MongoException e) {
            LOG.error(e, e);
        }

        if (mongo != null) {
            datastore = new DatastoreImpl(new Morphia(), mongo, config.getDatabase(), config
                    .getUser(), config.getPassword().toCharArray());
        }
    }

    public static Datastore getDefaultDataStore() {
        return instance.getDatastore();
    }

    public Datastore getDatastore() {
        return datastore;
    }

    static class MongoConfig {

        private String host;

        private int port;

        private String database;

        private String user;

        private String password;

        public MongoConfig(String host, int port, String database, String user, String password) {
            super();
            this.host = host;
            this.port = port;
            this.database = database;
            this.user = user;
            this.password = password;
        }

        public static MongoConfig getGJConfig() {
            return new MongoConfig("db1.jie.d.xiaonei.com", 62166, "sns_online", "gj",
                    "dec2011online4nb");
        }

        public static MongoConfig getTestGJConfig() {
            return new MongoConfig("localhost", 27017, "test", "test", "test");
        }

        public String getHost() {
            return host;
        }

        public int getPort() {
            return port;
        }

        public String getDatabase() {
            return database;
        }

        public String getUser() {
            return user;
        }

        public String getPassword() {
            return password;
        }

    }

}
