package com.xiaonei.xce.friendsrecentphotos;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ClientInterface;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.friendsphotos.FriendsRecentPhotosManagerPrx;
import xce.friendsphotos.FriendsRecentPhotosManagerPrxHelper;
import xce.friendsphotos.FriendsRecentPhotosWriterPrx;
import xce.friendsphotos.FriendsRecentPhotosWriterPrxHelper;
import xce.friendsphotos.Photo;
import xce.friendsphotos.PhotoResult;
import xce.friendsphotos.UserPhotos;
import Ice.ConnectTimeoutException;
import Ice.TimeoutException;

import com.xiaonei.xce.log.Oce;

public class FriendsRecentPhotosAdapter extends ReplicatedClusterAdapter {
  private FriendsRecentPhotosAdapter() {
    super(friendsRecentPhotosManagerEndpoints, interval);
  }

  private static final String friendsRecentPhotosManagerEndpoints = "ControllerFriendsRecentPhotosManager";
  private static final String friendsRecentPhotosWriterEndpoints = "ControllerFriendsRecentPhotosWriter";
  private static int interval = 120;

  private static FriendsRecentPhotosAdapter _instance = new FriendsRecentPhotosAdapter();
  
  private static ClientInterface _friendsRecentPhotosWriterClient;
  
  static {
    try{
      _friendsRecentPhotosWriterClient = new ClientInterface(friendsRecentPhotosWriterEndpoints,
          interval);
    }catch(Throwable e){
      e.printStackTrace();
    }
  }

  /**
   * 获取FriendsRecentPhotosAdapter对象实例
   * @return  获取的FriendsRecentPhotosAdapter对象实例
   */
  public static FriendsRecentPhotosAdapter getInstance() {
    return _instance;
  }
  
  private UserPhotos[] toUserPhotosArray(final List<UserPhotosInfo> userPhotosInfo) {
    UserPhotos[] result = new UserPhotos[userPhotosInfo.size()];
    for (int i = 0; i < userPhotosInfo.size(); ++i) {
      UserPhotos userPhotos = new UserPhotos();
      userPhotos.userId = userPhotosInfo.get(i).getUserId();
      List<PhotoInfo> photoInfos = userPhotosInfo.get(i).getPhotoInfos();
      userPhotos.photos = new Photo[photoInfos.size()];
      for (int j = 0; j < userPhotos.photos.length; ++j) {
        userPhotos.photos[j] = new Photo(photoInfos.get(j).getUploadTime(), photoInfos.get(j).getPhotoId());
      }
      result[i] = userPhotos;
    }
    return result;
  }
  
  private int[] intListToArray(final List<Integer> nums) {
    int[] result = new int[nums.size()];
    for (int i = 0; i < nums.size(); ++i) {
      result[i] = nums.get(i).intValue();
    }
    return result;
  }
  
  /**
   * 获取用户的好友的最新照片数据
   * 照片数据按照上传时间(照片id)排序
   * @param userId  用户的id
   * @param begin   要获取的最新照片的偏移量
   * @param limit   要获取最新照片的最大数目
   * @return  该用户的好友的最新照片数据
   */
  public PhotosResultInfo getFriendsRecentPhotos(int userId, int begin, int limit) {
    FriendsRecentPhotosManagerPrx prx = null;
    PhotoResult result = null;
    try {
      prx = getFriendsRecentPhotosManagerPrx();
      result = prx.getFriendsRecentPhotos(userId, begin, limit);
    } catch (ConnectTimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getFriendsRecentPhotos [" + prx
              + "] userId = " + userId + " begin = " + begin + " limit = " + limit + " " + e);
    } catch (TimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getFriendsRecentPhotos [" + prx
              + "] userId = " + userId + " begin = " + begin + " limit = " + limit + " " + e);
    } catch (Exception e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getFriendsRecentPhotos [" + prx
              + "] userId = " + userId + " begin = " + begin + " limit = " + limit + " " + e);
    }
    return new PhotosResultInfo(result);
  }
  
