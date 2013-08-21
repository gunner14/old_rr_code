package com.xiaonei.reg.jump.data.logic;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.jump.data.dao.MlinkRegToGameMapDAO;
import com.xiaonei.reg.jump.data.model.MlinkRegToGameMapModel;

/**
 * MlinkRegToGameMapLogic.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 创建文件
 */
public class MlinkRegToGameMapLogic {

    public static MlinkRegToGameMapLogic getInstance() {
        return instance;
    }

    private MlinkRegToGameMapLogic() {
        super();
    }

    private static MlinkRegToGameMapLogic instance = new MlinkRegToGameMapLogic();
    public List<MlinkRegToGameMapModel> select(User host){
        if(host == null){
            GuideLogger.printLog(" host:null",GuideLogger.WARN);
        }
        
        List<MlinkRegToGameMapModel>  ret = null;
        try {
            ret = MlinkRegToGameMapDAO.getInstance().select();
        } catch (SQLException e) {
            GuideLogger.printLog(" e:"+e.toString());
            e.printStackTrace();
        }
        
        return ret;
        
    }
    
    public MlinkRegToGameMapModel selectBySsIdAndRtId(User host,final int ss_id,final int rt_id){
        if(host == null){
            GuideLogger.printLog(" host:null",GuideLogger.WARN);
        }
        
        MlinkRegToGameMapModel ret = null;
        try {
            ret = MlinkRegToGameMapDAO.getInstance().selectBySsIdAndRtId(ss_id, rt_id);
        } catch (SQLException e) {
            GuideLogger.printLog(" e:"+e.toString());
            e.printStackTrace();
        }
        
        return ret;
    }
    
    public int insert(User host,final MlinkRegToGameMapModel model) {
        if(host == null){
            GuideLogger.printLog(" host:null",GuideLogger.WARN);
        }
        
        int ret = -1;
        try {
            ret = MlinkRegToGameMapDAO.getInstance().insert(model);
        } catch (SQLException e) {
            GuideLogger.printLog(" e:"+e.toString());
            e.printStackTrace();
        }
        
        return ret;
    }

    public int update(User host, MlinkRegToGameMapModel model) {
        if(host == null){
            GuideLogger.printLog(" host:null",GuideLogger.WARN);
        }
        
        int ret = -1;
        try {
            ret = MlinkRegToGameMapDAO.getInstance().update(model);
        } catch (SQLException e) {
            GuideLogger.printLog(" e:"+e.toString());
            e.printStackTrace();
        }
        
        return ret;
    }

    public int delete(User host, MlinkRegToGameMapModel model) {
        if(host == null){
            GuideLogger.printLog(" host:null",GuideLogger.WARN);
        }
        
        int ret = -1;
        try {
            ret = MlinkRegToGameMapDAO.getInstance().delete(model);
        } catch (SQLException e) {
            GuideLogger.printLog(" e:"+e.toString());
            e.printStackTrace();
        }
        
        return ret;
    }
    
}
