package com.renren.sns.guide.model;
/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午10:52:03
 * 类说明
 */
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
public abstract class GuideAbstractModel implements GuideIRelationModel {
    private volatile long begin;
    private volatile long end;
    private User host;
    /** 想要多少人 */
    protected int limit;
    /** 结果集合 */
    private List<GuideIRelation> ret;
    protected GuideAbstractModel(User host, int limit) {
        this.host = host;
        this.limit = limit;
        this.ret = new ArrayList<GuideIRelation>();
        this.begin = 0;
        this.end = 0;
    }
    @Override
    public void filter() {
    }
    @Override
    public List<GuideIRelation> getResult() {
        return this.ret;
    }
    @Override
    public void shuffle() {
        for (GuideIRelation relation : this.ret) {
            if (relation.getList().size() < 50) {
                Collections.shuffle(relation.getList());
            } else {
                Collections.shuffle(relation.getList().subList(0, 4));
                Collections.shuffle(relation.getList().subList(4, 8));
                Collections.shuffle(relation.getList().subList(8, 16));
                Collections.shuffle(relation.getList().subList(16, 32));
                Collections.shuffle(relation.getList().subList(32, relation.getList().size()));
            }
        }
    }
    @Override
    public GuideIRelationModel call() throws Exception {
        try {
            //*3 坑死人了……inshion 
            this.getRelations(host, 0, this.limit * 3);
            GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" cost:"+(getEnd() - getBegin()) );
            this.shuffle();
            this.filter();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return this;
    }
    public abstract List<GuideIRelation> getRelations(User host, int offset, int limit);
    public long getBegin() {
        return this.begin;
    }
    public void setBegin(long begin) {
        this.begin = begin;
    }
    public long getEnd() {
        return this.end;
    }
    public void setEnd(long end) {
        this.end = end;
    }
}