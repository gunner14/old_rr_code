package xce.tripod.util;

import xce.tripod.ProducerManagerService;

/**
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class JadeDAOUtil {

    public static <T> T getJadeDAO(Class<T> c) {
	return ProducerManagerService.getRoseAppContext().getBean(c);
    }
}
