package com.renren.ad.sns.services.uranus;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.ThreadPoolExecutor.AbortPolicy;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.services.uranus.cpa.ModelHandler;
import com.renren.ad.sns.services.uranus.prg.Ranker;
import com.renren.ad.sns.services.uranus.util.Util;

/**
 * recommend engine for goods
 * 
 * @author socialgraph
 * 
 */
public class RecommendEngine {

    private final static Log LOG = LogFactory.getLog(RecommendEngine.class);

    private ModelHandler modelHandler;

    private IDataProvider dataProvider;

    private ICandidatePicker[] candidatePicker;

    private ExecutorService executor;

    private CandidatesFilter filter;

    public static final long MODEL_TIMEOUT = 100;

    /* ThreadPoolExecutor parameters */
    public static final int TPE_CORE_SIZE = 80;

    public static final int TPE_MAX_SIZE = 100;

    public static final int TPE_QUEUE_SIZE = 1;

    public static final long TPE_KEEP_ALIVE_TIME = 60;

    public RecommendEngine(ModelHandler modelHandler, IDataProvider dataProvider,
            ICandidatePicker[] candidatePicker, CandidatesFilter filter) {
        this.modelHandler = modelHandler;
        this.dataProvider = dataProvider;
        this.candidatePicker = candidatePicker;
        this.executor = new ThreadPoolExecutor(TPE_CORE_SIZE, TPE_MAX_SIZE, TPE_KEEP_ALIVE_TIME,
                TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(TPE_QUEUE_SIZE),
                new AbortPolicy());
        this.filter = filter;
    }

    public List<Candidate> recommend(long uid, long gid, int maxnum) {

        final long beginTime = System.currentTimeMillis();
        
        List<Candidate> recResult = null;
        List<Candidate> candidates = new ArrayList<Candidate>();
        try {
            // 1. get all candidates (100ms)
            int lastCount = 0;
            long pickBeginTime = 0;
            for (ICandidatePicker picker : candidatePicker) {
                pickBeginTime = System.currentTimeMillis();
                candidates.addAll(picker.pick(uid, gid));
                LOG.info(picker.getClass().getSimpleName() + " load candidates: "
                        + (candidates.size() - lastCount) + ", elapse: "
                        + (System.currentTimeMillis() - pickBeginTime) + " ms.");
                lastCount = candidates.size();
            }
            final long pickEndTime = System.currentTimeMillis();
            LOG.info("pick candidate elapse: " + (pickEndTime - beginTime)
                    + " ms, load candidates:" + candidates.size());

            // 2. do filter
            candidates = filter.filter(candidates, uid, gid);
            final long filterEndTime = System.currentTimeMillis();
            LOG.info("filter elapse:" + (filterEndTime - pickEndTime) + " ms, filtered size:"
                    + candidates.size());

            // 3. use user-preference model to re-rank candidates
            Future<List<Candidate>> modelFuture = null;
            try {
                modelFuture = executor.submit(new RecommendTask(candidates, uid, maxnum));
                recResult = modelFuture.get(MODEL_TIMEOUT, TimeUnit.MILLISECONDS);
            } catch (TimeoutException te) {
                LOG.error("model future timeout.");
            } catch (RejectedExecutionException re){
                LOG.error("model reject.");
            } catch (Exception e) {
                LOG.error(e.getLocalizedMessage());
            }
        } catch (Exception e) {
            LOG.error(e.getMessage(), e);
        } finally {
            if (recResult == null && candidates != null) {
                recResult = new ArrayList<Candidate>();
                for (int i = 0; i < candidates.size() && i < maxnum; i++) {
                    recResult.add(candidates.get(i));
                }
            }
        }

        LOG.info(formatRecommendResult(uid, gid, recResult) + " elapse:"
                + (System.currentTimeMillis() - beginTime));
        return recResult;
    }

    /**
     * result format: uid|gid|gid1,src1,weight1|gid2,src2,weight2
     * 
     * @param uid
     * @param gid
     * @param candidates
     * @return
     */
    public static String formatRecommendResult(long uid, long gid, List<Candidate> candidates) {
        final String ITEM_SEP = "|";
        final String FIELD_SEP = ",";
        StringBuffer sb = new StringBuffer(uid + ITEM_SEP + gid);
        for (Candidate c : candidates) {
            sb.append(ITEM_SEP + c.getGid() + FIELD_SEP + c.getSource().getValue() + FIELD_SEP
                    + String.format("%.3f", c.getWeight()));
        }
        return sb.toString();
    }

    /**
     * Use user-preference model to rank candidates.
     * 
     * @param uid
     * @param candidates
     * @param maxnum
     * @return
     */
    List<Candidate> recommend(long uid, List<Candidate> candidates, int maxnum) {
        long t_beg = System.currentTimeMillis();
        Number[] userInfo = dataProvider.getUserMetaData(uid).getValues();
        ArrayList<String> gids = new ArrayList<String>();
        List<Double> scores = new ArrayList<Double>();
        for (Candidate candidate : candidates) {
            Number[] goodsInfo = dataProvider.getGoodsMetaData(uid, candidate.getGid()).getValues();
            StringBuilder sb = new StringBuilder();
            int len = Math.min(userInfo.length, UserMetadata.FIELD_SIZE);
            for (int j = 0; j < len; j++) {
                sb.append(userInfo[j] + Util.SEPERATOR_TERM);
            }
            for (int j = 0; j < goodsInfo.length; j++) {
                sb.append(goodsInfo[j] + Util.SEPERATOR_TERM);
            }
            gids.add(sb.toString());
            scores.add(candidate.getWeight());

            if (LOG.isDebugEnabled()) {
                LOG.debug(candidate.getWeight() + "\t" + sb.toString());
            }
        }

        List<Double> cpa = modelHandler.predict(gids);
        List<Integer> rankedid = Ranker.rank(scores, cpa, maxnum);
        ArrayList<Candidate> result = new ArrayList<Candidate>();
        for (int i = 0; i < rankedid.size(); i++) {
            result.add(candidates.get(rankedid.get(i)));
        }
        LOG.info("recommed candidate elapse : " + (System.currentTimeMillis() - t_beg) + " ms"
                + " , with candidates : " + candidates.size());
        return result;
    }

    class PickTask implements Callable<List<Candidate>> {

        private long uid;

        private long gid;

        private ICandidatePicker picker;

        public PickTask(ICandidatePicker picker, long uid, long gid) {
            this.picker = picker;
            this.uid = uid;
            this.gid = gid;
        }

        @Override
        public List<Candidate> call() throws Exception {
            if (picker != null) {
                return picker.pick(uid, gid);
            }
            return new ArrayList<Candidate>();
        }

    }

    class RecommendTask implements Callable<List<Candidate>> {

        List<Candidate> candidates;

        long uid;

        int maxnum;

        public RecommendTask(List<Candidate> candidates, long uid, int maxnum) {
            super();
            this.candidates = candidates;
            this.uid = uid;
            this.maxnum = maxnum;
        }

        public List<Candidate> call() throws Exception {
            List<Candidate> res = new ArrayList<Candidate>();
            try {
                res = recommend(uid, candidates, maxnum);
            } catch (Exception e) {// time out
                LOG.error(e, e);
            }
            return res;
        }
    }
}
