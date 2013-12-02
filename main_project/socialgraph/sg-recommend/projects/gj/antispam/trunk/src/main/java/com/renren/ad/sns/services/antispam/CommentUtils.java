package com.renren.ad.sns.services.antispam;

import java.io.FileInputStream;
import java.net.UnknownHostException;
import java.util.Scanner;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.DatastoreImpl;
import com.google.code.morphia.Morphia;
import com.google.code.morphia.query.Query;
import com.google.code.morphia.query.UpdateOperations;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.renren.ad.sns.data.CombiComments;
import com.renren.ad.sns.data.Comment;
import com.renren.ad.sns.data.FwdComments;
import com.renren.ad.sns.data.LikeComments;

/**
 * @author socialgraph
 */
public class CommentUtils {
    static Datastore datastore;

    static Options opts;

    static {
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
        opts = new Options();
        opts.addOption("h", false, "print help for this application");
        opts.addOption("c", false, "with entry or feed's comments");
        opts.addOption("l", false, "with likeComments");
        opts.addOption("w", false, "with FwdComments");
        opts.addOption("f", true, "the file cantains comments' id");
    }

    private boolean fromEntry(String file) {
        boolean res = true;
        try {
            Scanner cin = new Scanner(new FileInputStream(file));
            int num = 0;
            while (cin.hasNext()) {
                long cid = cin.nextLong();
                delComment(cid);
                System.out.println("done for cid=" + cid);
                num++;
                if (num % 100 == 0) {
                    try {
                        Thread.sleep(2000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
            cin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    /**
     * delete goods' comment or feed's comment
     * 
     * @param commentId
     * @return
     */
    private boolean delComment(long commentId) {
        boolean res = false;
        try {

            Query<Comment> query = datastore.createQuery(Comment.class).field(
                    "commentId").equal(commentId);
            UpdateOperations<Comment> ops = datastore.createUpdateOperations(
                    Comment.class).set("state", -100);
            datastore.update(query, ops);
            Comment comment = query.get();

            long contentId = comment.getContentId();

            // delete entry's comment if ok
            Query<CombiComments> cquery = datastore.createQuery(
                    CombiComments.class).field("contentId").equal(contentId);
            UpdateOperations<CombiComments> cops = datastore.createUpdateOperations(
                                                                    CombiComments.class)
                                                            .removeAll(
                                                                    "comments",
                                                                    commentId);
            datastore.update(cquery, cops);

            // delete feed's comment if ok
            Query<CombiComments> fquery = datastore.createQuery(
                    CombiComments.class).field("contentId").equal(contentId);
            UpdateOperations<CombiComments> fops = datastore.createUpdateOperations(
                                                                    CombiComments.class)
                                                            .removeAll(
                                                                    "comments",
                                                                    commentId);
            datastore.update(fquery, fops);

            // delete like's comment if ok
            Query<LikeComments> lquery = datastore.createQuery(
                    LikeComments.class).field("contentId").equal(contentId);
            UpdateOperations<LikeComments> lops = datastore.createUpdateOperations(
                                                                   LikeComments.class)
                                                           .removeAll(
                                                                   "comments",
                                                                   commentId);
            datastore.update(lquery, lops);

            // delete fwd's comment if ok
            Query<FwdComments> fwdquery = datastore.createQuery(
                    FwdComments.class).field("contentId").equal(contentId);
            UpdateOperations<FwdComments> fwdops = datastore.createUpdateOperations(
                                                                    FwdComments.class)
                                                            .removeAll(
                                                                    "comments",
                                                                    commentId);
            datastore.update(fwdquery, fwdops);

            res = true;
        } catch (MongoException e) {
            e.printStackTrace();
        }
        return res;
    }

    public static void main(String[] args) {
        BasicParser parser = new BasicParser();
        try {
            CommandLine cl = parser.parse(opts, args);
            if (cl.hasOption("h")) {
                HelpFormatter hf = new HelpFormatter();
                hf.printHelp("Comment Utils", opts);
            }

            if (cl.hasOption("f")) {
                String file = cl.getOptionValue("f");
                System.out.println(file);
                CommentUtils commentUtils = new CommentUtils();
                if (cl.hasOption("c")) {
                    commentUtils.fromEntry(file);
                } else if (cl.hasOption("w")) {
                    // commentUtils.fromUids(file);
                } else if (cl.hasOption("l")) {
                    // commentUtils.fromUidAndGids(file);
                }
            } else {
                System.out.println("must specify the file containes comment ids ");
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

}