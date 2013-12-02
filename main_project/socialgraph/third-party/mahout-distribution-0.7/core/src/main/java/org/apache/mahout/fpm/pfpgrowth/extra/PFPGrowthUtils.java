package org.apache.mahout.fpm.pfpgrowth.extra;

import java.util.concurrent.atomic.AtomicInteger;

import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.mahout.common.Parameters;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class PFPGrowthUtils {

    private static final Logger log = LoggerFactory.getLogger(PFPGrowthUtils.class);

    public static boolean shouldRunNextPhase(Parameters params, AtomicInteger currentPhase) {
        int phase = currentPhase.getAndIncrement();
        String startPhase = params.get("startPhase");
        String endPhase = params.get("endPhase");
        boolean phaseSkipped = (startPhase != null && phase < Integer.parseInt(startPhase))
                || (endPhase != null && phase > Integer.parseInt(endPhase));
        if (phaseSkipped) {
            log.info("Skipping phase {}", phase);
        }
        return !phaseSkipped;
    }

}
