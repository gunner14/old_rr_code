package com.renren.commons.tools.holmes;

import sun.jvm.hotspot.oops.*;
import sun.jvm.hotspot.debugger.*;
import sun.jvm.hotspot.tools.*;
import sun.jvm.hotspot.utilities.*;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Apr 15, 2012 2:12:41 PM
 * @Who 参照:https://gist.github.com/1593521
 * @Modify
 */
public class DirectMemorySize extends Tool {

    @Override
    public void run() {
        // Ready to go with the database...
        try {
            final long reservedMemory = getStaticLongFieldValue("java.nio.Bits", "reservedMemory");
            final long directMemory = getStaticLongFieldValue("sun.misc.VM", "directMemory");

            System.out.println("NIO direct memory:");
            System.out.printf("  reserved size = %fMB (%d bytes)\n", toM(reservedMemory), reservedMemory);
            System.out.printf("  max size = %fMB (%d bytes)\n", toM(directMemory), directMemory);
        } catch (final AddressException e) {
            System.err.println("Error accessing address 0x" + Long.toHexString(e.getAddress()));
            e.printStackTrace();
        }
    }

    private static long getStaticLongFieldValue(final String className, final String fieldName) {
        final InstanceKlass klass = SystemDictionaryHelper.findInstanceKlass(className);
        final LongField field = (LongField) klass.findField(fieldName, "J");
        return field.getValue(klass);
    }

    private static double toM(final long value) {
        return value / (1024 * 1024.0);
    }

    @Override
    public String getName() {
        return "directMemorySize";
    }

    public static void main(final String[] args) {
        final DirectMemorySize tool = new DirectMemorySize();
        tool.start(args);
        tool.stop();
    }
}
