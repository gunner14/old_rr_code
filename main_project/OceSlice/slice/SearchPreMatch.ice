module com {
  module renren {
    module search {
     
  sequence<string> MatchStr;
//      dictionary<int, string> QueryMap;

      interface PreMatchManager 
      {
        MatchStr searchPreMatch(int uid, string queryy);
      };
    };
  };
};
