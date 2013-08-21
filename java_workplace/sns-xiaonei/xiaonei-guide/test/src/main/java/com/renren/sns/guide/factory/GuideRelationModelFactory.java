package com.renren.sns.guide.factory;

import com.renren.sns.guide.model.GuideCoWorkerRelationModel;
import com.renren.sns.guide.model.GuideCollegeSchoolRelationModel;
import com.renren.sns.guide.model.GuideCommonRelationModel;
import com.renren.sns.guide.model.GuideElementarySchoolRelationModel;
import com.renren.sns.guide.model.GuideHighSchoolRelationModel;
import com.renren.sns.guide.model.GuideIRelationModel;
import com.renren.sns.guide.model.GuideJuniorSchoolRelationModel;
import com.renren.sns.guide.model.GuideLocationRelationModel;
import com.renren.sns.guide.model.GuideUniversityRelationModel;
import com.xiaonei.platform.core.model.User;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午10:50:30 类说明
 */
public class GuideRelationModelFactory {

    public static final int COMMON_RELATION_MODEL = 0;

    public static final int COWORKER_RELATION_MODEL = 1;

    public static final int UNIVERSITY_RELATION_MODEL = 2;

    public static final int COLLEGE_RELATION_MODEL = 3;

    public static final int HIGH_RELATION_MODEL = 4;

    public static final int JUNIOR_RELATION_MODEL = 5;

    public static final int ELEMENTARY_RELATION_MODEL = 6;

    public static final int LOCATION_RELATION_MODEL = 7;

    public static final int GUIDE_RELATION_MODEL = 8;

    public static GuideIRelationModel create(User host, int limit, int type) {
        GuideIRelationModel instance = null;
        switch (type) {
            case COMMON_RELATION_MODEL:
                instance = new GuideCommonRelationModel(host, limit);
                break;
            case COWORKER_RELATION_MODEL:
                instance = new GuideCoWorkerRelationModel(host, limit);
                break;
            case UNIVERSITY_RELATION_MODEL:
                instance = new GuideUniversityRelationModel(host, limit);
                break;
            case COLLEGE_RELATION_MODEL:
                instance = new GuideCollegeSchoolRelationModel(host, limit);
                break;
            case HIGH_RELATION_MODEL:
                instance = new GuideHighSchoolRelationModel(host, limit);
                break;
            case JUNIOR_RELATION_MODEL:
                instance = new GuideJuniorSchoolRelationModel(host, limit);
                break;
            case ELEMENTARY_RELATION_MODEL:
                instance = new GuideElementarySchoolRelationModel(host, limit);
                break;
            case LOCATION_RELATION_MODEL:
                instance = new GuideLocationRelationModel(host, limit);
                break;
            default:
                break;
        }
        return instance;
    }
}
