package com.renren.sns.wiki.stat;

import java.util.List;

/**
 * @author yi.li@renren-inc.com since 2012-4-23
 * 
 */
public interface FileWalker {

    public void setFile(String filePath);

    public void setProcessors(List<LogProcessor> processors);

    public void process();
}
