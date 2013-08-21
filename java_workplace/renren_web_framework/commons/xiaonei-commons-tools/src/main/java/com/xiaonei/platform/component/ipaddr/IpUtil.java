/**
 * 
 */
package com.xiaonei.platform.component.ipaddr;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

import org.apache.commons.lang.math.NumberUtils;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;

/**
 * 
 * @author skllb
 * 
 */
public class IpUtil {
	private List<IpAddr> list = null;

	public IpAddr get(String ip) {
		if ((ip == null) || "".equals(ip)) {
			return null;
		} else {
			long code = 0;
			String[] ips = ip.split("\\.");
			if (ips.length == 4) {
				long[] ipsInt = new long[4];
				for (int i = 0; i < ips.length; i++) {
					ipsInt[i] = NumberUtils.toLong(ips[i]);
				}
				code += ipsInt[0] * 256 * 256 * 256;
				code += ipsInt[1] * 256 * 256;
				code += ipsInt[2] * 256;
				code += ipsInt[3];

			} else {
				return null;
			}
			return this.get(code);
		}
	}

	/**
	 * 注意：list有可能为空的
	 * 
	 * @param code
	 * @return
	 */
	public IpAddr get(long code) {
		// code
		if (list != null) {
			int low = 0;
			int high = list.size() - 1;
			while (low <= high) {
				int mid = low + ((high - low) / 2);
				long midStart = list.get(mid).getStart();
				long midEnd = list.get(mid).getEnd();
				if (code < midStart) {
					high = mid - 1;
				} else if (code > midEnd) {
					low = mid + 1;
				} else {
					return list.get(mid);
				}
			}

		}
		return null;
	}

	private IpUtil() {

	}

	private static IpUtil instance = null;

	// 最后检查时间
	private static long lastCheckTime = 0;

	// 每次间隔检查时间
	private static long MAX_CHECK_STEP_TIME = 300000;

	public static IpUtil getInstance() {
		if (instance == null) {
			synchronized (IpUtil.class) {// 双锁定
				if (instance == null)
					instance = new IpUtil();
			}
		}

		if (((instance.list == null) || (instance.list.size() == 0))
				&& (System.currentTimeMillis() - lastCheckTime > MAX_CHECK_STEP_TIME)) {// 如果第一次使用时数据库初始化失败，则每间隔一定时间初始化一次

			// 放置首次初始化不成功，重试
			synchronized (instance) {
				if (((instance.list == null) || (instance.list.size() > 0))
						&& (System.currentTimeMillis() - lastCheckTime > MAX_CHECK_STEP_TIME)) {
					IpUtil.lastCheckTime = System.currentTimeMillis();
					IpUtil.init(instance);
				}
			}
		}

		return instance;
	}

	/**
	 * 初始化
	 * 
	 * @param instance
	 */
	@SuppressWarnings("unchecked")
	private static void init(IpUtil instance) {
		// instance.list = new ArrayList<IpAddr>();
		// for (int i = 0; i < 100000; i++)
		// {
		// IpAddr iar = new IpAddr();
		// iar.setStart(i * 1000);
		// iar.setEnd(iar.getStart() + 500 + new Random().nextInt(500));
		// instance.list.add(iar);
		// }
		OpList list = new OpList("select * from region_ip_table",
				DataAccessMgr.BIZ_COMMON) {

			@Override
			public IpAddr parse(ResultSet rs) throws SQLException {
				IpAddr ia = new IpAddr();
				ia.setCode(rs.getString("code"));
				ia.setEnd(rs.getLong("end"));
				ia.setName(rs.getString("name"));
				ia.setStart(rs.getLong("start"));
				return ia;
			}

		};

		try {
			instance.list = DataAccessMgr.getInstance().queryList(list);
			Collections.sort(instance.list);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		IpUtil.getInstance().get("202.198.160.30");

		// IpUtil iu = IpUtil.getInstance();//
		// long t = System.currentTimeMillis();
		// long var = new Random().nextInt(100000000);
		// IpAddr iar = iu.get(var);
		// System.out.println(System.currentTimeMillis() - t);
		// System.out.println(iar.getStart() + "\t" + var + "\t" +
		// iar.getEnd());

	}
}
