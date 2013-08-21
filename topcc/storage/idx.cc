#include "storage/vldb.h"
#include "storage/pack.h"
#include <stdlib.h>

namespace arch {

const char kPageMagicCode[] = "\xe4\xe5PS";
//const char kItemMagicCode[] = "\xe4\xe6";

int64 Align(int64 size, int fix) {
  int x = size % fix;
  if (x != 0)
    return size - x + fix;
  return size;
}

struct IndexItem {
  uint32 off;      // 256B align
  uint32 len:24;     // 256 align
  uint32 flag:8;  // del=1,zip=2
};

Index::Index()
 : bdb_db_(NULL)
 , bdb_db2_(NULL)
 , bdb_env_(NULL) {
}

bool Index::Close() {
  int res = 0;
  bool b = false;
  if (bdb_db_) {
    res = bdb_db_->close(bdb_db_, 0);
    if (res != 0)
      printf(">>error on close dbd_db_:%d\n",res);
    else
      b = true;
  }
  if (bdb_db2_) {
    res = bdb_db2_->close(bdb_db2_, 0);
    if (res != 0)
      printf(">>error on close bdb_db2_:%d\n",res);
    else
      b = true;
  }
  if (bdb_env_) {
    res = bdb_env_->close(bdb_env_, 0);
    if (res != 0)
      printf(">>error on close bdb_env_:%d\n",res);
    else
      b = true;
  }
  return b;
}
struct PageTable {
 // PageId pid;
  uint64 offset:32;
  uint64 len:24;
  uint64 valid_item:8;
};

struct ItemTable {
 // ItemId iid;
 uint64 flag:8;
 uint64 offset:36;
 uint64 len:20;
};

struct DBPage {
  char magic[4];
  PageId page_id; // 4 or 8 byte
  uint16 number;
  uint8 count_item;
  uint8 valid_item;
  uint32 flag:8; // 0 normal, 1 simple format
  uint32 count_byte:24;
  uint32 free_byte;
};

struct DBItem {
  // uint16 magic;
  ItemId item_id; // 4 or 8 byte
  uint32 flag:8; // 1 self del,2 manager del,4 zip
  uint32 page_offset:24;
  uint32 title_size:8;
  uint32 content_size:24;
  char title[0];
  char content[0];

};

// 返回偏移点,
// count返回总占用空间,包括对齐前的间隙
// item返回DBItem 的总长度,不包括对齐前的间隙
int64 CalcItem(int64 pos, const std::string &value, int *item, int *count) {
  int64 off = Align(pos, kItemAlign);
  if (count && item) {
    *item = sizeof(DBItem) + value.size();
    *count = *item + off - pos;
  }
  return off;
}

// DBItem 扩容过的
bool MakeItem(int64 pos, DBItem *di, ItemId iid, const char* value, int size) {
  if(!di || !value)
    return false;
  di->item_id = iid;
  di->flag = 0; // TODO: 先不考虑压缩及删除
  di->page_offset = pos;
  di->title_size = 0;
  di->content_size = size;
  memcpy(di->content, value, size);
  return true;
}

bool Index::SelectSpace(uint32 advice_page, PageTable *pt) {
  int64 startpos;
  if (!pt)
    return false;
  uint32 free_size;
  int64 free_offset;
  if(df_->GetFreeSize(advice_page, &free_size, &free_offset)) {
    if (free_size < advice_page + 256) {
      pt->offset = free_offset;
      pt->len = free_size;
      df_->RemoveFreeSize(free_size, free_offset);
      return true;
    }
  }
  pt->offset = Align(size_, kPageAlign);
  pt->len = Align(size_ + advice_page, kPageAlign) - pt->offset;
  size_ += pt->len;
  return true;
}

bool Index::UpdateIndex2(DBPage *olddbp, int64 pos) {
  
  int64 start = sizeof(DBPage);
  int count = olddbp->count_item;
  DBItem *dbi;
  for (int i = 0; i < count; i++) {
    start = Align(start, kItemAlign);
    dbi = (DBItem*)((char*)olddbp + start);
    ItemId iid = dbi->item_id;
    int len = sizeof(DBItem) + dbi->content_size;
    bool b = SetIndex2(iid, pos + start, len);
    if (b == false)
      return false;
    start += len;
  }
  return true;
}

// new page insert
bool Index::NewPage(PageId pid, ItemId iid,
                           PageTable *pt, int size,
                           char *oldbuf,
                           int oldsize, // old db page user space
                           const std::string &value) {
  PageTable *newpage;
  PageTable *lastpage;
  int advice_page; // 建议的页空间
  int advice_item; // 建议的项空间
  DBPage *olddbp = (DBPage*)oldbuf;
  int oldvaliditem;
  int oldcountitem;

  if (olddbp) {
    oldvaliditem = olddbp->valid_item;
    oldcountitem = olddbp->count_item;
  } else {
    oldvaliditem = 0;
    oldcountitem = 0;
  }

  if (oldvaliditem == kDbPageSize - 1)
    advice_item = 0;
  else if (oldcountitem)
    advice_item = oldsize/oldcountitem;
  else
    advice_item = 0; // 多少合适呢

  if (pt) {
    if (!oldbuf) {
      size++;
      newpage = (PageTable*)malloc(sizeof(PageTable) * (size));
      memcpy(newpage, pt, sizeof(PageTable) * (size-1));
    } else {
      newpage = (PageTable*)malloc(sizeof(PageTable) * (size));
      memcpy(newpage, pt, sizeof(PageTable) * size);
    }
    lastpage = &newpage[size-1];
  } else {
    newpage = (PageTable*)malloc(sizeof(PageTable));
    lastpage = newpage;
    size = 1;
  }
  if (!oldbuf) {
    oldsize = sizeof(DBPage);
  } 

  // 选择一个好的空间
  int count, item;
  int64 free_offset;
  uint32 free_size;
  int64 off0 = CalcItem(oldsize, value, &item, &count);
  advice_page = oldsize + count + advice_item; 
  SelectSpace(advice_page, lastpage);

  lastpage->valid_item = oldvaliditem + 1;

  char *buf = (char*)malloc(oldsize + count);
  DBPage *dbp = (DBPage*)buf;
  if (oldbuf) {
    memcpy(buf, oldbuf, oldsize);
  } else {
    memcpy(dbp->magic, kPageMagicCode, sizeof(kPageMagicCode) - 1);
    dbp->page_id = pid;
    dbp->number = size;
    dbp->flag = 0;
  }
  DBItem *dbitem = (DBItem*)(buf + off0);
  int64 off = lastpage->offset + off0;
  MakeItem(off0, dbitem, iid, value.c_str(), value.size());
  

  // fill db page head
  dbp->count_item = oldcountitem + 1;
  dbp->valid_item = oldvaliditem + 1;
  dbp->count_byte = lastpage->len;
  dbp->free_byte = lastpage->len - oldsize - count;
   
  bool b = df_->Write(lastpage->offset, oldsize + count, buf);
  if (!b)
    return false;

  // update new offset to index2
  if (oldbuf && bdb_db2_) {
    dbp->count_item -= 1; // UpdateIndex2 need old count
    b = UpdateIndex2(dbp, lastpage->offset);
    if (!b)
      return false;
  }

  lastpage->offset /= 256;
  lastpage->len = Align(lastpage->len , 256) /256;

  DBT key, data;
  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));
  key.data = &pid;
  key.size = sizeof(pid);
  data.data = newpage;
  data.size = sizeof(PageTable) * (size);
  int res = bdb_db_->put(bdb_db_, 0, &key, &data, 0); 
  if (res == DB_KEYEXIST) { // jianguile
    return false;
  }
  b = SetIndex2(iid, off, item);
  if (!b)
    return false;

  free(buf);
  free(newpage);
  return true;
}

