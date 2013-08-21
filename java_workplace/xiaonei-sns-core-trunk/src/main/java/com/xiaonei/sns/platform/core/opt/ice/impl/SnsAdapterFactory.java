package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.lang.reflect.Proxy;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import com.xiaonei.platform.core.control.BlockCounter;
import com.xiaonei.platform.core.control.BlockCounterFactory;
import com.xiaonei.platform.core.control.BlockExecutor;
import com.xiaonei.platform.core.control.BlockService;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyApplyCacheAdapterN;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyByAddTimeCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyByIdCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyByNameCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyByOnlineTimeCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyCoreAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IBuddyGroupAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IFootprintAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IFriendDistAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IFriendFinderByCommonAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IFriendFinderGuideReplicaAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IOfferFriendsAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IRegisterable;
import com.xiaonei.sns.platform.core.opt.ice.ISearchAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserBasicAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserBornAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserConfigInfoAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserContactAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserDescAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserDoingAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserFavAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserLoginCountAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserNameAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserPassportAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserRightAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserStageAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserStateAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserStateCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserTimeAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserUrlAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IUserViewAdapter;
import com.xiaonei.sns.platform.core.opt.ice.IVisitFootprintAdapter;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheDataN;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheWithTotalN;

/**
 * SNS Adapter factory<br>
 * This factory provides access to XCE service.<br />
 * All the method in the {@link IRegisterable} interface can be blocked and
 * cutted by this factory.
 * 
 * @author xylz(xylz@live.cn)
 * @since 20091022
 * @see {@linkplain BlockCounterFactory}
 * @see {@linkplain BlockCounter}
 * @see {@linkplain BlockService}
 * @see {@linkplain BlockExecutor}
 * @see {@linkplain SnsBlockCutInvocationHandler}
 */
@SuppressWarnings("deprecation")
public class SnsAdapterFactory {

	private static volatile IUserLoginCountAdapter userLoginCountAdapter = null;

	private static volatile IFootprintAdapter footprintAdapter = null;
	
	private static volatile IVisitFootprintAdapter visitfootprintAdapter = null;

	// private static volatile IBuddyApplyCacheAdapter buddyApplyCacheAdapter =
	// null;

	private static volatile IFriendDistAdapter friendDistAdapter = null;

	private static volatile IUserAdapter userAdapter = null;

	private static volatile IUserBasicAdapter userBasicAdapter = null;

	private static volatile IUserCacheAdapter userCacheAdapter = null;

	private static volatile IUserConfigInfoAdapter userConfigInfoAdapter = null;

	private static volatile IUserDescAdapter userDescAdapter = null;

	private static volatile IUserViewAdapter userViewAdapter = null;

	private static volatile IOfferFriendsAdapter offerFriendsAdapter = null;

	private static volatile IFriendFinderGuideReplicaAdapter friendFinderGuideReplicaAdapter = null;

	private static volatile IFriendFinderByCommonAdapter friendFinderByCommonAdapter = null;



	private static volatile IBuddyByIdCacheAdapter buddyByIdCacheAdapter = null;

	private static volatile IBuddyByNameCacheAdapter buddyByNameCacheAdapter = null;

	private static volatile IBuddyByAddTimeCacheAdapter buddyByAddTimeCacheAdapter = null;

	private static volatile IBuddyByOnlineTimeCacheAdapter buddyByOnlineTimeCacheAdapter = null;

	private static volatile IBuddyGroupAdapter buddyGroupAdapter = null;

	private static volatile IBuddyCoreAdapter buddyCoreAdapter = null;

	private static volatile ISearchAdapter searchAdapter = null;

	private static volatile IBuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN> buddyApplyCacheAdapterN = null;

	private static ConcurrentMap<String, IRegisterable> userRegisterables = new ConcurrentHashMap<String, IRegisterable>();

	private static volatile IUserNameAdapter userNameAdapter;

	private static volatile IUserStateAdapter userStateAdapter;

	private static volatile IUserUrlAdapter userUrlAdapter;

	private static volatile IUserPassportAdapter userPassportAdapter;

	private static volatile IUserRightAdapter userRightAdapter;

	private static volatile IUserBornAdapter userBornAdapter;

	private static volatile IUserStageAdapter userStageAdapter;

