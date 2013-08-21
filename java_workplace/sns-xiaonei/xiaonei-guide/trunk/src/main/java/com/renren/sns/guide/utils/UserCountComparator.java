package com.renren.sns.guide.utils;

import java.io.Serializable;
import java.util.Comparator;

import com.renren.sns.guide.controllers.window.GuideUserCountController.UserCount;

public class UserCountComparator implements Comparator<UserCount>,Serializable {

    /**
	 * 
	 */
	private static final long serialVersionUID = -3723261134809716368L;

	public int compare(UserCount arg0, UserCount arg1) {
        if (arg0.getOrder()== arg1.getOrder()) {
            return 0;
        } else if (arg0.getOrder() < arg1.getOrder()) {
            return -1;
        } else {
            return 1;
        }
    }

}