int Index::GetPageSize(PageId pid) {
  PageTable* pt;
  int number = GetPageList(pid, &pt);
  free(pt);
  return number;
}

int Index::GetPageList(PageId pid, PageTable **values) {
  if (!values)
    return -1;

  DBT key, data;
  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));
  key.data = &pid;
  key.size = sizeof(pid);
  char buf[2048];
  PageTable *pt = (PageTable*)buf;
  data.data = buf;
  data.ulen = sizeof(buf);
  data.flags = DB_DBT_USERMEM;

  int64 off = 0;
  int res = bdb_db_->get(bdb_db_, 0, &key, &data, 0);
  if (res != 0) { 
    if (res == DB_NOTFOUND) {
      *values = NULL;
      return 0;
    }
    return -1;
   }

  *values = (PageTable*)malloc(data.size);
  if (memcpy(*values, data.data, data.size) == 0)
    return -1;
  int number = data.size / sizeof(PageTable);
  return number; 
}

int Index::GetPage(PageId pid, int idx, std::vector<std::pair<ItemId,std::string> > *values) {
  if(!values)
    return false;

  PageTable* arr;
  int number = GetPageList(pid, &arr);

  if (idx < 0)
    idx += number;

  if (idx >= number) {
    free(arr);
    return false;
  }
  PageTable *the = arr + idx;
  bool b =  GetPageData(the, values);
  free(arr);
  if (!b)
    return -1;
  return number;
}