	private static volatile IUserDoingAdapter userDoingAdapter;
	private static volatile IUserTimeAdapter userTimeAdapter;
	private static volatile IUserFavAdapter userFavAdapter;
	private static volatile IUserContactAdapter userContactAdapter;

    private static volatile IUserStateCacheAdapter userStateCacheAdapter;

	/**
	 * Register user define class for SNS Adapter.<br>
	 * This method must be called before calling the method of getting instance.<br>
	 * For example in following code, the method of
	 * <code>{@link #registerSnsAdapter(Class, IRegisterable)}</code> is called
	 * before we get the instance
	 * <code>{@link #getBuddyApplyCacheAdapter()}</code>.
	 * 
	 * <pre>
	 * registerSnsAdapter(IBuddyApplyCacheAdapter.class,
	 * 		new IBuddyApplyCacheAdapter() {
	 * 
	 * 			public WBuddyApplyCacheWithTotal getBuddyApplyCacheWithTotal(
	 * 					int userId, int begin, int count) {
	 * 				System.out.println(&quot;getBuddyApplyCacheWithTotal&quot;);
	 * 				return null;
	 * 			}
	 * 
	 * 			public List&lt;WBuddyApplyCacheData&gt; getBuddyApplyCache(int userId,
	 * 					int begin, int count) {
	 * 				System.out.println(&quot;getBuddyApplyCache&quot;);
	 * 				return null;
	 * 			}
	 * 
	 * 			public String toString() {
	 * 				return &quot;Good good study, day day up&quot;;
	 * 			}
	 * 		});
	 * IBuddyApplyCacheAdapter iba = getBuddyApplyCacheAdapter();
	 * System.out.println(iba.toString());
	 * iba.getBuddyApplyCache(0, 0, 0);
	 * iba.getBuddyApplyCacheWithTotal(0, 0, 0);
	 * </pre>
	 * 
	 * @param clazz
	 *            the interface for register
	 * @param registerable
	 *            the object for register<br>
	 *            The <code>registerable</code> must be the instance of
	 *            <code>clazz</code>.
	 * @since 20091026
	 */
	public static <T extends IRegisterable> void registerSnsAdapter(
			Class<T> clazz, T registerable) {
		if (clazz == null)
			throw new NullPointerException("The clazz is empty.");
		if (userRegisterables.containsKey(clazz)) {
			throw new IllegalStateException(
					"This 'IRegisterable' is registered by:"
							+ userRegisterables.get(clazz.getName()));
		}
		userRegisterables.put(clazz.getName(), registerable);

	}

	@SuppressWarnings("unchecked")
	private static <T extends IRegisterable> T getSnsAdapter(Class<T> clazz) {
		IRegisterable ir = userRegisterables.get(clazz.getName());
		return (T) ir;
	}

	public static IFootprintAdapter getFootprintAdapter() {
		if (footprintAdapter == null) {
			synchronized (IFootprintAdapter.class) {
				if (footprintAdapter == null) {
					footprintAdapter = createAdapter(//
							IFootprintAdapter.class,//
							getSnsAdapter(IFootprintAdapter.class) != null ? //
							getSnsAdapter(IFootprintAdapter.class)
									: new WFootprintAdapter(), "footprint");
				}
			}
		}
		return footprintAdapter;
	}
	
	public static IVisitFootprintAdapter getVisitFootprintAdapter() {
		if (visitfootprintAdapter == null) {
			synchronized (IVisitFootprintAdapter.class) {
				if (visitfootprintAdapter == null) {
					visitfootprintAdapter = createAdapter(//
							IVisitFootprintAdapter.class,//
							getSnsAdapter(IVisitFootprintAdapter.class) != null ? //
									getSnsAdapter(IVisitFootprintAdapter.class)
									: new WVisitFootprintAdapter(), "visitfootprint");
				}
			}
		}
		return visitfootprintAdapter;
	}

	// public static IBuddyApplyCacheAdapter getBuddyApplyCacheAdapter() {
	// if (buddyApplyCacheAdapter == null) {
	// synchronized (IBuddyApplyCacheAdapter.class) {
	// if (buddyApplyCacheAdapter == null) buddyApplyCacheAdapter =
	// createAdapter(
	// IBuddyApplyCacheAdapter.class,//
	// getSnsAdapter(IBuddyApplyCacheAdapter.class) != null ? //
	// getSnsAdapter(IBuddyApplyCacheAdapter.class)
	// : new WBuddyApplyCacheAdapter(), "buddyapply");
	// }
	// }
	// return buddyApplyCacheAdapter;
	// }

