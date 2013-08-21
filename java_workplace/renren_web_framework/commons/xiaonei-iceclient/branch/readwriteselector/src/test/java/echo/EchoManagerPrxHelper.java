// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1

package echo;

public final class EchoManagerPrxHelper extends Ice.ObjectPrxHelperBase implements EchoManagerPrx
{
    public int
    echo(int count, int delay)
    {
        return echo(count, delay, null, false);
    }

    public int
    echo(int count, int delay, java.util.Map<String, String> __ctx)
    {
        return echo(count, delay, __ctx, true);
    }

    @SuppressWarnings("unchecked")
    private int
    echo(int count, int delay, java.util.Map<String, String> __ctx, boolean __explicitCtx)
    {
        if(__explicitCtx && __ctx == null)
        {
            __ctx = _emptyContext;
        }
        int __cnt = 0;
        while(true)
        {
            Ice._ObjectDel __delBase = null;
            try
            {
                __checkTwowayOnly("echo");
                __delBase = __getDelegate(false);
                _EchoManagerDel __del = (_EchoManagerDel)__delBase;
                return __del.echo(count, delay, __ctx);
            }
            catch(IceInternal.LocalExceptionWrapper __ex)
            {
                __handleExceptionWrapper(__delBase, __ex, null);
            }
            catch(Ice.LocalException __ex)
            {
                __cnt = __handleException(__delBase, __ex, null, __cnt);
            }
        }
    }

    public static EchoManagerPrx
    checkedCast(Ice.ObjectPrx __obj)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            try
            {
                __d = (EchoManagerPrx)__obj;
            }
            catch(ClassCastException ex)
            {
                if(__obj.ice_isA("::echo::EchoManager"))
                {
                    EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
                    __h.__copyFrom(__obj);
                    __d = __h;
                }
            }
        }
        return __d;
    }

    public static EchoManagerPrx
    checkedCast(Ice.ObjectPrx __obj, java.util.Map<String, String> __ctx)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            try
            {
                __d = (EchoManagerPrx)__obj;
            }
            catch(ClassCastException ex)
            {
                if(__obj.ice_isA("::echo::EchoManager", __ctx))
                {
                    EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
                    __h.__copyFrom(__obj);
                    __d = __h;
                }
            }
        }
        return __d;
    }

    public static EchoManagerPrx
    checkedCast(Ice.ObjectPrx __obj, String __facet)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            Ice.ObjectPrx __bb = __obj.ice_facet(__facet);
            try
            {
                if(__bb.ice_isA("::echo::EchoManager"))
                {
                    EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
                    __h.__copyFrom(__bb);
                    __d = __h;
                }
            }
            catch(Ice.FacetNotExistException ex)
            {
            }
        }
        return __d;
    }

    public static EchoManagerPrx
    checkedCast(Ice.ObjectPrx __obj, String __facet, java.util.Map<String, String> __ctx)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            Ice.ObjectPrx __bb = __obj.ice_facet(__facet);
            try
            {
                if(__bb.ice_isA("::echo::EchoManager", __ctx))
                {
                    EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
                    __h.__copyFrom(__bb);
                    __d = __h;
                }
            }
            catch(Ice.FacetNotExistException ex)
            {
            }
        }
        return __d;
    }

    public static EchoManagerPrx
    uncheckedCast(Ice.ObjectPrx __obj)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            try
            {
                __d = (EchoManagerPrx)__obj;
            }
            catch(ClassCastException ex)
            {
                EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
                __h.__copyFrom(__obj);
                __d = __h;
            }
        }
        return __d;
    }

    public static EchoManagerPrx
    uncheckedCast(Ice.ObjectPrx __obj, String __facet)
    {
        EchoManagerPrx __d = null;
        if(__obj != null)
        {
            Ice.ObjectPrx __bb = __obj.ice_facet(__facet);
            EchoManagerPrxHelper __h = new EchoManagerPrxHelper();
            __h.__copyFrom(__bb);
            __d = __h;
        }
        return __d;
    }

    protected Ice._ObjectDelM
    __createDelegateM()
    {
        return new _EchoManagerDelM();
    }

    protected Ice._ObjectDelD
    __createDelegateD()
    {
        return new _EchoManagerDelD();
    }

    public static void
    __write(IceInternal.BasicStream __os, EchoManagerPrx v)
    {
        __os.writeProxy(v);
    }

    public static EchoManagerPrx
    __read(IceInternal.BasicStream __is)
    {
        Ice.ObjectPrx proxy = __is.readProxy();
        if(proxy != null)
        {
            EchoManagerPrxHelper result = new EchoManagerPrxHelper();
            result.__copyFrom(proxy);
            return result;
        }
        return null;
    }
}
