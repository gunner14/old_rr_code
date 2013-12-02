package com.renren.ad.sns.services.antispam.model;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class RecordLoader {

    private static final Log LOG = LogFactory.getLog(RecordLoader.class);

    /**
     * 从文件中加载记录
     * 
     * @return
     */
    public static List<AntiSpamRecord> loadRecordsFromFile(String recordsFile) {
        List<AntiSpamRecord> records = new ArrayList<AntiSpamRecord>();

        try {
            List<String> lines = FileUtils.readLines(new File(recordsFile));
            if (lines != null && lines.size() > 0) {
                for (String line : lines) {
                    AntiSpamRecord record = AntiSpamRecord.parseLine(line);
                    if (record != null) {
                        records.add(record);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        LOG.info("Load AntiSpamRecord records: " + records.size());
        return records;
    }
}