	public static IBuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN> getBuddyApplyCacheAdapter() {
		return getBuddyApplyCacheAdapterN();
	}

	@SuppressWarnings("unchecked")
	public static IBuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN> getBuddyApplyCacheAdapterN() {
		if (buddyApplyCacheAdapterN == null) {
			synchronized (IBuddyApplyCacheAdapterN.class) {
				if (buddyApplyCacheAdapterN == null) {
					buddyApplyCacheAdapterN = createAdapter(
							IBuddyApplyCacheAdapterN.class,//
							getSnsAdapter(IBuddyApplyCacheAdapterN.class) != null ? //
							getSnsAdapter(IBuddyApplyCacheAdapterN.class)
									: new WBuddyApplyCacheAdapterN(),
							"buddyApplyCacheAdapterN");
				}
			}
		}
		return buddyApplyCacheAdapterN;
	}

	// WBuddyApplyCacheAdapterN

	public static IFriendDistAdapter getFriendDistAdapter() {
		if (friendDistAdapter == null) {
			synchronized (IFriendDistAdapter.class) {
				if (friendDistAdapter == null) {
					friendDistAdapter = createAdapter(IFriendDistAdapter.class,//
							getSnsAdapter(IFriendDistAdapter.class) != null ? // 
							getSnsAdapter(IFriendDistAdapter.class)
									: new WFriendDistAdapter(), "frienddist");
				}
			}
		}
		return friendDistAdapter;
	}

	public static IUserAdapter getUserAdapter() {
		if (userAdapter == null) {
			synchronized (IUserAdapter.class) {
				if (userAdapter == null) {
					userAdapter = createAdapter(IUserAdapter.class,//
							getSnsAdapter(IUserAdapter.class) != null ? //
							getSnsAdapter(IUserAdapter.class)
									: new WUserAdapter(), "user");
				}
			}
		}
		return userAdapter;
	}

	public static IUserBasicAdapter getUserBasicAdapter() {
		if (userBasicAdapter == null) {
			synchronized (IUserBasicAdapter.class) {
				if (userBasicAdapter == null) {
					userBasicAdapter = createAdapter(IUserBasicAdapter.class,//
							getSnsAdapter(IUserBasicAdapter.class) != null ? //
							getSnsAdapter(IUserBasicAdapter.class)
									: new WUserBasicAdapter(), "userbasic");
				}
			}
		}
		return userBasicAdapter;
	}

	public static IUserCacheAdapter getUserCacheAdapter() {
		if (userCacheAdapter == null) {
			synchronized (IUserCacheAdapter.class) {
				if (userCacheAdapter == null) {
					userCacheAdapter = createAdapter(IUserCacheAdapter.class,//
							getSnsAdapter(IUserCacheAdapter.class) != null ? //
							getSnsAdapter(IUserCacheAdapter.class)
									: new WUserCacheAdapter(), "usercache");
				}
			}
		}
		return userCacheAdapter;
	}

	public static IUserConfigInfoAdapter getUserConfigInfoAdapter() {
		if (userConfigInfoAdapter == null) {
			synchronized (IUserConfigInfoAdapter.class) {
				if (userConfigInfoAdapter == null) {
					userConfigInfoAdapter = createAdapter(
							IUserConfigInfoAdapter.class,//
							getSnsAdapter(IUserConfigInfoAdapter.class) != null ? //
							getSnsAdapter(IUserConfigInfoAdapter.class)
									: new WUserConfigInfoAdapter(),
							"userconfiginfo");
				}
			}
		}
		return userConfigInfoAdapter;
	}

	public static IUserDescAdapter getUserDescAdapter() {
		if (userDescAdapter == null) {
			synchronized (IUserDescAdapter.class) {
				if (userDescAdapter == null) {
					userDescAdapter = createAdapter(IUserDescAdapter.class,//
							getSnsAdapter(IUserDescAdapter.class) != null ? //
							getSnsAdapter(IUserDescAdapter.class)
									: new WUserDescAdapter(), "userdesc");
				}
			}
		}
		return userDescAdapter;
	}