  /**
   * 获取用户的每个好友的最近num个照片，每个好友的num个照片按时间有序，好友之间按每人的最近照片上传时间有序
   * @param userId  用户id
   * @param begin   每次取好友的偏移量
   * @param limit   每次取好友的数量
   * @param num     取每个好友照片的数量，如果一个好友的照片数量小于num，则取实际的数目
   * @return    用户好友照片数据的列表
   */
  public List<UserPhotosInfo> getFriendsRecentFixedNumPhotos(int userId, int begin, int limit, int num) {
      FriendsRecentPhotosManagerPrx prx = null;
      PhotoResult result = null;
      try {
          prx = getFriendsRecentPhotosManagerPrx();
          result = prx.getFriendsRecentFixedNumPhotos(userId, begin, limit, num);
      }  catch (ConnectTimeoutException e) {
          Oce.getLogger().error(
                  this.getClass().getName() + ".getFriendsRecentFixedNumPhotos [" + prx + "] userId = " 
                          + userId + " begin = " + begin + " limit = " + limit + " num = " + num + " " + e);
      } catch (TimeoutException e) {
          Oce.getLogger().error(
                  this.getClass().getName() + ".getFriendsRecentFixedNumPhotos [" + prx + "] userId = "
                          + userId + " begin = " + begin + " limit = " + limit + " num = " + num + " " + e);
      } catch (Exception e) {
          Oce.getLogger().error(
                  this.getClass().getName() + ".getFriendsRecentFixedNumPhotos [" + prx + "] userId = "
                          + userId + " begin = " + begin + " limit = " + limit + " num = " + num + e);
      }
      return (new PhotosResultInfo(result)).getUserPhotos();
  }
  
