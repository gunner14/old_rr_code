package com.renren.ad.sns.services.uranus;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.ThreadPoolExecutor.AbortPolicy;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SimilarityTagList2CandidatePicker implements ITagList2CandidatePicker {

    private static final Log LOG = LogFactory.getLog(SimilarityTagList2CandidatePicker.class);

    private static final int CANDIDATE_MAX_SIZE = 200;

    public static final long TAG_2_CANDIDATE_TIMEOUT = 100;

    /* ThreadPoolExecutor parameters */
    public static final int TPE_CORE_SIZE = 80;

    public static final int TPE_MAX_SIZE = 100;

    public static final int TPE_QUEUE_SIZE = 1;

    public static final long TPE_KEEP_ALIVE_TIME = 60;

    //private final Object tag2CandidateLock = new byte[0];

    private Map<Long, CandidateList> tag2Candidate;

    private Map<Long, Integer> goods2category;

    private ExecutorService executor;

    public SimilarityTagList2CandidatePicker() {

        executor = new ThreadPoolExecutor(TPE_CORE_SIZE, TPE_MAX_SIZE, TPE_KEEP_ALIVE_TIME,
                TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(TPE_QUEUE_SIZE),
                new AbortPolicy());
    }

    @Override
    public List<Candidate> pick(long gid, List<Long> tagList, Map<Long, CandidateList> tag2Candidate) {

        this.tag2Candidate = tag2Candidate;
        List<Candidate> candidates = new ArrayList<Candidate>();
        try {
            Future<List<Candidate>> future = executor
                    .submit(new TagList2CandidateTask(gid, tagList));
            candidates = future.get(TAG_2_CANDIDATE_TIMEOUT, TimeUnit.MILLISECONDS);
        } catch (TimeoutException te) {
            LOG.error("qurey goods info timeout.");
        } catch (RejectedExecutionException re) {
            LOG.error("qurey goods info reject.");
        } catch (Exception e) {
            LOG.error(e.getMessage(), e);
        }
        return candidates;
    }

    class TagList2CandidateTask implements Callable<List<Candidate>> {

        private int CANDIDATE_MAX_SIZE_TAG = 5000;

        private long gid;

        private List<Long> tagList;

        public TagList2CandidateTask(long gid, List<Long> tagList) {
            this.gid = gid;
            this.tagList = tagList;
        }

        @Override
        public List<Candidate> call() throws Exception {
            Map<Candidate, Integer> candidates = new HashMap<Candidate, Integer>();
            List<Candidate> result = new ArrayList<Candidate>();
            final Candidate excludeCandidate = new Candidate(gid, 0);

            for (Long tag : tagList) {
                if (tag2Candidate.containsKey(tag)) {
                    CandidateList clist = tag2Candidate.get(tag);
                    candidates = CandidateList.merge(candidates, clist, excludeCandidate,
                            CANDIDATE_MAX_SIZE_TAG);
                }
            }

            List<Map.Entry<Candidate, Integer>> infoIds = new ArrayList<Map.Entry<Candidate, Integer>>(
                    candidates.entrySet());

            Collections.sort(infoIds, new Comparator<Map.Entry<Candidate, Integer>>() {

                @Override
                public int compare(Map.Entry<Candidate, Integer> one,
                        Map.Entry<Candidate, Integer> two) {
                    
                    float oneWeight = calculateWeight(one.getKey(), one.getValue(), (float)one.getKey().getWeight());
                    float twoWeight = calculateWeight(two.getKey(), two.getValue(), (float)two.getKey().getWeight());
                    
                    if(oneWeight < twoWeight){
                        return 1;
                    }else if(oneWeight > twoWeight){
                        return -1;
                    }else{
                        return 0;
                    }
                }
            });

            for (int i = 0; i < CANDIDATE_MAX_SIZE && i < infoIds.size(); i++) {
                result.add(infoIds.get(i).getKey());
            }
            return result;
        }

        /**
         * 计算综合了商品相关性（类目相关性和tag相关性）以及商品质量的权重
         * 
         * @return
         */
        private float calculateWeight(Candidate candidate, float rawTagScore, float rawQualityScore) {
            final float correlationWeight = 60;
            final float qualityWeight = 40;
            
            //1. goods correlation
            final float categoryWeight = 0.8f * correlationWeight;
            final float tagWeight = 0.2f * correlationWeight;
            float categoryScore = 0;
            if (goods2category != null && goods2category.containsKey(candidate.getGid())
                    && goods2category.containsKey(gid)) {
                categoryScore = normSmallerBetter(Math.abs(goods2category.get(gid)
                        - goods2category.get(candidate.getGid())), categoryWeight);
            }
            float tagScore = normBiggerBetter(rawTagScore, tagWeight);

            //2. goods quality
            float qualityScore = normBiggerBetter(rawQualityScore, qualityWeight);

            return categoryScore + tagScore + qualityScore;
        }

        private float normBiggerBetter(float raw, float maxVal) {
            return maxVal * raw / (1 + raw);
        }

        private float normSmallerBetter(float raw, float maxVal) {
            return maxVal / (1 + raw);
        }
    }

    /**
     * 增加类目的信息
     */
    @Override
    public List<Candidate> pick(long gid, List<Long> tagList,
            Map<Long, CandidateList> tag2Candidate, Map<Long, Integer> goods2category) {
        this.tag2Candidate = tag2Candidate;
        this.goods2category = goods2category;

        List<Candidate> candidates = new ArrayList<Candidate>();
        try {
            Future<List<Candidate>> future = executor
                    .submit(new TagList2CandidateTask(gid, tagList));
            candidates = future.get(TAG_2_CANDIDATE_TIMEOUT, TimeUnit.MILLISECONDS);
        } catch (TimeoutException te) {
            LOG.error("qurey goods info timeout.");
        } catch (RejectedExecutionException re) {
            LOG.error("qurey goods info reject.");
        } catch (Exception e) {
            LOG.error(e.getMessage(), e);
        }
        return candidates;
    }

    public List<Candidate> pickSync(long gid, List<Long> tagList,
            Map<Long, CandidateList> tag2Candidate, Map<Long, Integer> goods2category) {
        this.tag2Candidate = tag2Candidate;
        this.goods2category = goods2category;

        List<Candidate> candidates = new ArrayList<Candidate>();
        try {
            candidates = new TagList2CandidateTask(gid, tagList).call();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return candidates;
    }
}
