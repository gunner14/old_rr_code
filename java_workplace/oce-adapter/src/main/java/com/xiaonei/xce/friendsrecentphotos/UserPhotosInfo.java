package com.xiaonei.xce.friendsrecentphotos;

import java.util.ArrayList;
import java.util.List;

import xce.friendsphotos.Photo;
import xce.friendsphotos.UserPhotos;

public class UserPhotosInfo {
  private int userId = 0;
  private List<PhotoInfo> photoInfos = new ArrayList<PhotoInfo>();
  
  public UserPhotosInfo(int userId, final List<PhotoInfo> photos) {
    this.userId = userId;
    if (photos != null) {
      for (PhotoInfo info : photos) {
        this.photoInfos.add(new PhotoInfo(info.getUploadTime(), info.getPhotoId()));
      }
    }
  }
  
  public UserPhotosInfo(int userId, final List<Integer> uploadTimes, final List<Long> photoIds) {
    this.userId = userId;
    for (int i = 0; i < uploadTimes.size(); ++i) {
      this.photoInfos.add(new PhotoInfo(uploadTimes.get(i), photoIds.get(i)));
    }
  }
  
  public UserPhotosInfo(final UserPhotos userPhotos) {
    if (userPhotos != null) {
      this.userId = userPhotos.userId;
      for (Photo photo : userPhotos.photos) {
        this.photoInfos.add(new PhotoInfo(photo));
      }
    }
  }
  
  /**
   * 获取最近一张照片的上传时间
   * @return
   */
  public int getLatestPhotoUploadTime() {
    if (this.photoInfos.size() != 0) {
      return this.photoInfos.get(0).getUploadTime();
    }
    return 0;
  }
  
  /**
   * 获取最近一张照片的id
   * @return
   */
  public long getLatestPhotoId() {
      if (this.photoInfos.size() != 0) {
          return this.photoInfos.get(0).getPhotoId();
      }
      return 0;
  }
  
  public int getUserId() {
    return this.userId;
  }
  
  public List<PhotoInfo> getPhotoInfos() {
    return this.photoInfos;
  }
  
  public void addPhotoInfo(final PhotoInfo info) {
    if (info != null) {
      this.photoInfos.add(new PhotoInfo(info.getUploadTime(), info.getPhotoId()));
    }
  }
  
  @Override
  public boolean equals(Object obj) {
    if (obj == null && this.photoInfos.isEmpty()) {
      return true;
    }
    if (obj == null || !(obj instanceof UserPhotosInfo)) {
      return false;
    } else {
      System.out.println("str1 = " + this + " str2 = " + obj);
      return this.toString().equals(obj.toString());
    }
  }
  
  @Override
  public String toString() {
    return "{userId = " + userId + " photoInfos = " + photoInfos + "}";
  }
}