bool Index::GetPageData(PageTable *pt, std::vector<std::pair<ItemId,std::string> > *values) {
  values->clear();
  char *buf = (char*)malloc(pt->len*256);
  int64 sz = df_->ReadSome(pt->offset*256, pt->len*256, buf);
  if (sz < sizeof(DBPage))
    return false;

  DBPage *dbp = (DBPage*)buf;
  int count = dbp->count_item;
  if ((dbp->flag & 1) == 1)
    return false;
  DBItem *dbi;
  int64 start = sizeof(DBPage);
  int64 alignpos;
  int32 pre;
  for (int i = 0; i < count && start < sz; i++) {
    alignpos = Align(start, kItemAlign);
    pre = alignpos - start;
    dbi = (DBItem*)(buf + alignpos);
    if ((dbi->flag & 3) == 0) // no deleted 
      values->push_back(std::make_pair(dbi->item_id,std::string(dbi->content, dbi->content_size))); 
    start += pre + sizeof(DBItem) + dbi->content_size;
  }
  return true;
}

int Index::Get(PageId pid, ItemId iid, std::string *value) {
  if (!value)
    return false;
  DBT key, data;
  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));

  if (!bdb_db2_) {
    if (pid == 0)
      return -1;
    // 枚举所有此页的所有pid
    // 暂时不实现
    return 0;
  } else {
    key.data = &iid;
    key.size = sizeof(iid);
    ItemTable it;
    data.data = &it;
    data.size = sizeof(it);
    data.ulen = sizeof(it);
    data.flags = DB_DBT_USERMEM;
    int res = bdb_db2_->get(bdb_db2_, NULL, &key, &data, 0);
    if (res != 0) { 
      if (res == DB_NOTFOUND) {
        return 256;
      }
      return -1;
    }
    if ((it.flag & 3) > 0) // deleted
      return false;
    char *buf = (char*)malloc(it.len*16);
    int64 sz = df_->ReadSome(it.offset*16, it.len*kItemAlign, buf);
    if (sz < sizeof(DBItem)) {
      free(buf);
      return -1;
    }
    DBItem *dbi = (DBItem*)buf;
    value->assign(dbi->content, dbi->content_size);
    free(buf);
    return dbi->flag;
  }
  
  return -1;
}

bool Index::SetIndex2(ItemId iid, int64 offset, int32 len) {
  if (!bdb_db2_)
    return true;

  DBT key, data;
  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));
  key.data = &iid;
  key.size = sizeof(iid);

  int32 k = Align(len, 16) / 16;
  ItemTable it ={0,
                offset/16,
                k};
  data.data = &it;
  data.size = sizeof(it);
  int res = bdb_db2_->put(bdb_db2_, 0, &key, &data, 0);
  if (res != 0)
    return false;

  return true;
}

