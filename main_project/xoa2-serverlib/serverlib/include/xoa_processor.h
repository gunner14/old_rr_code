/**
 * 用于处理的processor
 * 它能够对调用的函数进行统计（主要统计调用次数和每次的调用时间），便于以后的分析。 
 * @author zhe.long
 * @email zhe.long@renren-inc.com
 * @date 2012.11.23
 */


#ifndef _XOA_PROCESSOR_H_
#define _XOA_PROCESSOR_H_

#include <boost/shared_ptr.hpp>
#include <thrift/transport/TTransport.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/TProcessor.h>

#include <time.h>

#include "concurrency/Mutex.h"
#include "service_base_impl.h"

using namespace boost;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace xoa2::base;
using apache::thrift::concurrency::Guard;
using apache::thrift::concurrency::Mutex;
namespace xoa2 { namespace processor {

template <class HandlerType>
class XoaProcessor : public apache::thrift::TProcessor {
public:
  XoaProcessor(boost::shared_ptr<TProcessor> processor)
    : real_processor_(processor), has_base_(false)
  {}
  virtual ~XoaProcessor() {}

  void setHandler(boost::shared_ptr<HandlerType> handler) {
    handler_ = handler;
  }
  void setHasBase(bool has_base) {
    has_base_ = has_base;
  }
  virtual bool process(boost::shared_ptr<apache::thrift::protocol::TProtocol> piprot,
                       boost::shared_ptr<apache::thrift::protocol::TProtocol> poprot,
                       void* serverContext) {
    
    boost::shared_ptr<apache::thrift::protocol::TProtocol> buf_protocol;
    
    std::string fname;
    apache::thrift::protocol::TMessageType mtype;
    int32_t seqid;
 
    apache::thrift::protocol::TType ftype = apache::thrift::protocol::T_STRUCT;
    {
      Guard g(service_lock_);
      buf_protocol = createBufferProtocol();

      piprot_ = piprot;
    
      piprot_->readMessageBegin(fname, mtype, seqid);
      if (mtype != apache::thrift::protocol::T_CALL) {
        MCE_ERROR("Unknown message type\n");
        throw apache::thrift::TException("Unexpected message type");
      }
     
      buf_protocol->writeMessageBegin(fname, mtype, seqid);
      printAndPassToBuffer(buf_protocol,ftype);
      //buf_protocol->writeMessageEnd();
    }

    struct timeval start_time, end_time;
    int64_t elapsed_time;
    gettimeofday(&start_time, NULL);
    bool ret = real_processor_->process(buf_protocol, poprot, serverContext);
    gettimeofday(&end_time, NULL);
    cout<<end_time.tv_sec-start_time.tv_sec<<"==========="<<endl;
    elapsed_time = 1000000*(end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec);
   
    if(has_base_) {
      dynamic_pointer_cast<ServiceBaseImpl>(handler_)->setCallMethodInfo(fname, elapsed_time/1000);
    }
    return true;
  }

  const std::map<std::string, int64_t>& get_frequency_map() {
    return frequency_map_;
  }

protected:
  void printAndPassToBuffer(boost::shared_ptr<apache::thrift::protocol::TProtocol> buf_protocol,apache::thrift::protocol::TType ftype) {
    switch (ftype) {
      case apache::thrift::protocol::T_BOOL:
        {
          bool boolv;
          piprot_->readBool(boolv);
          buf_protocol->writeBool(boolv);
        }
        break;
      case apache::thrift::protocol::T_BYTE:
        {
          int8_t bytev;
          piprot_->readByte(bytev);
          buf_protocol->writeByte(bytev);
        }
        break;
      case apache::thrift::protocol::T_I16:
        {
          int16_t i16;
          piprot_->readI16(i16);
          buf_protocol->writeI16(i16);
        }
        break;
      case apache::thrift::protocol::T_I32:
        {
          int32_t i32;
          piprot_->readI32(i32);
          buf_protocol->writeI32(i32);
        }
        break;
      case apache::thrift::protocol::T_I64:
        {
          int64_t i64;
          piprot_->readI64(i64);
          buf_protocol->writeI64(i64);
        }
        break;
      case apache::thrift::protocol::T_DOUBLE:
        {
          double dub;
          piprot_->readDouble(dub);
          buf_protocol->writeDouble(dub);
        }
        break;
      case apache::thrift::protocol::T_STRING:
        {
          std::string str;
          piprot_->readString(str);
          buf_protocol->writeString(str);
        }
        break;
      case apache::thrift::protocol::T_STRUCT:
        {
          std::string name;
          int16_t fid;
          apache::thrift::protocol::TType ftype;
          piprot_->readStructBegin(name);
          buf_protocol->writeStructBegin(name.c_str());
          while (true) {
            piprot_->readFieldBegin(name, ftype, fid);
            if (ftype == apache::thrift::protocol::T_STOP) {
              buf_protocol->writeFieldStop();
              break;
            }
            buf_protocol->writeFieldBegin(name.c_str(), ftype, fid);
            printAndPassToBuffer(buf_protocol,ftype);
            piprot_->readFieldEnd();
            buf_protocol->writeFieldEnd();
          }
          piprot_->readStructEnd();
          buf_protocol->writeStructEnd();
        }
        break;
      case apache::thrift::protocol::T_MAP:
        {
          apache::thrift::protocol::TType keyType;
          apache::thrift::protocol::TType valType;
          uint32_t i, size;
          piprot_->readMapBegin(keyType, valType, size);
          buf_protocol->writeMapBegin(keyType, valType, size);
          for (i = 0; i < size; i++) {
            printAndPassToBuffer(buf_protocol,keyType);
            printAndPassToBuffer(buf_protocol,valType);
          }
          piprot_->readMapEnd();
          buf_protocol->writeMapEnd();
        }
        break;
      case apache::thrift::protocol::T_SET:
        {
          apache::thrift::protocol::TType elemType;
          uint32_t i, size;
          piprot_->readSetBegin(elemType, size);
          buf_protocol->writeSetBegin(elemType, size);
          for (i = 0; i < size; i++) {
            printAndPassToBuffer(buf_protocol,elemType);
          }
          piprot_->readSetEnd();
          buf_protocol->writeSetEnd();
        }
        break;
      case apache::thrift::protocol::T_LIST:
        {
          apache::thrift::protocol::TType elemType;
          uint32_t i, size;
          piprot_->readListBegin(elemType, size);
          buf_protocol->writeListBegin(elemType, size);
          for (i = 0; i < size; i++) {
            printAndPassToBuffer(buf_protocol,elemType);
          }
          piprot_->readListEnd();
          buf_protocol->writeListEnd();
        }
        break;
      default:
        break;
    }
  }

  boost::shared_ptr<apache::thrift::protocol::TProtocol> piprot_;
  std::map<std::string, int64_t> frequency_map_;

  boost::shared_ptr<HandlerType> handler_;
  boost::shared_ptr<TProcessor> real_processor_; 
  Mutex service_lock_; 
  bool has_base_;
private:
  boost::shared_ptr<apache::thrift::protocol::TProtocol> createBufferProtocol() {
    boost::shared_ptr<transport::TTransport> trans = boost::shared_ptr<transport::TTransport>(new TMemoryBuffer(256));
    boost::shared_ptr<protocol::TProtocol> buf_protocol(new TBinaryProtocol(trans));
    return buf_protocol; 
  }
};

}} // apache::thrift::processor

#endif
