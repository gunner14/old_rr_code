/**
 * 
 */
package xce.tripod.checker;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import xce.tripod.TripodException;

/**
 * <p>
 * Title: AbstractChecker.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-5-28
 */
public abstract class AbstractChecker {

    public static void log(String content) {
        System.out.println("[" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())
                + "] " + content);
    }

    private void checkOneId(int id, int retry) throws TripodException {
        //		String msg = "";
        //		String msg = "Checking " + id;
        Object a = getDataFromCache(id);
        Object b = getDataFromDB(id);
        // a, b若其中一个返回null,也执行reload
        if (!compare(a, b)) {
            log("Check failed! ID=" + id);
            //			msg += " failed!";
            //			msg += " Reload";
            int i = 0;
            for (; i < retry; ++i) {
                if (reloadData(id)) break;
            }
            if (i == retry) {
                log("Reload failed! ID=" + id);
            } else {
                log("Reload success! ID=" + id);
            }
        } else {
            log("Check success! ID=" + id);
        }
        //		log(msg);
    }

    /**
     * @param filePath
     * @param interval
     * @param retry
     */
    public void startCheck(String filePath, int interval, int retry) {
        File file = new File(filePath);
        BufferedReader reader = null;
        try {
            // 以行为单位读取文件内容，一次读一整行
            reader = new BufferedReader(new FileReader(file));
            String lineString = null;
            int id = -1;
            // 一次读入一行，直到读入null为文件结束
            while ((lineString = reader.readLine()) != null) {
                id = Integer.parseInt(lineString);
                try {
                    checkOneId(id, retry);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                Thread.sleep(interval);
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e1) {}
            }
        }
    }

    /**
     * @param a
     * @param b
     * @return
     */
    protected abstract boolean compare(Object dataFromCache, Object dataFromDB);

    /**
     * @param id
     */
    protected abstract boolean reloadData(int id);

    /**
     * @param id
     * @return
     */
    protected abstract Object getDataFromDB(int id) throws TripodException;

    /**
     * @param id
     * @return
     * @throws TripodException
     */
    protected abstract Object getDataFromCache(int id) throws TripodException;

}
