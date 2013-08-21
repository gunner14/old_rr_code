package com.renren.sns.wiki.stat;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author yi.li@renren-inc.com since 2012-4-23
 * 
 */
public class DefaultFileWalker implements FileWalker {

    private Log logger = LogFactory.getLog(this.getClass());

    private static final int BUF_SIZE = 1024 * 1024 * 100;

    private String filePath;

    private List<LogProcessor> processors;

    public DefaultFileWalker() {

    }

    @Override
    public void process() {
        File processFile = new File(filePath);
        if (!processFile.exists()) {
            throw new RuntimeException("The file needed to process does not exists!" + filePath);
        }

        BufferedReader bufReader = null;
        try {
            boolean isSucc = processFile.setReadOnly();
            if (!isSucc) {
                logger.warn("file can not be set to read only, filename: " + filePath);
            }

            bufReader = new BufferedReader(new InputStreamReader(new FileInputStream(processFile)),
                    BUF_SIZE);
            String line = bufReader.readLine();
            while (line != null) {
                try {
                    // process log
                    String[] params = line.split("\\|\\|");
                    if (logger.isDebugEnabled()) {
                        logger.debug("Splited log: " + Arrays.toString(params));
                    }

                    if (params.length < 5) {
                        throw new RuntimeException("log format error! " + line);
                    }

                    if (null == processors || processors.size() == 0) {
                        continue;
                    }

                    processLog(params);

                    line = bufReader.readLine();
                } catch (Exception e) {
                    e.printStackTrace();
                    line = bufReader.readLine();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            // close buffer
            if (bufReader != null) {
                try {
                    bufReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            try {
                boolean isSucc = processFile.setWritable(true);
                if (!isSucc) {
                    System.out.println("Fail to change log file to writable" + filePath);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            finishProcess();
        }
    }

    private void processLog(String[] params) {
        String logType = params[4];
        if (logger.isDebugEnabled()) {
            logger.debug("[processing log] log type: " + logType);
        }

        for (LogProcessor p : processors) {
            try {
                if (!p.support(logType)) {
                    continue;
                }

                p.parseLog(params);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void finishProcess() {
        for (LogProcessor p : processors) {
            try {
                p.finish();
            } catch (Exception e) {

            }
        }
    }

    @Override
    public void setFile(String filePath) {
        this.filePath = filePath;
    }

    @Override
    public void setProcessors(List<LogProcessor> processors) {
        this.processors = processors;
    }

}
