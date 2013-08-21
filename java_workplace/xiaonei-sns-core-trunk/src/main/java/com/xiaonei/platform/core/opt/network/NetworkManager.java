package com.xiaonei.platform.core.opt.network;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.concurrent.DefaultThreadFactory;
import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.JuniorSchool;
import com.xiaonei.platform.core.model.Network;
import com.xiaonei.platform.core.model.Region;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.jdbc.dao.NetworkDao;
import com.xiaonei.platform.core.opt.permission.define.Stage;

@SuppressWarnings("unchecked")
public class NetworkManager {

    private static volatile NetworkManager instance;

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

    private static Log logger = LogFactory.getLog(NetworkManager.class);
    
    public static NetworkManager getInstance() {
        if (instance == null) {
            synchronized (NetworkManager.class) {
                if (instance == null) {
                	long step = System.currentTimeMillis();
                	instance = new NetworkManager();
                	if (logger.isInfoEnabled()) {
                    	logger.info("'new NetworkManager()' cost " + (System.currentTimeMillis() - step) + "ms");
                    }
                }
            }
        }
        return instance;
    }
    
    private ExecutorService workers = Executors.newCachedThreadPool(new DefaultThreadFactory("NetworkManager"));
    
    private NetworkManager() {
    	reload();
    }

    public void reload() {
    	
    	List<Future<?>> futures = new ArrayList<Future<?>>();
    	
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadHighSchool();
				if (logger.isInfoEnabled()) {
					logger.info("reloadHeighSchool cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
		
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadUniv();
				if (logger.isInfoEnabled()) {
					logger.info("reloadUniv cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
		
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadWorkplace();
				if (logger.isInfoEnabled()) {
					logger.info("reloadWorkplace cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
		
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadRegion();
				if (logger.isInfoEnabled()) {
					logger.info("reloadRegion cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
		
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadCollege();
				if (logger.isInfoEnabled()) {
					logger.info("reloadCollege cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
		
    	futures.add(workers.submit(new Runnable() {
			@Override
			public void run() {
				long step = System.currentTimeMillis();
				reloadJuniorSchool();
				if (logger.isInfoEnabled()) {
					logger.info("reloadJuniorSchool cost "
							+ (System.currentTimeMillis() - step) + "ms");
				}
			}
		}));
    	
    	
    	//等待分发的异步任务都执行完成
    	for(Future<?> future : futures) {
    		try {
				future.get();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
			}
    	}
    	
        /*this.reloadHeighSchool();
        this.reloadUniv();
        this.reloadWorkplace();
        this.reloadRegion();
        this.reloadCollege();
        this.reloadJuniorSchool();*/
        //		System.out.println("-------univ:" + this.univs.size());
        //		System.out.println("-------hss:" + this.hss.size());
        //		System.out.println("-------wps:" + this.wps.size());
        //		System.out.println("-------zones:" + this.regions.size());
        /*if (univs == null) {
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
        }*/
    }

    

    
	private void reloadUniv() {
		
		
		
        if ((this.univs = (Map<Integer, University>) localReload("univs.db")) == null) {
            this.univs = NetworkDao.getInstance().getAllUniversity();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(univs, "univs.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllUniversity(), "univs.db");
                }
            });
        }
    }

    private void reloadHighSchool() {
        if ((this.hss = (Map<Integer, HighSchool>) localReload("hss.db")) == null) {
            this.hss = NetworkDao.getInstance().getAllHighSchool();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(hss, "hss.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllHighSchool(), "hss.db");
                }
            });
        }

    }

    private void reloadWorkplace() {
        if ((this.wps = (Map<Integer, Workplace>) localReload("wps.db")) == null) {
            this.wps = NetworkDao.getInstance().getAllWorkplace();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(wps, "wps.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllWorkplace(), "wps.db");
                }
            });
        }

    }

    private void reloadRegion() {
        if ((this.regions = (Map<Integer, Region>) localReload("regions.db")) == null) {
            this.regions = NetworkDao.getInstance().getAllRegion();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(regions, "regions.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllRegion(), "regions.db");
                }
            });
        }
    }

    private void reloadCollege() {
        if ((this.college = (Map<Integer, College>) localReload("college.db")) == null) {
            this.college = NetworkDao.getInstance().getAllCollege();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(college, "college.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllCollege(), "college.db");
                }
            });
        }
    }

