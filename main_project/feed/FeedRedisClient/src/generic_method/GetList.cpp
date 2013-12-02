template <class T> 
bool RedisClient::GetList(const std::string& key, std::vector<T>& ret_vec, int begin, int limit) {
  std::ostringstream oss_log;
  oss_log << "RedisClient::GetList --> address_:" << address_ << " port_:" << port_ << " key:" << key << " begin:" << begin << " limit:" << limit; 
  MCE_INFO(oss_log.str());                                              
#ifdef STDOUT 
  std::cout << oss_log.str() << std::endl;
#endif

  if(limit < 1) {
    MCE_WARN("RedisClient::GetList --> address_:" << address_ << " port_:" << port_ << " key:" << key << " begin:" << begin << " limit:" << limit);
    return false; 
  }

  int start = begin;
  const int stop = begin + limit - 1;

  while(start <= stop) {
    redisReply* reply = NULL;


    std::ostringstream oss_cmd;
    oss_cmd << "LRANGE " << key << " ";    

    int batch_end = start + kReadBatchSize - 1;
    batch_end = batch_end < stop ? batch_end : stop;
    oss_cmd << start << " " << batch_end;

    std::string command = oss_cmd.str();   


    {
      TimeLog tl(oss_log.str());
      reply = (redisReply*) redisCommand(rc_, command.c_str());
    }

    if(CheckErr(reply, rc_, oss_log.str())) {
      SetValid(false);
      if(reply) {
        freeReplyObject(reply);
      }
      return false;
    }


    size_t fetch_size = reply->elements;
    for(size_t i = 0; i < fetch_size; i++) {
      T item;
      memmove(&item, reply->element[i]->str, reply->element[i]->len);
      ret_vec.push_back(item);
    }

    std::ostringstream loop_log;
    loop_log << "RedisClient::GetList --> address_:" << address_ << " port_:" << port_ << " key:" << key << " start:" << start << " batch_end:" << batch_end << " fetch size:" << fetch_size; 
    MCE_INFO(loop_log.str());                                              
#ifdef STDOUT 
    std::cout << loop_log.str() << std::endl;
#endif

    start += fetch_size;


    freeReplyObject(reply);

    if(fetch_size < kReadBatchSize) {
      break;
    }
  } //while

  return true;
}

