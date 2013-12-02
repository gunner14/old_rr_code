module com
{
  module xiaonei
  {
    module services
    {
      module FollowerInput
      {
        sequence<int> IntSeq;
        interface FollowerInputManager
        {
          IntSeq getFollowerInput(int hostId, string condition, int limit);
          void reLoadFollower(int hostId);
        };
        interface FollowerInputData{};
      };
    };
  };
};
