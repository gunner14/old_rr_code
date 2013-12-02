#ifndef __XCE_ADAPTERTEST_TRIPODSIMPLEADAPTERTEST__
#define __XCE_ADAPTERTEST_TRIPODSIMPLEADAPTERTEST__

#include <gtest/gtest.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <QueryRunner.h>
#include <cctype> //toupper
#include <algorithm> //transform
#include <tripod_options.pb.h> //公共属性

using namespace std;
using namespace com::xiaonei::xce;

using google::protobuf::Message;
using google::protobuf::FieldDescriptor;
using google::protobuf::Descriptor;
using google::protobuf::Reflection;

using testing::Types;

//gtest的模板测试只支持一个模板参数，而测试需要用到多个
//所以把多个模板参数合到一个模板中传入
template<class T1, class T2>
struct TS{
  typedef T1 P1;
  typedef T2 P2; 
};

//传入的参数T需是TS类型的
template <class T>
class TripodSimpleAdapterTest : public testing::Test {
  typedef typename T::P1 TAdapter;
  typedef typename T::P2 TProto;

public:
  static void SetUpTestCase() {
    MCE_WARN("[TripodSimpleAdapterTest] init");
    ConnectionPoolManager::instance().initialize();
  }

public:
  TripodSimpleAdapterTest() {
    _message = (Message*)new TProto;
    _reflection = _message->GetReflection();
    _descriptor = _message->GetDescriptor();

    //从Proto中获取参数
    _business_id = _descriptor->options().GetExtension(::tripod::options::business_id);
    _database_source = _descriptor->options().GetExtension(tripod::options::database_source);
    _table_name = _descriptor->options().GetExtension(tripod::options::table_name);
    _primary_key = _descriptor->options().GetExtension(tripod::options::primary_key);
    transform(_primary_key.begin(), _primary_key.end(), _primary_key.begin(), (int(*)(int))toupper);
    MCE_INFO("[TripodSimpleAdapterTest] Option business_id = " << _business_id);

    //测试需要在测试环境部署Tripod服务
    _adapter = new TAdapter("xcetest3:2181/Tripod", "test", _business_id);

    //测试id
    _id = 35178;
  }

  virtual ~TripodSimpleAdapterTest() { delete _adapter; }

  void cleanData() {
    MyUtil::Str2StrMap props;
    props[_primary_key] = boost::lexical_cast<string>(_id);
    Statement sql = Statement::buildDelete(_table_name, props);
    QueryRunner(_database_source, CDbWServer).execute(sql);
    _adapter->remove(_id);
  }

  int id() { return _id; }