	@Deprecated
	public static IUserViewAdapter getUserViewAdapter() {
		if (userViewAdapter == null) {
			synchronized (IUserViewAdapter.class) {
				if (userViewAdapter == null) {
					userViewAdapter = createAdapter(IUserViewAdapter.class,//
							getSnsAdapter(IUserViewAdapter.class) != null ? //
							getSnsAdapter(IUserViewAdapter.class)
									: new WUserViewAdapter(), "userview");
				}
			}
		}
		return userViewAdapter;
	}

	@SuppressWarnings("unchecked")
	private static <T> T createAdapter(Class<T> c, final T obj,
			final String name) {
		if (!c.isInterface())
			throw new IllegalArgumentException(
					"the class type must be an interface");
		return (T) Proxy.newProxyInstance(//
				SnsAdapterFactory.class.getClassLoader(),//
				new Class[] { c },//
				new SnsBlockCutInvocationHandler(name, obj));
	}

	public static void main(String[] args) {
		// ConfigCenter.setBoolean("debug_snsadapter", true);
		// registerSnsAdapter(IBuddyApplyCacheAdapter.class, new
		// IBuddyApplyCacheAdapter() {
		//
		// public WBuddyApplyCacheWithTotal getBuddyApplyCacheWithTotal(int
		// userId, int begin,
		// int count) {
		// System.out.println("getBuddyApplyCacheWithTotal");
		// return null;
		// }
		//
		// public List<WBuddyApplyCacheData> getBuddyApplyCache(int userId, int
		// begin, int count) {
		// // System.out.println("getBuddyApplyCache");
		// try {
		// Thread.sleep(1000L);
		// } catch (Exception e) {
		//
		// }
		// return null;
		// }
		//
		// public String toString() {
		// return "Good good study, day day up";
		// }
		// });
		// final IBuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN,
		// SnsBuddyApplyCacheWithTotalN> iba = getBuddyApplyCacheAdapter();
		// System.out.println(iba.toString());
		// iba.getBuddyApplyCache(0, 0, 0);
		// for (int i = 1; i <= 100; i++) {
		// new Thread() {
		//
		// @Override
		// public void run() {
		// while (true) {
		// iba.getBuddyApplyCache(0, 0, 0);
		// }
		// }
		// }.start();
		// }

	}

	
	/**
	 * 叶湘新服务
	 * 
	 * @author michael(liyong@opi-corp.com)
	 * @return
	 */
	public static IBuddyByIdCacheAdapter getBuddyByIdCacheAdapter() {
		if (buddyByIdCacheAdapter == null) {
			synchronized (IBuddyByIdCacheAdapter.class) {
				if (buddyByIdCacheAdapter == null) {
					buddyByIdCacheAdapter = createAdapter(
							IBuddyByIdCacheAdapter.class,//
							getSnsAdapter(IBuddyByIdCacheAdapter.class) != null ? //
							getSnsAdapter(IBuddyByIdCacheAdapter.class)
									: new WBuddyByIdCacheAdapter(),
							"buddyByIdCache");
				}
			}
		}
		return buddyByIdCacheAdapter;
	}

	/**
	 * 叶湘新服务
	 * 
	 * @author michael(liyong@opi-corp.com)
	 * @return
	 */
	public static IBuddyByNameCacheAdapter getBuddyByNameCacheAdapter() {
		if (buddyByNameCacheAdapter == null) {
			synchronized (IBuddyByNameCacheAdapter.class) {
				if (buddyByNameCacheAdapter == null) {
					buddyByNameCacheAdapter = createAdapter(
							IBuddyByNameCacheAdapter.class,//
							getSnsAdapter(IBuddyByNameCacheAdapter.class) != null ? //
							getSnsAdapter(IBuddyByNameCacheAdapter.class)
									: new WBuddyByNameCacheAdapter(),
							"buddyByNameCache");
				}
			}
		}
		return buddyByNameCacheAdapter;
	}

	/**
	 * 叶湘新服务
	 * 
	 * @author michael(liyong@opi-corp.com)
	 * @return
	 */
	public static IBuddyByAddTimeCacheAdapter getBuddyByAddTimeCacheAdapter() {
		if (buddyByAddTimeCacheAdapter == null) {
			synchronized (IBuddyByAddTimeCacheAdapter.class) {
				if (buddyByAddTimeCacheAdapter == null) {
					buddyByAddTimeCacheAdapter = createAdapter(
							IBuddyByAddTimeCacheAdapter.class,//
							getSnsAdapter(IBuddyByAddTimeCacheAdapter.class) != null ? //
							getSnsAdapter(IBuddyByAddTimeCacheAdapter.class)
									: new WBuddyByAddTimeCacheAdapter(),
							"buddyByAddTimeCache");
				}
			}
		}
		return buddyByAddTimeCacheAdapter;
	}

