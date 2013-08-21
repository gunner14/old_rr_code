struct BsonUnitTest : public UnitTest {
void testRegex() {

    BSONObjBuilder b;
    b.appendRegex("x", "foo");
    BSONObj o = b.done();

    BSONObjBuilder c;
    c.appendRegex("x", "goo");
    BSONObj p = c.done();

    assert( !o.woEqual( p ) );
    assert( o.woCompare( p ) < 0 );

}
void testoid() {
    OID id;
    id.init();
    //            sleepsecs(3);

    OID b;
    // goes with sleep above...
    // b.init();
    // assert( memcmp(id.getData(), b.getData(), 12) < 0 );

    b.init( id.str() );
    assert( b == id );
}

void testbounds(){
    BSONObj l , r;
    {
        BSONObjBuilder b;
        b.append( "x" , numeric_limits<long long>::max() );
        l = b.obj();
    }
    {
        BSONObjBuilder b;
        b.append( "x" , numeric_limits<double>::max() );
        r = b.obj();
    }
    assert( l.woCompare( r ) < 0 );
    assert( r.woCompare( l ) > 0 );
    {
        BSONObjBuilder b;
        b.append( "x" , numeric_limits<int>::max() );
        l = b.obj();
    }
    assert( l.woCompare( r ) < 0 );
    assert( r.woCompare( l ) > 0 );
}

void testorder(){
    {
        BSONObj x,y,z;
        { BSONObjBuilder b; b.append( "x" , (long long)2 ); x = b.obj(); }
        { BSONObjBuilder b; b.append( "x" , (int)3 ); y = b.obj(); }
        { BSONObjBuilder b; b.append( "x" , (long long)4 ); z = b.obj(); }
        assert( x.woCompare( y ) < 0 );
        assert( x.woCompare( z ) < 0 );
        assert( y.woCompare( x ) > 0 );
        assert( z.woCompare( x ) > 0 );
        assert( y.woCompare( z ) < 0 );
        assert( z.woCompare( y ) > 0 );
    }

    {
        BSONObj ll,d,i,n,u;
        { BSONObjBuilder b; b.append( "x" , (long long)2 ); ll = b.obj(); }
        { BSONObjBuilder b; b.append( "x" , (double)2 ); d = b.obj(); }
        { BSONObjBuilder b; b.append( "x" , (int)2 ); i = b.obj(); }
        { BSONObjBuilder b; b.appendNull( "x" ); n = b.obj(); }
        { BSONObjBuilder b; u = b.obj(); }

        assert( ll.woCompare( u ) == d.woCompare( u ) );
        assert( ll.woCompare( u ) == i.woCompare( u ) );
        BSONObj k = BSON( "x" << 1 );
        assert( ll.woCompare( u , k ) == d.woCompare( u , k ) );
        assert( ll.woCompare( u , k ) == i.woCompare( u , k ) );

        assert( u.woCompare( ll ) == u.woCompare( d ) );
        assert( u.woCompare( ll ) == u.woCompare( i ) );
        assert( u.woCompare( ll , k ) == u.woCompare( d , k ) );
        assert( u.woCompare( ll , k ) == u.woCompare( d , k ) );

        assert( i.woCompare( n ) == d.woCompare( n ) );

        assert( ll.woCompare( n ) == d.woCompare( n ) );
        assert( ll.woCompare( n ) == i.woCompare( n ) );
        assert( ll.woCompare( n , k ) == d.woCompare( n , k ) );
        assert( ll.woCompare( n , k ) == i.woCompare( n , k ) );

        assert( n.woCompare( ll ) == n.woCompare( d ) );
        assert( n.woCompare( ll ) == n.woCompare( i ) );
        assert( n.woCompare( ll , k ) == n.woCompare( d , k ) );
        assert( n.woCompare( ll , k ) == n.woCompare( d , k ) );
    }

    {
        BSONObj l,r;
        { BSONObjBuilder b; b.append( "x" , "eliot" ); l = b.obj(); }
        { BSONObjBuilder b; b.appendSymbol( "x" , "eliot" ); r = b.obj(); }
        assert( l.woCompare( r ) == 0 );
        assert( r.woCompare( l ) == 0 );
    }
}

void run() {
    testRegex();
    BSONObjBuilder A,B,C;
    A.append("x", 2);
    B.append("x", 2.0);
    C.append("x", 2.1);
    BSONObj a = A.done();
    BSONObj b = B.done();
    BSONObj c = C.done();
    assert( !a.woEqual( b ) ); // comments on operator==
    int cmp = a.woCompare(b);
    assert( cmp == 0 );
    cmp = a.woCompare(c);
    assert( cmp < 0 );
    testoid();
    testbounds();
    testorder();
}
} bson_unittest;