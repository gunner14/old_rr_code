package com.xiaonei.platform.core.opt.network;

import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.JuniorSchool;
import com.xiaonei.platform.core.model.Network;
import com.xiaonei.platform.core.model.Region;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.jdbc.dao.NetworkDao;
import com.xiaonei.platform.core.opt.permission.define.Stage;

public class NetworkManagerFromDB {

	private static NetworkManagerFromDB instance;

	private Map<Integer, University> univs = new HashMap<Integer, University>(2);

	private Map<Integer, HighSchool> hss = new HashMap<Integer, HighSchool>(5);

	private Map<Integer, Workplace> wps = new HashMap<Integer, Workplace>(4);

	private Map<Integer, Region> regions = new HashMap<Integer, Region>();

	private Map<Integer, College> college = new HashMap<Integer, College>();

	private Map<Integer, JuniorSchool> juniorschool = new HashMap<Integer, JuniorSchool>();

	public static int TYPE_UNIV = 0;

	public static int TYPE_HIGH = 1;

	public static int TYPE_WORK = 2;

	public static int TYPE_RIGION = 6;

	public static int TYPE_COLLEGE = 3;

	public static int TYPE_JUNIORSCHOOL = 4;

	public static NetworkManagerFromDB getInstance() {
		if (instance == null) {
			synchronized (NetworkManagerFromDB.class) {
				if (instance == null)
					instance = new NetworkManagerFromDB();
			}
		}
		return instance;
	}

	private NetworkManagerFromDB() {
		this.reload();
	}

	public void reload() {
		long begin = System.currentTimeMillis();
		System.out.println("NetworkManagerFromDB begin:"+ begin);
		this.reloadHeighSchool();
		long afterReloadHeighSchool = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadHeighSchool:"+ afterReloadHeighSchool);
		this.reloadUniv();
		long afterReloadUniv = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadUniv:"+ afterReloadUniv);
		this.reloadWorkplace();
		long afterReloadWorkplace = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadWorkplace:"+ afterReloadWorkplace);
		this.reloadRegion();
		long afterReloadRegion = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadRegion:"+ afterReloadRegion);
		this.reloadCollege();
		long afterReloadCollege = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadCollege:"+ afterReloadCollege);
		this.reloadJuniorSchool();
		long afterReloadJuniorSchool = System.currentTimeMillis(); 
		System.out.println("NetworkManagerFromDB afterReloadJuniorSchool:"+ afterReloadJuniorSchool);
		if (univs == null) {
			univs = new HashMap<Integer, University>(2);
		}
		if (hss == null) {
			hss = new HashMap<Integer, HighSchool>(5);
		}
		if (wps == null) {
			wps = new HashMap<Integer, Workplace>(4);
		}
		if (regions == null) {
			regions = new HashMap<Integer, Region>();
		}
		if (college == null) {
			college = new HashMap<Integer, College>();
		}
		if (juniorschool == null) {
			juniorschool = new HashMap<Integer, JuniorSchool>();
		}
	}

	ScheduledExecutorService executors = Executors.newScheduledThreadPool(6);

	private void reloadUniv() {
		this.univs = NetworkDao.getInstance().getAllUniversity();
	}

	private void reloadHeighSchool() {
		this.hss = NetworkDao.getInstance().getAllHighSchool();
	}

	private void reloadWorkplace() {
		this.wps = NetworkDao.getInstance().getAllWorkplace();
	}

	private void reloadRegion() {
		this.regions = NetworkDao.getInstance().getAllRegion();
	}

	private void reloadCollege() {
		this.college = NetworkDao.getInstance().getAllCollege();
	}

	private void reloadJuniorSchool() {
		this.juniorschool = NetworkDao.getInstance().getAllJuniorSchool();
	}

	public List<University> getUnivs() {
		Collection<University> col = this.univs.values();
		return Arrays.asList(col.toArray(new University[0]));
	}

	public List<HighSchool> getHss() {
		Collection<HighSchool> col = this.hss.values();
		return Arrays.asList(col.toArray(new HighSchool[0]));
	}

	public List<Workplace> getWps() {
		Collection<Workplace> col = this.wps.values();
		return Arrays.asList(col.toArray(new Workplace[0]));
	}

	public University getUnivById(int id) {
		return this.univs.get(new Integer(id));
	}

	public HighSchool getHighSchoolById(int id) {
		return this.hss.get(new Integer(id));
	}

	public Region getRegionByid(int id) {
		return this.regions.get(new Integer(id));
	}

	public Workplace getWorkplaceById(int id) {
		return this.wps.get(new Integer(id));
	}

	public Region getRegionById(int id) {
		return this.regions.get(new Integer(id));
	}

	public College getCollegeById(int id) {
		return this.college.get(new Integer(id));
	}

	public JuniorSchool getJuniorSchoolById(int id) {
		return this.juniorschool.get(new Integer(id));
	}

	public Network getNetById(int id) {
		Network nt = null;
		if (id >= Stage.HEIGHSCHOOL_MIN_ID && id <= Stage.HEIGHSCHOOL_MAX_ID)
			nt = this.hss.get(new Integer(id));
		else if (id >= Stage.UNIV_MIN_ID && id <= Stage.UNIV_MAX_ID) {
			nt = this.univs.get(new Integer(id));
		} else if (id >= Stage.MAJOR_MIN_ID && id <= Stage.MAJOR_MAX_ID) {
			nt = this.wps.get(new Integer(id));
		} else if (id > Stage.COLLEGE_MIN_ID && id < Stage.COLLEGE_MAX_ID) {
			nt = this.getCollegeById(id);
		} else if (id > Stage.JUNIOR_SCHOOL_MIN_ID && id < Stage.JUNIOR_SCHOOL_MAX_ID) {
			nt = this.getJuniorSchoolById(id);
		} else
			nt = this.regions.get(new Integer(id));
		if (nt == null) {
			nt = new Network();
			nt.setId(id);
			nt.setName("");
		}
		return nt;
	}

	public static void main(String args[]) {
		System.out.println(NetworkManager.getInstance().getUnivs());
	}

}
