package com.renren.ad.sns.services.uranus;

import java.util.ArrayList;
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


public class BasicTagList2CandidatePicker implements ITagList2CandidatePicker {

    private static final Log LOG = LogFactory.getLog(BasicTagList2CandidatePicker.class);

    private static final int CANDIDATE_MAX_SIZE = 200;

    public static final long TAG_2_CANDIDATE_TIMEOUT = 100;

    /* ThreadPoolExecutor parameters */
    public static final int TPE_CORE_SIZE = 80;

    public static final int TPE_MAX_SIZE = 100;

    public static final int TPE_QUEUE_SIZE = 1;

    public static final long TPE_KEEP_ALIVE_TIME = 60;

    //private final Object tag2CandidateLock = new byte[0];

    private Map<Long, CandidateList> tag2Candidate;

    private ExecutorService executor;

    public BasicTagList2CandidatePicker() {

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

        private long gid;

        private List<Long> tagList;

        public TagList2CandidateTask(long gid, List<Long> tagList) {
            this.gid = gid;
            this.tagList = tagList;
        }

        @Override
        public List<Candidate> call() throws Exception {
            List<Candidate> candidates = new ArrayList<Candidate>();
            final Candidate excludeCandidate = new Candidate(gid, 0);

            for (Long tag : tagList) {
                if (tag2Candidate.containsKey(tag)) {
                    CandidateList clist = tag2Candidate.get(tag);
                    candidates = CandidateList.mergeAndExclude(candidates, clist, excludeCandidate,
                            CANDIDATE_MAX_SIZE);
                    if (candidates.size() >= CANDIDATE_MAX_SIZE) {
                        break;
                    }
                }
            }

            return candidates;
        }

    }

    @Override
    public List<Candidate> pick(long gid, List<Long> tagList,
            Map<Long, CandidateList> tag2Candidate, Map<Long, Integer> goods2category) {
        return pick(gid, tagList, tag2Candidate);
    }

}
