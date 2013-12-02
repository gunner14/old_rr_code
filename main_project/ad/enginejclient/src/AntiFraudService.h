/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef AntiFraudService_H
#define AntiFraudService_H

#include <TProcessor.h>
#include "engine_types.h"

namespace com { namespace renren { namespace ad { namespace engine {

class AntiFraudServiceIf {
 public:
  virtual ~AntiFraudServiceIf() {}
  virtual int32_t check(const ClickAction& clickAction) = 0;
};

class AntiFraudServiceNull : virtual public AntiFraudServiceIf {
 public:
  virtual ~AntiFraudServiceNull() {}
  int32_t check(const ClickAction& /* clickAction */) {
    int32_t _return = 0;
    return _return;
  }
};

typedef struct _AntiFraudService_check_args__isset {
  _AntiFraudService_check_args__isset() : clickAction(false) {}
  bool clickAction;
} _AntiFraudService_check_args__isset;

class AntiFraudService_check_args {
 public:

  AntiFraudService_check_args() {
  }

  virtual ~AntiFraudService_check_args() throw() {}

  ClickAction clickAction;

  _AntiFraudService_check_args__isset __isset;

  bool operator == (const AntiFraudService_check_args & rhs) const
  {
    if (!(clickAction == rhs.clickAction))
      return false;
    return true;
  }
  bool operator != (const AntiFraudService_check_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AntiFraudService_check_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class AntiFraudService_check_pargs {
 public:


  virtual ~AntiFraudService_check_pargs() throw() {}

  const ClickAction* clickAction;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AntiFraudService_check_result__isset {
  _AntiFraudService_check_result__isset() : success(false) {}
  bool success;
} _AntiFraudService_check_result__isset;

class AntiFraudService_check_result {
 public:

  AntiFraudService_check_result() : success(0) {
  }

  virtual ~AntiFraudService_check_result() throw() {}

  int32_t success;

  _AntiFraudService_check_result__isset __isset;

  bool operator == (const AntiFraudService_check_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const AntiFraudService_check_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AntiFraudService_check_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AntiFraudService_check_presult__isset {
  _AntiFraudService_check_presult__isset() : success(false) {}
  bool success;
} _AntiFraudService_check_presult__isset;

class AntiFraudService_check_presult {
 public:


  virtual ~AntiFraudService_check_presult() throw() {}

  int32_t* success;

  _AntiFraudService_check_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class AntiFraudServiceClient : virtual public AntiFraudServiceIf {
 public:
  AntiFraudServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  AntiFraudServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int32_t check(const ClickAction& clickAction);
  void send_check(const ClickAction& clickAction);
  int32_t recv_check();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AntiFraudServiceProcessor : virtual public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<AntiFraudServiceIf> iface_;
  virtual bool process_fn(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid);
 private:
  std::map<std::string, void (AntiFraudServiceProcessor::*)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*)> processMap_;
  void process_check(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
 public:
  AntiFraudServiceProcessor(boost::shared_ptr<AntiFraudServiceIf> iface) :
    iface_(iface) {
    processMap_["check"] = &AntiFraudServiceProcessor::process_check;
  }

  virtual bool process(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot);
  virtual ~AntiFraudServiceProcessor() {}
};

class AntiFraudServiceMultiface : virtual public AntiFraudServiceIf {
 public:
  AntiFraudServiceMultiface(std::vector<boost::shared_ptr<AntiFraudServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AntiFraudServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AntiFraudServiceIf> > ifaces_;
  AntiFraudServiceMultiface() {}
  void add(boost::shared_ptr<AntiFraudServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int32_t check(const ClickAction& clickAction) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->check(clickAction);
      } else {
        ifaces_[i]->check(clickAction);
      }
    }
  }

};

}}}} // namespace

#endif