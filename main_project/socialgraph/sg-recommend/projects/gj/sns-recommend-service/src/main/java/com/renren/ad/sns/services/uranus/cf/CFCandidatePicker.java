package com.renren.ad.sns.services.uranus.cf;

import static com.renren.ad.sns.services.uranus.cf.CFConstants.NEW_MIN_VALUE;
import static com.renren.ad.sns.services.uranus.cf.CFConstants.NEW_VALUE_RANGE;
import static com.renren.ad.sns.services.uranus.cf.CFConstants.OLD_VALUE_RANGE;
import static com.renren.ad.sns.services.uranus.cf.CFConstants.OLD_MIN_VALUE;
import static com.renren.ad.sns.services.uranus.cf.CFConstants.recommendResultFile;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.services.uranus.Candidate;
import com.renren.ad.sns.services.uranus.CandidateType;
import com.renren.ad.sns.services.uranus.ICandidatePicker;
import com.renren.ad.sns.util.FileWatchdog;

public class CFCandidatePicker implements ICandidatePicker {

    protected static final Log LOG = LogFactory.getLog(CFCandidatePicker.class);

    private FileWatchdog recommendResultWatchDog;

    private Map<Long, List<Candidate>> uId2Candidates;

    private String recommenResultPath;
    
    private final Object uId2CandidatesLock = new byte[0];

    public CFCandidatePicker(String recommenResultPath) {
        this.recommenResultPath = recommenResultPath;
    }

    public void init() {
        recommendResultWatchDog = new FileWatchdog(recommenResultPath + File.separator
                + recommendResultFile) {

            @Override
            protected void doOnChange() {
                refreshRecommendations();
            }
        };
        recommendResultWatchDog.setDelay(CFConstants.DEFAULT_REFRESH_INTERVAL);
        recommendResultWatchDog.start();
    }

    /**
     * Refresh user-recommendations data.
     */
    private void refreshRecommendations() {
        long begin = System.currentTimeMillis();
        LOG.info("Start refreshing recommendation.");
        Map<Long, List<Candidate>> newUid2Candidates = CFRecommendationLoader
                .loadRecommendations(recommenResultPath + File.separator + recommendResultFile);
        if (newUid2Candidates != null) {
            synchronized(uId2CandidatesLock){
                uId2Candidates = newUid2Candidates;
            }
            LOG.info("Finish refreshing recommendation. time elapsed:"
                    + (System.currentTimeMillis() - begin));
        } else {
            LOG.warn("Refresh recommendation data failed.");
        }
    }

    @Override
    public List<Candidate> pick(long uid, long gid) {
        List<Candidate> result = new ArrayList<Candidate>();
        synchronized(uId2CandidatesLock){
            if (uId2Candidates != null && uId2Candidates.containsKey(uid)) {
                for (Candidate candidate : uId2Candidates.get(uid)) {
                    result.add(new Candidate(candidate.getGid(),
                            normalizeWeight(candidate.getWeight()), CandidateType.CFCandidatePicker));
                }
            }
        }
        
        return result;
    }

    public List<Candidate> pickDefault(long uid, long gid) {
        return new ArrayList<Candidate>();
    }

    /**
     * Batch version of the method 'normalizeWeight'
     * 
     * @return
     */
    private List<Candidate> normalizeWeights(List<Candidate> raw) {
        List<Candidate> result = new ArrayList<Candidate>();
        for (Candidate candidate : raw) {
            result.add(new Candidate(candidate.getGid(), normalizeWeight(candidate.getWeight())));
        }
        return result;
    }

    /**
     * Mapping the old weight (range [1.0, 5.0]) to a new one (range [3.09,
     * 3.11]).
     * 
     * @param oldWeight
     * @return
     */
    private double normalizeWeight(double oldWeight) {
        return NEW_MIN_VALUE + NEW_VALUE_RANGE * (oldWeight - OLD_MIN_VALUE) / OLD_VALUE_RANGE;
    }

    public Map<Long, List<Candidate>> getuId2Candidates() {
        return uId2Candidates;
    }

}
