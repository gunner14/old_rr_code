package xce.tripod;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 提供给开发者的抽象Producer
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public abstract class Producer {

    public abstract byte[] create(String key, boolean useMaster);

    public Map<String, byte[]> create(List<String> keys, boolean useMaster) {
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        if (keys == null || keys.isEmpty()) {
            return result;
        }
        for (String key : keys) {
            byte[] value = create(key, useMaster);
            if (value != null) {
                result.put(key, value);
            }
        }
        return result;
    }

}
