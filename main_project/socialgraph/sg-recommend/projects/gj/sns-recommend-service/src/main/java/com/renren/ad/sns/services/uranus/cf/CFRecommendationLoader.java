package com.renren.ad.sns.services.uranus.cf;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.services.uranus.Candidate;

public class CFRecommendationLoader {

    /* Delimiter between key and value */
    public static final String KEY_VAL_DELIMITER = "\\s+";

    public static final String ITEM_DELIMITER = ",";

    public static final String ID_WEIGHT_DELIMITER = ":";

    private static final Log LOG = LogFactory.getLog(CFRecommendationLoader.class);

    /**
     * Load the newest recommendation result.
     * 
     * @param resultFile recommendation result file, the line format is as
     *        follows:<br/>
     *        userId [itemId1:weight1,itemId2:weight2,...]
     * @return null if any exception occurs, else the expected (uid,
     *         candidates) map
     */
    public static Map<Long, List<Candidate>> loadRecommendations(String resultFile) {
        Map<Long, List<Candidate>> result = new HashMap<Long, List<Candidate>>();
        BufferedReader br = null;
        int counter = 0;
        long begin = System.currentTimeMillis();
        LOG.info("Start to load recommendations from file.");

        try {
            br = new BufferedReader(new FileReader(resultFile));
            String line = br.readLine();
            while (line != null) {
                String[] kv = line.split(KEY_VAL_DELIMITER);
                if (kv.length == 2) {
                    long uid = Long.parseLong(kv[0]);
                    List<Candidate> candidates = parseCandidates(kv[1]);
                    result.put(uid, candidates);

                    //log progress
                    if (++counter % 10000 == 0) {
                        LOG.info("Loaded " + counter + " records.");
                    }
                }

                line = br.readLine();
            }
            LOG.info("Finish load recommendations, time elapsed: "
                    + (System.currentTimeMillis() - begin));
        } catch (Exception e) {
            e.printStackTrace();
            LOG.warn("Load recommendations failed:\n" + e.getLocalizedMessage());
            result = null;
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    private static List<Candidate> parseCandidates(String line) {
        List<Candidate> result = new ArrayList<Candidate>();
        //remove the first and last square bracket
        line = line.substring(1, line.length() - 1);
        String[] items = line.split(ITEM_DELIMITER);
        for (String item : items) {
            String[] idWeight = item.split(ID_WEIGHT_DELIMITER);
            if (idWeight.length == 2) {
                long gid = Long.parseLong(idWeight[0]);
                double weight = Double.parseDouble(idWeight[1]);
                result.add(new Candidate(gid, weight));
            }
        }
        return result;
    }
}
