package com.renren.commons.tools.findunusedjar;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

import sun.jvm.hotspot.memory.*;
import sun.jvm.hotspot.oops.*;
import sun.jvm.hotspot.debugger.*;
import sun.jvm.hotspot.runtime.*;
import sun.jvm.hotspot.tools.*;

@SuppressWarnings("restriction")
public class DumpClassLoadInfo extends Tool {

    private final File resultLog;

    public DumpClassLoadInfo(final File log) {
        super();
        resultLog = log;
    }

    public void run() {
        try {
            final ArrayList<InstanceKlass> klasses = new ArrayList<InstanceKlass>(128);
            final SystemDictionary dict = VM.getVM().getSystemDictionary();
            dict.classesDo(new SystemDictionary.ClassVisitor() {

                public void visit(final Klass k) {
                    if (k instanceof InstanceKlass) {
                        klasses.add((InstanceKlass) k);
                    }
                }
            });

            Collections.sort(klasses, new Comparator<InstanceKlass>() {

                public int compare(final InstanceKlass x, final InstanceKlass y) {
                    return x.getName().asString().compareTo(y.getName().asString());
                }
            });

            //System.out.println("class                path                loader");
            //System.out.println("-----------------------------------------------");
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new FileWriter(resultLog, true));
                for (final InstanceKlass k : klasses) {
                    final String data = getClassNameFrom(k) + "," + getPathFromInstanceKlass(k) + "," + getClassLoaderOopFrom(k);
                    writer.append(data);
                    writer.newLine();
                    //System.out.printf("%s, %s, %s\n", getClassNameFrom(k), getPathFromInstanceKlass(k), getClassLoaderOopFrom(k));
                }
            } catch (final IOException e) {
                e.printStackTrace();
            } finally {
                if (writer != null) {
                    try {
                        writer.flush();
                        writer.close();
                    } catch (final IOException e) {
                        e.printStackTrace();
                    }
                }
            }

        } catch (final AddressException e) {
            System.err.println("Error accessing address 0x" + Long.toHexString(e.getAddress()));
            e.printStackTrace();
        }
    }

    private static String getClassNameFrom(final InstanceKlass klass) {
        return klass != null ? klass.getName().asString().replace('/', '.') : null;
    }

    private static String getPathFromInstanceKlass(final InstanceKlass klass) {
        final Oop protectionDomain = klass.getProtectionDomain();
        if (protectionDomain == null) {
            return null;
        }

        final Oop codesource = getOopFieldValueFrom(protectionDomain, "codesource", "Ljava/security/CodeSource;");
        if (codesource == null) {
            return null;
        }

        final Oop location = getOopFieldValueFrom(codesource, "location", "Ljava/net/URL;");
        if (location == null) {
            return null;
        }

        final Oop path = getOopFieldValueFrom(location, "path", "Ljava/lang/String;");
        if (path == null) {
            return null;
        }

        return OopUtilities.stringOopToString(path);
    }

    private static String getClassLoaderOopFrom(final InstanceKlass klass) {
        final Oop loader = klass.getClassLoader();
        return loader != null ? getClassNameFrom((InstanceKlass) loader.getKlass()) + " @ " + loader.getHandle() : "<bootstrap>";
    }

    private static Oop getOopFieldValueFrom(final Oop oop, final String name, final String sig) {
        final InstanceKlass klass = (InstanceKlass) oop.getKlass();
        final OopField field = (OopField) klass.findField(name, sig);
        return field.getValue(oop);
    }

    @Override
    public String getName() {
        return "dumpClassURL";
    }

    @Override
    protected void stop() {
        System.out.println();
        System.out.println("dump success,view classLoadInfo.log..");
        super.stop();
    }

    public static void main(final String[] args) {
        if (args.length != 1) {
            throw new IllegalArgumentException("argument is wrong..");
        }
        System.out.println("check class loading info...");
        final long pid = Long.parseLong(args[0]);
        //判断是否有类被卸载了
        final ClassLoadingUtil util = new ClassLoadingUtil();
        final long unloadClassCount = util.getUnloadedClassCount(pid);
        if (unloadClassCount > 0) {
            System.out.println("error,some class uploaded..");
            System.exit(0);
        } else {
            System.out.println("class load normal...");
            System.out.println();
        }

        final String currentPath = System.getProperty("user.dir");
        final String fileLog = currentPath + File.separator + "classLoadInfo.log";
        final File f = new File(fileLog);
        //f.deleteOnExit();
        final DumpClassLoadInfo tool = new DumpClassLoadInfo(f);
        tool.start(args);
        tool.stop();
    }
}
