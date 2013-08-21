package xiaonei.commons.jade.example.controllers;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

public class TraceInterceptor extends ControllerInterceptorAdapter {

    @Override
    public Object before(Invocation inv) throws Exception {

        System.out.println("========== Before: " + inv.getControllerClass().getName() // NL
                + "#" + inv.getMethod().getName());

        return true;
    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {

        System.out.println("========== After: " + inv.getControllerClass().getName() // NL
                + "#" + inv.getMethod().getName());

        return instruction;
    }
}
