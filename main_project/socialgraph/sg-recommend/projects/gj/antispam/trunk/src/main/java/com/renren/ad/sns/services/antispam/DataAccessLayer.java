package com.renren.ad.sns.services.antispam;

import java.net.UnknownHostException;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.google.code.morphia.query.Query;
import com.google.code.morphia.query.UpdateOperations;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.renren.ad.sns.data.TopicContent;

public class DataAccessLayer {
    static Datastore datastore;


    static {
        // format = new SimpleDateFormat("yyMMddHHmmss");
        Morphia morphia = new Morphia();
        Mongo mongo = null;
        try {
            mongo = new Mongo("db1.jie.d.xiaonei.com", 62166);
        } catch (UnknownHostException e) {
            e.printStackTrace();
            System.exit(1);
        } catch (MongoException e) {
            e.printStackTrace();
            System.exit(1);
        }
        datastore = new DatastoreImpl(morphia, mongo, "sns_online", "gj",
                "dec2011online4nb".toCharArray());
    }
    
    public void monthday(long topicid , long feedid ){
        Query<TopicContent> query = datastore.createQuery(TopicContent.class).field(
                "topicId").equal(topicid);
        UpdateOperations<TopicContent> ops = datastore.createUpdateOperations(
                TopicContent.class).removeAll("contents", feedid);
        datastore.update(query, ops);
    }
    
    public static void main (String  [] args){
//        new DataAccessLayer().monthday(1205031330114533102l, 1205091440413562063l);
        new DataAccessLayer().monthday(1205031330114533102l, 1205091417521397272l);
        
    }
}