	/**
	 * 叶湘新服务
	 * 
	 * @author michael(liyong@opi-corp.com)
	 * @return
	 */
	public static IBuddyByOnlineTimeCacheAdapter getBuddyByOnlineTimeCacheAdapter() {
		if (buddyByOnlineTimeCacheAdapter == null) {
			synchronized (IBuddyByAddTimeCacheAdapter.class) {
				if (buddyByOnlineTimeCacheAdapter == null) {
					buddyByOnlineTimeCacheAdapter = createAdapter(
							IBuddyByOnlineTimeCacheAdapter.class,//
							getSnsAdapter(IBuddyByOnlineTimeCacheAdapter.class) != null ? //
							getSnsAdapter(IBuddyByOnlineTimeCacheAdapter.class)
									: new WBuddyByOnlineTimeCacheAdapter(),
							"buddyByOnlineTimeCache");
				}
			}
		}
		return buddyByOnlineTimeCacheAdapter;
	}

	/**
	 * 张楠新服务 二度好友
	 * 
	 * @return {@link IOfferFriendsAdapter}
	 * 
	 * @author michael(liyong@opi-corp.com)
	 * @author tai.wang@opi-corp.com Sep 14, 2010 - 10:55:11 AM
	 */
	public static IOfferFriendsAdapter getOfferFriendsAdapter() {
		if (offerFriendsAdapter == null) {
			synchronized (IOfferFriendsAdapter.class) {
				if (offerFriendsAdapter == null) {
					offerFriendsAdapter = createAdapter(
							IOfferFriendsAdapter.class,//
							getSnsAdapter(IOfferFriendsAdapter.class) != null ? //
							getSnsAdapter(IOfferFriendsAdapter.class)
									: new WOfferFriendsAdapter(),
							"friendFinderReplicaAdapter");
				}
			}
		}
		return offerFriendsAdapter;
	}

	/**
	 * 为profile做的friendfinder
	 * 
	 * @return
	 */
	public static IFriendFinderGuideReplicaAdapter getFriendFinderGuideReplicaAdapter() {
		if (friendFinderGuideReplicaAdapter == null) {
			synchronized (IOfferFriendsAdapter.class) {
				if (friendFinderGuideReplicaAdapter == null) {
					friendFinderGuideReplicaAdapter = createAdapter(
							IFriendFinderGuideReplicaAdapter.class,//
							getSnsAdapter(IFriendFinderGuideReplicaAdapter.class) != null ? //
							getSnsAdapter(IFriendFinderGuideReplicaAdapter.class)
									: new WFriendFinderGuideAdapter(),
							"friendFinderGuideReplicaAdapter");
				}
			}
		}
		return friendFinderGuideReplicaAdapter;
	}

	/**
	 * byCommon
	 * 
	 * @return
	 */
	public static IFriendFinderByCommonAdapter getFriendFinderByCommonAdapter() {
		if (friendFinderByCommonAdapter == null) {
			synchronized (IFriendFinderByCommonAdapter.class) {
				if (friendFinderByCommonAdapter == null) {
					friendFinderByCommonAdapter = createAdapter(
							IFriendFinderByCommonAdapter.class,//
							getSnsAdapter(IFriendFinderByCommonAdapter.class) != null ? //
							getSnsAdapter(IFriendFinderByCommonAdapter.class)
									: new WFriendFinderByCommonAdapter(),
							"friendFinderByCommonAdapter");
				}
			}
		}
		return friendFinderByCommonAdapter;
	}

	/**
	 * BuddyGroup
	 */
	public static IBuddyGroupAdapter getBuddyGroupAdapter() {
		if (buddyGroupAdapter == null) {
			synchronized (IBuddyGroupAdapter.class) {
				if (buddyGroupAdapter == null) {
					buddyGroupAdapter = createAdapter(IBuddyGroupAdapter.class,//
							getSnsAdapter(IBuddyGroupAdapter.class) != null ? //
							getSnsAdapter(IBuddyGroupAdapter.class)
									: new WBuddyGroupAdapter(), "buddyGroup");
				}
			}
		}
		return buddyGroupAdapter;
	}

