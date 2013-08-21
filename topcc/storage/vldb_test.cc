#include "vldb.h"

#include <algorithm>
#include <iostream>
#include <gtest/gtest.h>

TEST(Test, primary) {
  arch::PageTable *pt;
  arch::DocumentFile df;
  unlink("./17.dat");
  unlink("./17.idx");
  unlink("./17.id2");
  unlink("./__db.001");
  unlink("./__db.002");
  unlink("./__db.003");

  int res = df.Open(17, ".", false);
  ASSERT_EQ(0, res);
  arch::Index *idx = df.index();

  int numb = idx->GetPageList(200435454, &pt);
  numb = idx->GetPageSize(200435454);
  ASSERT_EQ(numb, 0);

  numb = idx->Add(200435454, 101, "this is a test");
  ASSERT_EQ(numb, 0);
   
  numb = idx->GetPageList(200435454, &pt);
  ASSERT_EQ(numb, 1);
  numb = idx->GetPageSize(200435454);
  ASSERT_EQ(numb, 1);

  std::vector<std::pair<arch::ItemId,std::string> > values;
  numb = idx->GetPage(200435454, 0, &values);
  ASSERT_EQ(1, numb);
  ASSERT_EQ(1, values.size());
  ASSERT_EQ(101, values[0].first);
  ASSERT_EQ("this is a test", values[0].second);

  std::string value;
  int flag =  idx->Get(200435454, 101, &value);
  ASSERT_EQ(flag, 0);

  df.Close();
}

TEST(Test, many_bdb) {
  arch::PageTable *pt;
  arch::DocumentFile df;

  bool b = true;
  for(int i = 0; i < 100; i++) {
    int res = df.Open(18, ".", true);
    arch::Index *idx = df.index();
    std::string value;
    int flag =  idx->Get(200435454, 101, &value);
    b = df.Close();
    if (!b)
      break;
  }
  ASSERT_TRUE(b);
}
TEST(Test, sec_index) {
  arch::PageTable *pt;
  arch::DocumentFile df;
  unlink("./19.dat");
  unlink("./19.idx");
  unlink("./19.id2");
  unlink("./__db.001");
  unlink("./__db.002");
  unlink("./__db.003");

  int res = df.Open(19, ".", true);
  ASSERT_EQ(0, res); // new file
  arch::Index *idx = df.index();
  ASSERT_TRUE(idx != NULL);

  int numb = idx->Add(200435454, 101, "this is a test");
   
  std::string value;
  int flag =  idx->Get(200435454, 101, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "this is a test");

  df.Close();
}

TEST(Test, multi) {
  arch::PageTable *pt;
  arch::DocumentFile df;
  unlink("./20.dat");
  unlink("./20.idx");
  unlink("./20.id2");
  unlink("./__db.001");
  unlink("./__db.002");
  unlink("./__db.003");
  
  int numb;
  int res = df.Open(20, ".", true);
  arch::Index *idx = df.index();

  numb = idx->Add(200435454, 101, "this is a test");
  ASSERT_EQ(numb, 0);
  numb = idx->Add(200435454, 102, "that is a test");
  ASSERT_EQ(numb, 0);
   
  numb = idx->GetPageList(200435454, &pt);
  ASSERT_EQ(numb, 1);
  numb = idx->GetPageSize(200435454);
  ASSERT_EQ(numb, 1);

  std::vector<std::pair<arch::ItemId,std::string> > values;
  bool b = idx->GetPage(200435454, 0, &values);
  ASSERT_TRUE(b);
  ASSERT_EQ(2, values.size());

  ASSERT_EQ(101, values[0].first);
  ASSERT_EQ("this is a test", values[0].second);
  ASSERT_EQ(102, values[1].first);
  ASSERT_EQ("that is a test", values[1].second);

  std::string value;
  int flag =  idx->Get(200435454, 101, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "this is a test");

  flag =  idx->Get(200435454, 102, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "that is a test");

  df.Close();
}

TEST(Test, movepage) {
  arch::PageTable *pt;
  arch::DocumentFile df;
  unlink("./21.dat");
  unlink("./21.idx");
  unlink("./21.id2");
  unlink("./__db.001");
  unlink("./__db.002");
  unlink("./__db.003");
 
  bool b; 
  int numb;
  int res = df.Open(21, ".", true);
  arch::Index *idx = df.index();

  // move page
  //  pagetable pad    item:101          item:102        item:103                item:104                item:105
  // |---20---|--12--|-----30----|-2-|-----30-----|-2-|-------60-------|-4-|----------90---------|--6--|-----30----|
  // 0        20     32          62  64           94  96            156   160                   250   256         286
  numb = idx->Add(200435454, 101, "this is a test");
  numb = idx->Add(200435454, 102, "that is a test");
  numb = idx->Add(200435454, 103, "123456789_123456789_123456789_123456789_1234");
  numb = idx->Add(200435454, 104, "123456789_123456789_123456789_123456789_123456789_123456789_123456789_1234");
  ASSERT_EQ(numb, 0);
  numb = idx->Add(200435454, 105, "they is a test"); // move page
  ASSERT_EQ(numb, 0);

  std::vector<std::pair<arch::ItemId,std::string> > values;
  numb = idx->GetPage(200435454, 0, &values);
  ASSERT_EQ(1, numb);
  ASSERT_EQ(5, values.size());

  ASSERT_EQ(101, values[0].first);
  ASSERT_EQ("this is a test", values[0].second);
  ASSERT_EQ(102, values[1].first);
  ASSERT_EQ("that is a test", values[1].second);

  std::string value;
  int flag =  idx->Get(200435454, 101, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "this is a test");

  flag =  idx->Get(200435454, 102, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "that is a test");

  flag =  idx->Get(200435454, 105, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "they is a test");

  // write 31 item 
  //  pagetable pad    item:101        item:102       item:103       item:104         item:105         item:130      item:131
  // |---20---|--12--|----30----|-2-|----30----|-2-|----30----|-2-|----30----|--2--|----30----|.....|----30----|-2-|----30----|--
  // 0        20     32          62  64         94  96        126 128        158   160        190   960       992            1024

  for (int i = 0; i < 30; i++) {
    numb = idx->Add(68464, 201 + i, "fork is a test");
  }
  ASSERT_EQ(0, numb);

  // split page
  numb = idx->Add(68464, 231, "fork is a test");
  ASSERT_EQ(1, numb);

  numb = idx->GetPage(68464, 0, &values);
  ASSERT_EQ(2,numb);
  ASSERT_EQ(30, values.size());

  ASSERT_EQ(201, values[0].first);
  ASSERT_EQ("fork is a test", values[0].second);
  ASSERT_EQ(230, values[29].first);
  ASSERT_EQ("fork is a test", values[1].second);

  flag = idx->Get(68464, 201, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "fork is a test");

  flag =  idx->Get(68464, 230, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "fork is a test");

  numb = idx->GetPage(68464, 1, &values);
  ASSERT_EQ(2, numb);
  ASSERT_EQ(1, values.size());

  ASSERT_EQ(231, values[0].first);
  ASSERT_EQ("fork is a test", values[0].second);

  flag = idx->Get(68464, 231, &value);
  ASSERT_EQ(flag, 0);
  ASSERT_EQ(value, "fork is a test");

  df.Close();
}
