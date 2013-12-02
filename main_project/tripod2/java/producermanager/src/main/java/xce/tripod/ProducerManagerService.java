package xce.tripod;

import java.io.PrintStream;

import net.paoding.rose.scanning.context.RoseAppContext;

import org.apache.log4j.Logger;
import org.apache.log4j.xml.DOMConfigurator;

import xce.tripod.example.ExampleProducer;
import xce.util.service.ServiceI;

/**
 * ProducerManagerService 入口
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class ProducerManagerService extends ServiceI {

    private static final Logger logger = xce.util.log.Log.getInstance()
	    .getLog();

    private static RoseAppContext roseAppContext = null;

    public static RoseAppContext getRoseAppContext() {

	if (roseAppContext == null) {
	    synchronized (ProducerManagerService.class) {
		if (roseAppContext == null) {
		    roseAppContext = new RoseAppContext();
		}
	    }
	}
	return roseAppContext;
    }

    public static void tieSystemOutAndErrToLog() {
	System.setOut(createLoggingProxy(System.out));
	System.setErr(createLoggingProxy(System.err));
    }

    public static PrintStream createLoggingProxy(
	    final PrintStream realPrintStream) {
	return new PrintStream(realPrintStream) {
	    public void print(final String string) {
		realPrintStream.print(string);
		logger.info(string);
	    }
	};
    }

    @Override
    public void initialize() {
	DOMConfigurator.configure("../etc/classes/log4j.xml");
	tieSystemOutAndErrToLog();
	_adapter.add(new ProducerManagerImpl(), Ice.Util.stringToIdentity("M"));
	logger.warn("ProducerManagerService is started!");
    }

    @Override
    public void finishup() {
	logger.warn("BuddyXoaLogicService is finished!");
    }

}