int Index::Add(PageId pid, ItemId iid, const std::string &value) {
  // 1 pid => PageTable
  DBT key, data;
  memset(&key, 0, sizeof(DBT));
  memset(&data, 0, sizeof(DBT));
  key.data = &pid;
  key.size = sizeof(pid);
  char buf[2048];
  PageTable *pt = (PageTable*)buf;
  data.data = buf;
  data.ulen = sizeof(buf);
  data.flags = DB_DBT_USERMEM;
  int number = -1;

  int64 off = 0;
  //int flag = DB_FAST_STAT;
  //int test = bdb_db_->stat(bdb_db_, 0, 0, flag);
  //DB *pp = bdb_db_->get();
  int res = bdb_db_->get(bdb_db_, 0, &key, &data, 0);
  if (res != 0) {
    if (res == DB_NOTFOUND) {
      // new page 
      bool b = NewPage(pid, iid,
                           NULL, 0,
                           NULL, 0,
                           value);
      if (!b)
        return -1;
      number = 0;
    } else {
      return -1;
    }
  } else {
    PageTable *arr = (PageTable*)data.data;
    int pagecount = data.size / sizeof(PageTable);
    PageTable *last = arr + pagecount - 1;
    if (last->valid_item < kDbPageSize) {
      last->valid_item++;
      char *buf = (char*)malloc(last->len*256);
      int64 sz = df_->ReadSome(last->offset*256, last->len*256, buf);
      if (sz < sizeof(DBPage))
        return -1;
      DBPage *dbp = (DBPage*)buf;
      int64 pos = last->offset*256 + dbp->count_byte - dbp->free_byte;

      int count, item;
      off = CalcItem(pos, value, &item, &count);
      if (count > dbp->free_byte) { // 空间不足
        // new pagetable
        bool b = NewPage(pid, iid,
                           arr, pagecount,
                           buf,
                           dbp->count_byte - dbp->free_byte, // old db page user page
                           value);

        // free old pagetable
        dbp->flag |= 1; // delete
        df_->Write(last->offset*256, sizeof(DBPage), buf);
        df_->PushFreeSize(dbp->count_byte, last->offset*256);

      } else {
        DBItem * dbitem = (DBItem*)(buf + off - last->offset*256);
        MakeItem(off - last->offset*256, dbitem, iid, value.c_str(), value.size());
        dbp->free_byte -= count;
        dbp->count_item ++;
        dbp->valid_item ++;
        df_->Write(last->offset*256, dbp->count_byte - dbp->free_byte, buf);

        last->len = Align(dbp->count_byte - dbp->free_byte , 256) / 256;
        res = bdb_db_->put(bdb_db_, 0, &key, &data, 0);
        bool b = SetIndex2(iid, off, item); 
      }
      number = pagecount - 1;
      free(buf);
    } else {
      // new pagetable
      bool b = NewPage(pid, iid,
                           arr, pagecount,
                           NULL, 0,
                           value);
      number = pagecount;
    }
  }
  return number;
}

bool Index::Init(DocumentFile *df, const char *path, const char *file, const char *file2) {
  // main index
  int ret;
  df_ = df;

  // env
  DB_ENV *bdb_env;
  ret = db_env_create(&bdb_env, 0);
  if (ret != 0) {
    return false;
  }

  // DB_CREATE | DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN | DB_THREAD
  int env_flags = DB_CREATE | DB_INIT_MPOOL;
  ret = bdb_env->open(bdb_env, path, env_flags, 0);
  if (ret != 0) {
    return false;
  }
  bdb_env_ = bdb_env;

  // bdb_db_
  DB *bdb_db;
  if ((ret = db_create(&bdb_db, bdb_env, 0)) != 0) {
    Close();
    return false;
  }
  if ((ret = bdb_db->open(bdb_db, NULL, file, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
    Close();
    return false;
  }
  bdb_db_ = bdb_db;

  // bdb_db2_
  DB *bdb_db2;
  if (file2[0]) {
    if ((ret = db_create(&bdb_db2, bdb_env, 0)) != 0) {
      Close();
      return false;
    }
    if ((ret = bdb_db2->open(bdb_db2, NULL, file2, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
      Close();
      return false;
    }
    bdb_db2_ = bdb_db2;
  }
  size_ = df_->size();
  return true;
}

void Index::RemovePage(PageId pid) {

}

void Index::RemoveItem(ItemId iid) {


}

};
