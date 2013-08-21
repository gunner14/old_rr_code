package com.renren.sns.guide.model;
import java.util.List;
import java.util.concurrent.Callable;

import com.xiaonei.platform.core.model.User;
/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午10:54:22
 * 此子类的model具体logic成分
 */


public interface GuideIRelationModel extends Callable<GuideIRelationModel> {

    public List<GuideIRelation> getRelations(User host, int offset, int limit);

    /**
     * 乱序<br>
     * 如果返回人数组元素小于50，则全部一次性乱序
     * 如果返回人数组元素不小于50，则[0,3][4,7][8,15][16,31][32,size]分堆乱序
     */
    public void shuffle();

    public List<GuideIRelation> getResult();

    public void filter();

}

 