  //从Proto生成参照数据
  void genDataAFromProto(int deltaInt = 50, string deltaStr = "Hi", bool deltaBool = true) {
    for(int i=0; i < _descriptor->field_count(); ++i) {
      const FieldDescriptor* field_descriptor = _descriptor->field(i);
      assert(field_descriptor->label() != FieldDescriptor::LABEL_REPEATED);
      string name = field_descriptor->name();
      transform(name.begin(), name.end(), name.begin(), (int(*)(int))toupper);
      switch(field_descriptor->type()){
        case FieldDescriptor::TYPE_INT64 :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt);break;
        case FieldDescriptor::TYPE_FIXED64 :
        case FieldDescriptor::TYPE_UINT64 :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt);break;
        case FieldDescriptor::TYPE_INT32 :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt);break;
        case FieldDescriptor::TYPE_FIXED32:
        case FieldDescriptor::TYPE_UINT32 :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt);break;
        case FieldDescriptor::TYPE_STRING :
          //_dataMapA[name] = boost::lexical_cast<string>(name + " " + deltaStr);break;
          _dataMapA[name] = boost::lexical_cast<string>(deltaStr);break;
        case FieldDescriptor::TYPE_DOUBLE :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt + 3.14159265);break;
        case FieldDescriptor::TYPE_FLOAT :
          _dataMapA[name] = boost::lexical_cast<string>(i + deltaInt + 3.14159);break;
        case FieldDescriptor::TYPE_BOOL :
          _dataMapA[name] = boost::lexical_cast<string>(true && deltaBool);break;
        default:
          break;
      }
      MCE_INFO("[TripodSimpleAdapterTest] dataMapA[" << name << "] = " << _dataMapA[name]);
    }
  }

  //从Cache获取数据生成对比数据
  void genDataBFromCache() {
    TProto message = this->_adapter->get(this->_id);
    for(int i=0; i < _descriptor->field_count(); ++i) {
      const FieldDescriptor* field_descriptor = _descriptor->field(i);
      assert(field_descriptor->label() != FieldDescriptor::LABEL_REPEATED);
      string name = field_descriptor->name();
      transform(name.begin(), name.end(), name.begin(), (int(*)(int))toupper);
      switch(field_descriptor->type()){
        case FieldDescriptor::TYPE_INT64 : 
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetInt64(message,field_descriptor));break;
        case FieldDescriptor::TYPE_FIXED64 : 
        case FieldDescriptor::TYPE_UINT64 : 
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetUInt64(message,field_descriptor));break;
        case FieldDescriptor::TYPE_INT32 :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetInt32(message,field_descriptor));break;
        case FieldDescriptor::TYPE_FIXED32: 
        case FieldDescriptor::TYPE_UINT32 :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetUInt32(message,field_descriptor));break;
        case FieldDescriptor::TYPE_STRING :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetString(message,field_descriptor));break;
        case FieldDescriptor::TYPE_DOUBLE :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetDouble(message,field_descriptor));break;
        case FieldDescriptor::TYPE_FLOAT :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetFloat(message,field_descriptor));break;
        case FieldDescriptor::TYPE_BOOL :
          _dataMapB[name] = boost::lexical_cast<string>(_reflection->GetBool(message,field_descriptor));break;
        default:
          break;
      }
    }
  }

  MyUtil::Str2StrMap DataA() { return _dataMapA; }

  MyUtil::Str2StrMap DataB() { return _dataMapB; }

  void createData() {
    MCE_INFO("[TripodSimpleAdapterTest] createData");
    _dataMapA[_primary_key] = boost::lexical_cast<string>(this->id());
    _adapter->create(_dataMapA);
  }

  void setData() {
    MCE_INFO("[TripodSimpleAdapterTest] setData");
    _adapter->set(_id, _dataMapA);
  }

  vector<string> getIndex() {
    vector<string> result;
    for(int i=0; i < _descriptor->field_count(); ++i) {
      const FieldDescriptor* field_descriptor = _descriptor->field(i);
      assert(field_descriptor->label() != FieldDescriptor::LABEL_REPEATED);
      string name = field_descriptor->name();
      transform(name.begin(), name.end(), name.begin(), (int(*)(int))toupper);
      if(name != _primary_key) {
        result.push_back((name));
      }
    }
    return result;
  }

private:
  int _id;
  string _business_id;
  string _table_name;
  string _database_source;
  string _primary_key;

  TAdapter* _adapter;
  Message* _message;
  const Reflection* _reflection;
  const Descriptor* _descriptor;

  MyUtil::Str2StrMap _dataMapA; //理想数据
  MyUtil::Str2StrMap _dataMapB; //实际数据

};

TYPED_TEST_CASE_P(TripodSimpleAdapterTest);

//测试Adapter的create方法
TYPED_TEST_P(TripodSimpleAdapterTest, Create) {
  //步骤1：删除要测试ID在数据库中和缓存中的记录
  this->cleanData();

  //步骤2：根据Proto生成测试数据，并调用create方法增加数据
  this->genDataAFromProto();
  this->createData();

  //步骤3：调用get得到数据，并解析到缓存中，和理想值对比
  this->genDataBFromCache();
  vector<string> index = this->getIndex();
  MyUtil::Str2StrMap dataAMap = this->DataA();
  MyUtil::Str2StrMap dataBMap = this->DataB();
  for(vector<string>::iterator it = index.begin(); it != index.end(); ++it) {
    string strA = dataAMap[(*it)];
    string strB = dataBMap[(*it)];
    EXPECT_STREQ(strA.c_str(), strB.c_str());
  }
}

//测试Adapter的set方法，需要在Create后执行
TYPED_TEST_P(TripodSimpleAdapterTest, Set) {
  //步骤1：生成不同的数据，调用set
  this->genDataAFromProto(80, "OK", false);
  this->setData();

  //步骤2：调用get得到数据，并解析到缓存中，和理想值对比
  this->genDataBFromCache();
  vector<string> index = this->getIndex();
  MyUtil::Str2StrMap dataAMap = this->DataA();
  MyUtil::Str2StrMap dataBMap = this->DataB();
  for(vector<string>::iterator it = index.begin(); it != index.end(); ++it) {
    string strA = dataAMap[(*it)];
    string strB = dataBMap[(*it)];
    EXPECT_STREQ(strA.c_str(), strB.c_str());
  } 
}

REGISTER_TYPED_TEST_CASE_P(TripodSimpleAdapterTest, Create, Set);

#endif
