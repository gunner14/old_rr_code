package com.renren.ad.sns.services.uranus;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;

import com.renren.ad.sns.services.uranus.protocol.UranusService;
import com.renren.ad.sns.util.thrift.ThriftServerImplementor;

public class UranusServiceImpl extends ThriftServerImplementor implements UranusService.Iface {

    private RecommendEngine recommendEngine;
    protected static final Log LOG = LogFactory.getLog(UranusServiceImpl.class);
    
    @Override
    protected TProcessor createProcessor() {
        return new UranusService.Processor(this);
    }

    @Override
    public String recommend(long uid, long gid, int maxnum) throws TException {
    	List<Candidate> res = new ArrayList<Candidate>();
    	try{
    		res = recommendEngine.recommend(uid, gid, maxnum);
    	}catch(Exception e){
    		LOG.error(e.getMessage(), e);
    	}
        return toJson(res);
    }

    public void setRecommendEngine(RecommendEngine recommendEngine) {
        this.recommendEngine = recommendEngine;
    }
    
    private String toJson(List<Candidate> candidates){
    	if(candidates != null && candidates.size() > 0){
    		StringBuffer sb = new StringBuffer("[");
            for(Candidate c : candidates){
                sb.append("{\"gid\":"+c.getGid()+",\"src\":"+c.getSource().getValue()+"},");
            }
            sb.setCharAt(sb.length()-1, ']');
            return sb.toString();
    	}
        return "";
    }
}
