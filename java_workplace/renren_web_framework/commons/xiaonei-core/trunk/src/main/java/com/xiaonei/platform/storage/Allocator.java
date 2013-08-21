package com.xiaonei.platform.storage;

import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import java.util.Timer;

public class Allocator {

    private Domain domain;

    private Timer timer;

    private static Allocator instance = new Allocator(new DatabaseHelperImpl());

    private static final String FILE_SEPARATOR = "/";

    public static Allocator getInstance() {
        return instance;
    }

    /*  private static Allocator instance = Allocator
                .initialize(new DatabaseHelperImpl());*/

    /*  public static Allocator initialize(DatabaseHelper ds) {
            try {
                return new Allocator(ds);
            } catch (Exception e) {
                e.printStackTrace();
            }
            return null;
        }*/

    /*  public static Allocator getInstance() {
            if (null == instance) {
                System.err
                        .println("!!!!!!!!!!!! Allocator getInstance will return null.");
            }
            return instance;
        }
        */

    static final String KAIXIN_PREFIX = "kx/";

    private Allocator(DatabaseHelper ds) {
		try {
			domain = new Domain(ds);
			timer = new Timer(true);
			timer.schedule(new LoadNodeTask(domain),
					Constants.TRAP_INTERVAL * 1000,
					Constants.TRAP_INTERVAL * 1000);
		} catch (Throwable e) {
			e.printStackTrace();
		}
    }

    public void loadConfig() throws Exception {
        domain.loadConfig();
    }

    public String allocate(File source, String pattern, FileHelper helper) {
        if (source == null || pattern == null || helper == null) {
            return null;
        }

        for (int count = 0; count < 3; ++count) {
            Group group = domain.getWritableGroup(Constants.NODE_TYPE_NORMAL);
            if (group == null) {
                return null;
            }
            if (allocate(source, pattern, helper, group)) {
                return group.getName() + FILE_SEPARATOR + pattern;
            }
        }

        return null;
    }

    public Group allocateGroup(int type) {
        return domain.getWritableGroup(type);
    }

    public void free(String pattern) {
        String groupName = pattern.substring(0, pattern.indexOf(FILE_SEPARATOR));
        if (groupName == null) {
            return;
        }
        Group group = domain.getGroup(groupName);
        if (group == null) {
            return;
        }
        String fileName = pattern.substring(pattern.indexOf(FILE_SEPARATOR) + 1);
        if (fileName == null) {
            return;
        }
        ArrayList<Node> nodes = group.getNodes();
        for (int i = 0; i < nodes.size(); ++i) {
            Node node = nodes.get(i);
            File file = new File(node.getPath() + FILE_SEPARATOR + fileName);
            file.delete();
        }
    }

    public String locate(String pattern) {
        // we think the url of this image is like
        // "GROUP/photo/2007xxxx/xxxx.jpg,
        // so if it does not start with GROUP, just return null.

        Group group = null;

        boolean isKaxin = false;
        if (pattern.startsWith(KAIXIN_PREFIX)) {
            pattern = pattern.substring(KAIXIN_PREFIX.length());
            isKaxin = true;
        }
        int pos = pattern.indexOf(FILE_SEPARATOR);
        if (pos == -1) {
            group = domain.getGroup(pattern);
        } else {
            group = domain.getGroup(pattern.substring(0, pos));
        }
        if (group == null) {
            return null;
        }
        String fileName = pattern.substring(pattern.indexOf(FILE_SEPARATOR) + 1);
        if (fileName == null) {
            System.out.println("storage fileName null [" + pattern + "]");
            return null;
        }
        // check whether we do something wrong that upload the picture to
        // head storage, if so, recorrect it
        String serverName = group.getReadableNode(fileName).getDomain();
        if (isKaxin) {
            serverName = serverName.replaceAll("xnimg.cn", "rrimg.com");
        }
        if (group.getName().startsWith("hd") || group.getName().startsWith("kxhd")) {
            // :(, we have to add to subdirectory to recorrect it
            return "http://" + serverName + "/photos/" + group.getName() + "/" + fileName;
        } else if (group.getName().startsWith("fmn") || group.getName().startsWith("kxfmn")) {
            return "http://" + serverName + "/" + pattern;
        } else {
            return "http://" + serverName + "/" + fileName;
        }
    }