  /**
   * 获取指定用户最近上传的照片，按照时间有序
   * @param userId  要获取的用户id
   * @return  该用户最近上传照片的数据
   */
  public UserPhotosInfo getUserRecentPhotos(int userId) {
    FriendsRecentPhotosManagerPrx prx = null;
    UserPhotosInfo result = null;
    try {
      prx = getFriendsRecentPhotosManagerPrx();
      UserPhotos rawData = prx.getUserRecentPhotos(userId);
      if (rawData.userId == -1) {
        rawData.userId = userId;
      }
      result = new UserPhotosInfo(rawData);
//      System.out.println("The result of userId = " + userId + " is " + result.toString()
//          + " in getUserRecentPhotos");
    } catch (ConnectTimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getUserRecentPhotos [" + prx
              + "] userId = " + userId + " " + e);
    } catch (TimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getUserRecentPhotos [" + prx
              + "] userId = " + userId + " " + e);
    } catch (Exception e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".getUserRecentPhotos [" + prx
              + "] userId = " + userId + " " + e);
    }
    return result;
  }
  
  /**
   * 批量添加用户最近上传的照片信息
   * 如果用户原来没有要添加的相册，那么就添加；
   * 如果原来有就把照片加到原来相册照片的前面，有可能会把原来相册的照片挤掉，因为每个相册的大小有限制
   * @param userPhotosInfo  要添加的用户照片信息
   */
  public void addUserPhotos(final List<UserPhotosInfo> userPhotosInfo) {
    FriendsRecentPhotosWriterPrx prx = null;
    try {
      prx = getFriendsRecentPhotosWriterPrx();
      prx.addRecentPhotos(toUserPhotosArray(userPhotosInfo));
    } catch (ConnectTimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".addUserPhotos [" + prx
              + "] size  = " + userPhotosInfo.size() + " " + e);
    } catch (TimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".addUserPhotos [" + prx
              + "] size = " + userPhotosInfo.size() + " " + e);
    } catch (Exception e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".addUserPhotos [" + prx
              + "] size= " + userPhotosInfo.size() + " " + e);
    }
  }
  
  /**
   * 批量删除用户上传的照片信息
   * @param userPhotosInfo  要删除的用户上传的照片信息
   */
  public void deleteUserPhotos(final List<UserPhotosInfo> userPhotosInfo) {
    FriendsRecentPhotosWriterPrx prx = null;
    try {
      prx = getFriendsRecentPhotosWriterPrx();
      prx.deleteUserPhotos(toUserPhotosArray(userPhotosInfo));
    } catch (ConnectTimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".deleteUserPhotos [" + prx
              + "] size  = " + userPhotosInfo.size() + " " + e);
    } catch (TimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".deleteUserPhotos [" + prx
              + "] size = " + userPhotosInfo.size() + " " + e);
    } catch (Exception e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".deleteUserPhotos [" + prx
              + "] size= " + userPhotosInfo.size() + " " + e);
    }
  }
  
  /**
   * 重新从数据库加载一批用户的最新照片信息
   * @param userIds 要加载的用户id序列
   */
  public void reloadUserPhotos(final List<Integer> userIds) {
    FriendsRecentPhotosWriterPrx prx = null;
    try {
      prx = getFriendsRecentPhotosWriterPrx();
      prx.reloadUserPhotos(intListToArray(userIds));
    } catch (ConnectTimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".reloadUserPhotos [" + prx
              + "] size  = " + userIds.size() + " " + e);
    } catch (TimeoutException e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".reloadUserPhotos [" + prx
              + "] size = " + userIds.size() + " " + e);
    } catch (Exception e) {
      Oce.getLogger().error(
          this.getClass().getName() + ".reloadUserPhotos [" + prx
              + "] size= " + userIds.size() + " " + e);
    }
  }
  
  protected FriendsRecentPhotosManagerPrx getFriendsRecentPhotosManagerPrx() {
    Ice.ObjectPrx prx0 = getValidProxy(0, "FRPM");
    if (prx0 != null) {
      FriendsRecentPhotosManagerPrx prx = FriendsRecentPhotosManagerPrxHelper
          .uncheckedCast(prx0.ice_timeout(300).ice_twoway());
      return prx;
    } else {
      return null;
    }
  }
  
  protected FriendsRecentPhotosWriterPrx getFriendsRecentPhotosWriterPrx() {
    Ice.ObjectPrx prx0 = _friendsRecentPhotosWriterClient.getValidProxy(0);
    if (prx0 != null) {
      FriendsRecentPhotosWriterPrx prx = FriendsRecentPhotosWriterPrxHelper
          .uncheckedCast(prx0.ice_timeout(300).ice_twoway());
      return prx;
    } else {
      return null;
    }
  }
  
  public static void main(String args[]) {
      int userId = 200308413;
      int begin = 0;
      int limit = 20;
      int num = 2;
      FriendsRecentPhotosAdapter adapter = FriendsRecentPhotosAdapter.getInstance();
      
  /*    List<UserPhotosInfo> userPhotosInfo2 = new ArrayList<UserPhotosInfo>();
      List<Integer> uploadTimes = new ArrayList<Integer>();
      uploadTimes.add(new Integer(1337429952));
 //     uploadTimes.add(new Integer(1337257547));
      List<Long> photoIds = new ArrayList<Long>();
      photoIds.add(new Long(232342342146l));
 //     photoIds.add(new Long(23423424322l));
      UserPhotosInfo userPhoto = new UserPhotosInfo(123321, uploadTimes, photoIds);
      userPhotosInfo2.add(userPhoto);
      FriendsRecentPhotosAdapter.getInstance().addUserPhotos(userPhotosInfo2);
      List<Integer> newUserId = new ArrayList<Integer>();
      newUserId.add(123321);
      adapter.reloadUserPhotos(newUserId);*/
      
      
      List<UserPhotosInfo> result = adapter.getFriendsRecentFixedNumPhotos(userId, begin, limit, num);
      for(UserPhotosInfo userPhotos : result) {
          System.out.print("userid:" + userPhotos.getUserId() + ", ");
          for(PhotoInfo photo : userPhotos.getPhotoInfos()) {
              System.out.print(", photo(uploadTime, photoId):" + photo.getUploadTime() +
                      ", " + photo.getPhotoId());
          }
          System.out.println();
      }
      System.exit(0);
      
    List<Integer> userIds = new ArrayList<Integer>();
    userIds.add(new Integer(67913));
    try {
      Thread.sleep(4000);
    } catch (InterruptedException e1) {
      // TODO Auto-generated catch block
      e1.printStackTrace();
    }
    FriendsRecentPhotosAdapter.getInstance().reloadUserPhotos(userIds);
 //   List<UserPhotosInfo> userPhotosInfo = new ArrayList<UserPhotosInfo>();
 /*   List<Integer> uploadTimes = new ArrayList<Integer>();
    System.out.println(Integer.MAX_VALUE);
    uploadTimes.add(new Integer(1320921989));
    uploadTimes.add(new Integer(123414));
    List<Long> photoIds = new ArrayList<Long>();
    photoIds.add(new Long(232342342342l));
    photoIds.add(new Long(23423424322l));
    UserPhotosInfo userPhoto = new UserPhotosInfo(227198911, uploadTimes, photoIds);
    userPhotosInfo.add(userPhoto);
    FriendsRecentPhotosAdapter.getInstance().addUserPhotos(userPhotosInfo);
    try {
      Thread.sleep(4000);
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    System.out.println(FriendsRecentPhotosAdapter.getInstance().getUserRecentPhotos(227198911));*/
    System.exit(0);
  }
}