	/**
	 * BuddyCore
	 */
	public static IBuddyCoreAdapter getBuddyCoreAdapter() {
		if (buddyCoreAdapter == null) {
			synchronized (IBuddyCoreAdapter.class) {
				if (buddyCoreAdapter == null) {
					buddyCoreAdapter = createAdapter(IBuddyCoreAdapter.class,//
							getSnsAdapter(IBuddyCoreAdapter.class) != null ? //
							getSnsAdapter(IBuddyCoreAdapter.class)
									: new WBuddyCoreAdapter(), "buddyCore");
				}
			}
		}
		return buddyCoreAdapter;
	}

	/**
	 * wrapper for search
	 * 
	 * @author xylz(xylz@live.cn)
	 * @since 20091113
	 */
	public static ISearchAdapter getSearchAdapter() {
		if (searchAdapter == null) {
			synchronized (ISearchAdapter.class) {
				if (searchAdapter == null) {
					searchAdapter = createAdapter(ISearchAdapter.class,//
							getSnsAdapter(ISearchAdapter.class) != null ? //
							getSnsAdapter(ISearchAdapter.class)
									: new WSearchAdapter(), "search");
				}
			}
		}
		return searchAdapter;
	}

	/**
	 * @return the userLoginCountAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserLoginCountAdapter getUserLoginCountAdapter() {
		if (userLoginCountAdapter == null) {
			synchronized (IUserLoginCountAdapter.class) {
				if (userLoginCountAdapter == null) {
					userLoginCountAdapter = createAdapter(
							IUserLoginCountAdapter.class,//
							getSnsAdapter(IUserLoginCountAdapter.class) != null ? //
							getSnsAdapter(IUserLoginCountAdapter.class)
									: new WUserLoginCountAdapter(),
							"userLoginCount");
				}
			}
		}
		return userLoginCountAdapter;
	}

	/**
	 * @return the userLoginCountAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserRightAdapter getUserRightAdapter() {
		if (userRightAdapter == null) {
			synchronized (IUserLoginCountAdapter.class) {
				if (userRightAdapter == null) {
					userRightAdapter = createAdapter(IUserRightAdapter.class,//
							getSnsAdapter(IUserRightAdapter.class) != null ? //
							getSnsAdapter(IUserRightAdapter.class)
									: new WUserRightAdapter(), "userRight");
				}
			}
		}
		return userRightAdapter;
	}

	/**
	 * @return the IUserNameAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserNameAdapter getUserNameAdapter() {
		if (userNameAdapter == null) {
			synchronized (IUserNameAdapter.class) {
				if (userNameAdapter == null) {
					userNameAdapter = createAdapter(IUserNameAdapter.class,//
							getSnsAdapter(IUserNameAdapter.class) != null ? //
							getSnsAdapter(IUserNameAdapter.class)
									: new WUserNameAdapter(), "userName");
				}
			}
		}
		return userNameAdapter;
	}

	/**
	 * @return the IUserStateAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserStateAdapter getUserStateAdapter() {
		if (userStateAdapter == null) {
			synchronized (IUserStateAdapter.class) {
				if (userStateAdapter == null) {
					userStateAdapter = createAdapter(IUserStateAdapter.class,//
							getSnsAdapter(IUserStateAdapter.class) != null ? //
							getSnsAdapter(IUserStateAdapter.class)
									: new WUserStateAdapter(), "userState");
				}
			}
		}
		return userStateAdapter;
	}

	
	public static IUserStateCacheAdapter getUserStateCacheAdapter() {
	    if (userStateCacheAdapter == null) {
            synchronized (IUserStateAdapter.class) {
                if (userStateCacheAdapter == null) {
                    userStateCacheAdapter = createAdapter(IUserStateCacheAdapter.class,//
                            getSnsAdapter(IUserStateCacheAdapter.class) != null ? //
                            getSnsAdapter(IUserStateCacheAdapter.class)
                                    : new WUserStateCacheAdapter(), "userStateCache");
                }
            }
        }
        return userStateCacheAdapter;
	}
	/**
	 * @return the IUserUrlAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserUrlAdapter getUserUrlAdapter() {
		if (userUrlAdapter == null) {
			synchronized (IUserUrlAdapter.class) {
				if (userUrlAdapter == null) {
					userUrlAdapter = createAdapter(IUserUrlAdapter.class,//
							getSnsAdapter(IUserUrlAdapter.class) != null ? //
							getSnsAdapter(IUserUrlAdapter.class)
									: new WUserUrlAdapter(), "userUrl");
				}
			}
		}
		return userUrlAdapter;
	}

	/**
	 * @return the IUserPassportAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100119
	 */
	public static IUserPassportAdapter getUserPassportAdapter() {
		if (userPassportAdapter == null) {
			synchronized (IUserPassportAdapter.class) {
				if (userPassportAdapter == null) {
					userPassportAdapter = createAdapter(
							IUserPassportAdapter.class,//
							getSnsAdapter(IUserPassportAdapter.class) != null ? //
							getSnsAdapter(IUserPassportAdapter.class)
									: new WUserPassportAdapter(),
							"userPassport");
				}
			}
		}
		return userPassportAdapter;
	}

