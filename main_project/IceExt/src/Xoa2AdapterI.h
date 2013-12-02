#ifndef __XOA2ADAPTER_I_H__
#define __XOA2ADAPTER_I_H__

#include "Singleton.h"
#include "xoa2/client/client_pool.h"
#include "xoa2/client/xoa_client_factory.h"


namespace MyUtil {
template<class T>
class Xoa2AdapterI {
private:
  boost::shared_ptr< xoa2::client::ClientPool<T> > pool_;

protected:
  class NullClientException : public std::exception {
  public:
    virtual const char* what() const throw() {
      return "ClientPool Alloc NULL Client";
    }
  };


  class ClientHandler {
    T *client_;
    bool call_result_;
    boost::shared_ptr< xoa2::client::ClientPool<T> > pool_;

  public:
    T *client() {return client_;}
    void set_call_result(bool result) { call_result_ = result; }
    ClientHandler(int shard, boost::shared_ptr< xoa2::client::ClientPool<T> > &pool)
      : client_(NULL), call_result_(true), pool_(pool) 
    {
      client_ = pool_->Alloc(shard);
    }


    ~ClientHandler() {
      if (client_ != NULL) {
        pool_->Release(client_, call_result_);
        client_ = NULL;
      }
    }

  };

public:
	Xoa2AdapterI(const char *service_id, int timeout) :
  pool_(new xoa2::client::ClientPool<T>(service_id, timeout))
	{
	}

	virtual ~Xoa2AdapterI()
  {
	}


	boost::shared_ptr< ClientHandler > getClient(int mod) {
		return boost::shared_ptr< ClientHandler >(new ClientHandler(mod, pool_));
	}



};

}
;

#define __XOA2_RPC__(RETURN, CLIENT, FUN, SHARD, ARGS)         \
  boost::shared_ptr<ClientHandler> hclient = getClient(SHARD); \
  CLIENT *client  = hclient->client();                         \
                                                               \
  if (client != NULL) {                                        \
    try {                                                      \
      RETURN client->FUN ARGS;                                 \
    } catch (...) {                                            \
      hclient->set_call_result(false);                         \
      throw;                                                   \
    }                                                          \
                                                               \
  } else {                                                     \
    throw NullClientException();                               \
  }





#define XOA2_RPC_CALL(CLIENT, FUN, SHARD, ARGS)                \
  __XOA2_RPC__(;, CLIENT, FUN, SHARD, ARGS)


#define XOA2_RPC_RETURN_CALL(CLIENT, FUN, SHARD, ARGS)         \
  __XOA2_RPC__(return, CLIENT, FUN, SHARD, ARGS)

#endif
