module com {
        module renren {
                module searchrelation {
                        dictionary<int, int> Int2IntMap;
                        dictionary<int, short> Int2ShortMap;
			dictionary<int,Int2ShortMap> RelationMap;
			sequence<byte> ByteSeq;
			dictionary<int,ByteSeq> ByteSeqMap;

                        interface SearchRelationLoader {
                                void pregetRelation(int userID, int degrees ,string callBiz);
                                Int2IntMap getRelation(int userID, int degrees ,string callBiz);
                                RelationMap getRelationV2(int userID, int degrees ,string callBiz);
                                ByteSeqMap getRelationV3(int userID, int degrees ,string callBiz);
                        };
                };
        };
};