    public String locate(String pattern, int type) {
        boolean isKaxin = false;
        if (pattern.startsWith(KAIXIN_PREFIX)) {
            pattern = pattern.substring(KAIXIN_PREFIX.length());
            isKaxin = true;
        }
        // we think the url of this image is like
        // "GROUP/photo/2007xxxx/xxxx.jpg,
        // so if it does not start with GROUP, just return null.
        Group group = null;
        int pos = pattern.indexOf(FILE_SEPARATOR);
        if (pos == -1) {
            group = domain.getGroup(pattern);
        } else {
            group = domain.getGroup(pattern.substring(0, pos));
        }
        if (type == Constants.NODE_TYPE_NORMAL) {
            if (group == null) {
                return null;
            }
            String fileName = pattern.substring(pattern.indexOf(FILE_SEPARATOR) + 1);
            if (fileName == null) {
                return null;
            }
            String serverName = group.getReadableNode(fileName).getDomain();
            if (isKaxin) {
                serverName = serverName.replaceAll("xnimg.cn", "rrimg.com");
            }
            // check whether we do something wrong that upload the picture to
            // head storage, if so, recorrect it
            if (group.getName().startsWith("hd") || group.getName().startsWith("kxhd")) {
                // :(, we have to add to subdirectory to recorrect it
                return "http://" + serverName + "/photos/" + group.getName() + "/" + fileName;
            } else if (group.getName().startsWith("fmn") || group.getName().startsWith("kxfmn")) {
                return "http://" + serverName + "/" + pattern;
            } else {
                return "http://" + serverName + "/" + fileName;
            }
        } else if (type == Constants.NODE_TYPE_HEAD) {
            if (group != null) {
                String serverName = group.getReadableNode(pattern).getDomain();
                if (isKaxin) {
                    serverName = serverName.replaceAll("xnimg.cn", "rrimg.com");
                }
                return "http://" + serverName + "/photos/" + pattern;
            } else {
                if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {// 一个久远的历史问题，这些文件迁移到静态文件服务器上
                    String url = "";
                    if (pattern.indexOf("head.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_head.gif";
                    else if (pattern.indexOf("main.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_main.gif";
                    else if (pattern.indexOf("tiny.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_tiny.gif";
                    else url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/"
                            + pattern;
                    return url;
                } else {
                    String url = "";
                    if (pattern.indexOf("head.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_head.gif";
                    else if (pattern.indexOf("main.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_main.gif";
                    else if (pattern.indexOf("tiny.jpg") > 0) url = com.xiaonei.platform.core.opt.OpiConstants.urlHead
                            + "/photos/0/0/men_tiny.gif";
                    else url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/"
                            + pattern;
                    return url;
                }

            }
        }
        return null;
    }

    public Group locateGroup(String pattern) {
        @SuppressWarnings("unused")
        boolean isKaxin = false;
        if (pattern.startsWith(KAIXIN_PREFIX)) {
            pattern = pattern.substring(KAIXIN_PREFIX.length());
            isKaxin = true;
        }
        int pos = pattern.indexOf(FILE_SEPARATOR);
        if (pos == -1) {
            return domain.getGroup(pattern);
        } else {
            return domain.getGroup(pattern.substring(0, pos));
        }
    }

    public boolean allocate(File source, String pattern, FileHelper writer, Group group) {
        @SuppressWarnings("unused")
        boolean isKaxin = false;
        if (pattern.startsWith(KAIXIN_PREFIX)) {
            pattern = pattern.substring(KAIXIN_PREFIX.length());
            isKaxin = true;
        }

        ArrayList<Node> nodes = group.getNodes();
        ArrayList<File> results = new ArrayList<File>();
        for (int i = 0; i < nodes.size(); ++i) {
            Node node = nodes.get(i);
            File target = new File(node.getPath() + FILE_SEPARATOR + pattern);
            File parent = target.getParentFile();
            if (!parent.exists()) {
                parent.mkdirs();
            }
            try {
                writer.saveAs(source, target);
                results.add(target);
            } catch (Exception e) {
                node.setTime(new Date(0));
                break;
            }
        }
        if (results.size() != nodes.size()) {
            for (int i = 0; i < results.size(); ++i) {
                results.get(i).delete();
            }
            return false;
        }
        return true;
    }

    public Domain getDomain() {
        return this.domain;
    }

    public static void main(String args[]) {
        Allocator.getInstance();

    }
}
