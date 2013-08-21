<%@ page import="java.util.*"%>]
<%@ page import="java.util.concurrent.*"%>
<%@ page import="com.renren.xoa.registry.*"%>
<%@ page import="com.renren.xoa.registry.impl.zookeeper.*"%>



<%
    final ZookeeperBasedRegistry registry = (ZookeeperBasedRegistry) XoaRegistryFactory
            .getInstance().getDefaultRegistry();
    final List<XoaService> services = registry.getServices();
    for (XoaService service : services) {
        out.println(service.getServiceId());
    }
    ScheduledExecutorService exe = Executors.newSingleThreadScheduledExecutor();
    exe.scheduleAtFixedRate(new Runnable() {

        @Override
        public void run() {
            for (XoaService serviceId : services) {
                registry.getServiceNodes(serviceId.getServiceId());
            }
        }
    }, 1, 60, TimeUnit.SECONDS);
%>
Done.