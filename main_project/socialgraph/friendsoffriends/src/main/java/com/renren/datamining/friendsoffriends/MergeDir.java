package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hdfs.DistributedFileSystem;
import org.apache.hadoop.io.IOUtils;

public class MergeDir {

  /**
   * @param args
   * @throws IOException
   */
  public static void main(String[] args) throws IOException {
    FileSystem fs = DistributedFileSystem.get(new Configuration());
    List<Path> srcs = new ArrayList<Path>();
    for (int i = 0; i < args.length - 1; ++i) {
      Path path = new Path(args[i]);
      FileStatus status = fs.getFileStatus(path);
      if (status.isDir()) {
        FileStatus[] children = fs.listStatus(path);
        for (FileStatus child : children) {
          if (!child.isDir()) {
            srcs.add(child.getPath());
          }
        }
      }
    }

    Path target = new Path(args[args.length - 1]);
    FSDataOutputStream out = fs.create(target, true);
    for (Path src : srcs) {
      IOUtils.copyBytes(fs.open(src), out, 64 * 1024 * 1024);
    }
    out.flush();
    out.close();
  }
}
