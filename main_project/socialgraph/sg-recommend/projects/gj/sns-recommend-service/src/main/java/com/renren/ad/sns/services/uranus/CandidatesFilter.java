package com.renren.ad.sns.services.uranus;

import java.util.List;



public interface CandidatesFilter {
    
    /**
     * Filter out invalid candidates. 
     * @param candidates candidate list waiting to filter
     * @param uid current user id
     * @param gid current goods id
     * @return the filtered candidate list, if the result is empty, return an empty list.
     */
    List<Candidate> filter(List<Candidate> candidates, long uid, long gid);
}

