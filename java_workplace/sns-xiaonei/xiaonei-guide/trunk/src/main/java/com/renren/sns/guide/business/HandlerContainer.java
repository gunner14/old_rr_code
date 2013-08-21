package com.renren.sns.guide.business;

import com.renren.sns.guide.business.handler.GuideFridBirthdayHandler;
import com.renren.sns.guide.business.handler.GuideHotStarHandler;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-26 上午11:21:28
 */
public class HandlerContainer {

    public static GuideWindowHandler guideFridBirthdayHandler = new GuideFridBirthdayHandler();

    public static GuideWindowHandler gudieHotStarHandler = new GuideHotStarHandler();
}