	/**
	 * @return the IUserDoingAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100120
	 */
	public static IUserDoingAdapter getUserDoingAdapter() {
		if (userDoingAdapter == null) {
			synchronized (IUserPassportAdapter.class) {
				if (userDoingAdapter == null) {
					userDoingAdapter = createAdapter(IUserDoingAdapter.class,//
							getSnsAdapter(IUserDoingAdapter.class) != null ? //
							getSnsAdapter(IUserDoingAdapter.class)
									: new WUserDoingAdapter(), "userDoing");
				}
			}
		}
		return userDoingAdapter;
	}

	/**
	 * @return the IUserStageAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100120
	 */
	public static IUserStageAdapter getUserStageAdapter() {
		if (userStageAdapter == null) {
			synchronized (IUserPassportAdapter.class) {
				if (userStageAdapter == null) {
					userStageAdapter = createAdapter(IUserStageAdapter.class,//
							getSnsAdapter(IUserStageAdapter.class) != null ? //
							getSnsAdapter(IUserStageAdapter.class)
									: new WUserStageAdapter(), "userStage");
				}
			}
		}
		return userStageAdapter;
	}

	/**
	 * @return the IUserPassportAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 20100120
	 */
	public static IUserBornAdapter getUserBornAdapter() {
		if (userBornAdapter == null) {
			synchronized (IUserBornAdapter.class) {
				if (userBornAdapter == null) {
					userBornAdapter = createAdapter(IUserBornAdapter.class,//
							getSnsAdapter(IUserBornAdapter.class) != null ? //
							getSnsAdapter(IUserBornAdapter.class)
									: new WUserBornAdapter(), "userBorn");
				}
			}
		}
		return userBornAdapter;
	}

	/**
	 * @return the IUserTimeAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 2010-02-05
	 */
	public static IUserTimeAdapter getUserTimeAdapter() {
		if (userTimeAdapter == null) {
			synchronized (IUserTimeAdapter.class) {
				if (userTimeAdapter == null) {
					userTimeAdapter = createAdapter(IUserTimeAdapter.class,//
							getSnsAdapter(IUserTimeAdapter.class) != null ? //
							getSnsAdapter(IUserTimeAdapter.class)
									: new WUserTimeAdapter(), "usertime");
				}
			}
		}
		return userTimeAdapter;
	}

	/**
	 * @return the IUserContactAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 2010-02-05
	 */
	public static IUserContactAdapter getUserContactAdapter() {
		if (userContactAdapter == null) {
			synchronized (IUserContactAdapter.class) {
				if (userContactAdapter == null) {
					userContactAdapter = createAdapter(
							IUserContactAdapter.class,//
							getSnsAdapter(IUserContactAdapter.class) != null ? //
							getSnsAdapter(IUserContactAdapter.class)
									: new WUserContactAdapter(), "usercontact");
				}
			}
		}
		return userContactAdapter;
	}

	/**
	 * @return the IUserFavAdapter
	 * @author xylz (xylz@live.cn)
	 * @since 2010-02-05
	 */
	public static IUserFavAdapter getUserFavAdapter() {
		if (userFavAdapter == null) {
			synchronized (IUserFavAdapter.class) {
				if (userFavAdapter == null) {
					userFavAdapter = createAdapter(IUserFavAdapter.class,//
							getSnsAdapter(IUserFavAdapter.class) != null ? //
							getSnsAdapter(IUserFavAdapter.class)
									: new WUserFavAdapter(), "userfav");
				}
			}
		}
		return userFavAdapter;
	}
}
