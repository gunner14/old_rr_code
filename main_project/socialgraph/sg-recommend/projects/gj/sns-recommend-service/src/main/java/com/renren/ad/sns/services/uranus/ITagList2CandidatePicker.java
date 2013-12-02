package com.renren.ad.sns.services.uranus;

import java.util.List;
import java.util.Map;

public interface ITagList2CandidatePicker {

    List<Candidate> pick(long gid, List<Long> tagList, Map<Long, CandidateList> tag2Candidate);

    List<Candidate> pick(long gid, List<Long> tagList, Map<Long, CandidateList> tag2Candidate,
            Map<Long, Integer> goods2category);
}
