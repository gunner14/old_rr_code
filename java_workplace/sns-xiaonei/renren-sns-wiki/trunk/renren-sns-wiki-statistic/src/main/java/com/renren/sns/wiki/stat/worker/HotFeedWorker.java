package com.renren.sns.wiki.stat.worker;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.wiki.stat.DefaultFileWalker;
import com.renren.sns.wiki.stat.FileWalker;
import com.renren.sns.wiki.stat.LogProcessor;
import com.renren.sns.wiki.stat.processor.HotFeedProcessor;

/**
 * 热门新鲜事Work。用来包装包装processor来完成热门新鲜事的处理
 * 
 * @TODO 暂时采用这种结果今后根据业务的深入可以考虑将该流程抽象出来通过配置的方式完成，<br>
 *       让其能够迁移至hudson
 * 
 * @author yi.li@renren-inc.com since 2012-4-26
 * 
 */
public class HotFeedWorker {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.print("Usage: $> java HotFeedWorker <logpath>");
            System.exit(1);
        }

        String filePath = args[0];

        FileWalker walker = new DefaultFileWalker();
        HotFeedProcessor processor = new HotFeedProcessor();
        List<LogProcessor> pList = new ArrayList<LogProcessor>();
        pList.add(processor);
        walker.setProcessors(pList);
        walker.setFile(filePath);
        walker.process();
        System.exit(1);
    }

}
