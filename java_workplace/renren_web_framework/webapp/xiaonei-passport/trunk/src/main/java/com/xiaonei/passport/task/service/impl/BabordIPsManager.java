/**
 * 
 */
package com.xiaonei.passport.task.service.impl;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.IpAttributionDAO;
import com.xiaonei.passport.model.BabordIPs;


/**
 * BabordIPsManager <br>
 * 海外IP
 * 
 * @author tai.wang@opi-corp.com Aug 23, 2010 - 8:59:40 PM
 */
@Service
public class BabordIPsManager implements InitializingBean {

    private static volatile List<long[]> babordIPs;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Autowired
    private IpAttributionDAO dao;

    public BabordIPsManager() {
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        //        setBabordIPS(); 2011年3月11日关闭海外用户广告功能  
    }

    /**
     * getBabordIPsList<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:05:30 PM
     */
    public List<BabordIPs> getBabordIPsList() {
        try {
            return this.dao.getBanedIpList();
        } catch (final DataAccessException e) {
            e.printStackTrace();
            return new ArrayList<BabordIPs>(0);
        }
    }

    /**
     * getLongIP<br>
     * 获得ip的长整数表示
     * 
     * @param ip {@link String}
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:12:14 PM
     */
    public long getLongIP(String ip) {
        ip = StringUtils.trimToEmpty(ip);
        final String REGX_IP = "((25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)\\.){3}(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)";
        if (!ip.matches(REGX_IP)) {
            return -1;
        }
        final String[] sipt = ip.split("\\.");
        long ipt = 0L;
        for (int i = 0; i < 4; ++i) {
            ipt = ipt << 8 | Integer.parseInt(sipt[i]);
        }
        return ipt;
    }

    /**
     * isBaboradIP<br>
     * 是否为海外
     * 
     * @param ip {@link String}
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:27:25 PM
     */
    public boolean isBaboradIP(final String ip) {
        if (babordIPs == null) {
            //			setBabordIPS();
            this.logger.error("!!! babordIPs == null !!!");
            return false;
        }
        final long ipLong = this.getLongIP(ip);
        for (final long[] ips : babordIPs) {
            if (ipLong >= ips[0] && ipLong <= ips[1]) {
                return true;
            }
        }
        return false;
    }

    /**
     * reloadBabordIPS<br>
     * 重新加载
     * 
     * 
     * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:25:16 PM
     */
    public void reloadBabordIPS() {
        System.out.println("reloadBabordIPS start");
        final List<BabordIPs> list = this.getBabordIPsList();
        final List<long[]> babordIPsLoc = new ArrayList<long[]>();
        for (final BabordIPs babordIPs : list) {
            try {
                final long s = this.getLongIP(babordIPs.getStart());
                if (-1 == s) {
                    continue;
                }
                final long e = this.getLongIP(babordIPs.getEnd());
                if (-1 == e) {
                    continue;
                }
                final long[] ips = { s, e };
                babordIPsLoc.add(ips);
            } catch (final Exception e) {
                e.printStackTrace();
            }
        }
        babordIPs = babordIPsLoc;
        System.out.println("reloadBabordIPS end");
    }

    /**
     * setBabordIPS<br>
     * 设置IP
     * 
     * 
     * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:20:59 PM
     */
    public synchronized void setBabordIPS() {
        if (babordIPs == null) {
            this.reloadBabordIPS();
        }
    }
}