    private void reloadJuniorSchool() {
        if ((this.juniorschool = (Map<Integer, JuniorSchool>) localReload("juniorschool.db")) == null) {
            this.juniorschool = NetworkDao.getInstance().getAllJuniorSchool();
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(juniorschool, "juniorschool.db");
                }
            });
        } else {
            workers.submit(new Runnable() {

                @Override
                public void run() {
                    storeLocal(NetworkDao.getInstance().getAllJuniorSchool(), "juniorschool.db");
                }
            });
        }

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
        return this.univs.get(id);

    }

    public HighSchool getHighSchoolById(int id) {
        return this.hss.get(id);

    }

    public Region getRegionByid(int id) {
        return this.regions.get(id);
    }

    public Workplace getWorkplaceById(int id) {
        return this.wps.get(id);

    }

    public Region getRegionById(int id) {
        return this.regions.get(id);
    }

    public College getCollegeById(int id) {
        return this.college.get(id);
    }

    public JuniorSchool getJuniorSchoolById(int id) {
        return this.juniorschool.get(id);
    }

    public Network getNetById(int id) {
        //System.out.println("--------networkid:" + id);
        Network nt = null;
        if (id >= Stage.HEIGHSCHOOL_MIN_ID && id <= Stage.HEIGHSCHOOL_MAX_ID) nt = this.hss
                .get(id);
        else if (id >= Stage.UNIV_MIN_ID && id <= Stage.UNIV_MAX_ID) {

            nt = this.univs.get(id);

        } else if (id >= Stage.MAJOR_MIN_ID && id <= Stage.MAJOR_MAX_ID) {
            nt = this.wps.get(id);
        } else if (id > Stage.COLLEGE_MIN_ID && id < Stage.COLLEGE_MAX_ID) {
            nt = this.getCollegeById(id);
        } else if (id > Stage.JUNIOR_SCHOOL_MIN_ID && id < Stage.JUNIOR_SCHOOL_MAX_ID) {
            nt = this.getJuniorSchoolById(id);
        } else nt = this.regions.get(id);
        if (nt == null) {
            nt = new Network();
            nt.setId(id);
            nt.setName("");
            //nt.setName("没有网络"+id);
            //System.out.println("NoNetwork==" + id);
        }
        return nt;
    }

    public static void main(String args[]) {

        System.out.println(NetworkManager.getInstance().getUnivs());

    }

    private Map<Integer, ? extends Network> localReload(String dbName) {
        File fileDb = new File(dbName);
        if (!fileDb.isFile() || !fileDb.exists()) return null;//无文件，返回
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(fileDb);
            if (fis.available() == 0) return null;//文件大小为0，返回
        } catch (FileNotFoundException e) {
            return null;
        } catch (IOException e) {
            return null;
        } finally {
            if (fis != null) {
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        Map<Integer, Network> retMap;
        try {
            ObjectInputStream ois = new ObjectInputStream(fis);
            retMap = null;
            retMap = (Map<Integer, Network>) ois.readObject();
        } catch (IOException e) {
            return null;//文件格式出错，返回
        } catch (ClassNotFoundException e) {
            return null;//转类型出错，返回
        } catch (ClassCastException cce) {
            return null;//转类型出错，返回
        } finally {

        }
        return retMap;
    }

    private void storeLocal(Map<Integer, ? extends Network> storeMap, String dbName) {
        File fileDb = new File(dbName);
        boolean succ = fileDb.delete();
        if (!succ) {
        	logger.error("failed to delete " + dbName);
        }
        try {
            fileDb.createNewFile();
        } catch (IOException e) {
        	e.printStackTrace();
            return;
        }
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(new FileOutputStream(fileDb));
            oos.writeObject(storeMap);
            oos.flush();
        } catch (FileNotFoundException e) {
            return;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (oos != null) {
                try {
                    oos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }
}
