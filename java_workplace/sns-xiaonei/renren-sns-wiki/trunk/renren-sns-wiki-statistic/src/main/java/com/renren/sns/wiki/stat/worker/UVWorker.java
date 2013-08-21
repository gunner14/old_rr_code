package com.renren.sns.wiki.stat.worker;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.wiki.stat.DefaultFileWalker;
import com.renren.sns.wiki.stat.FileWalker;
import com.renren.sns.wiki.stat.LogProcessor;
import com.renren.sns.wiki.stat.processor.UVProcessor;

/**
 * @author yi.li@renren-inc.com since 2012-4-26
 */
public class UVWorker {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.print("Usage: $> java UVWorker <logpath>");
            System.exit(1);
        }

        String filePath = args[0];

        FileWalker walker = new DefaultFileWalker();
        UVProcessor processor = new UVProcessor();
        List<LogProcessor> pList = new ArrayList<LogProcessor>();
        pList.add(processor);
        walker.setProcessors(pList);
        walker.setFile(filePath);
        walker.process();
        System.exit(1);
    }
}
