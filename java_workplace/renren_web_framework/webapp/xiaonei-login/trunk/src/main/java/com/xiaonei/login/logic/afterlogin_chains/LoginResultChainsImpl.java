package com.xiaonei.login.logic.afterlogin_chains;

public class LoginResultChainsImpl implements LoginResultChains, Cloneable {

    private volatile LoginResultChainNode firstNode;

    @Override
    public LoginResultChainNode getFristNode() {
        return firstNode;
    }

    @Override
    public void setNode(LoginResultChainNode node) {
        if (isEmpty()) {
            firstNode = node;
        } else {
            LoginResultChainNode tempNode = firstNode;
            while (!tempNode.getNext().isEnd()) {
                // 如果重复不添加
                if (tempNode.equals(node)) {
                    return;
                }
                tempNode = tempNode.getNext();
            }
            tempNode.setNext(node);
        }
    }

    @Override
    public boolean isEmpty() {
        return null == firstNode || firstNode.isEnd();
    }

    @Override
    public LoginResultChainsImpl clone() throws CloneNotSupportedException {
        LoginResultChainsImpl chains = new LoginResultChainsImpl();
        LoginResultChainNode tempNode = firstNode;
        while (!tempNode.getNext().isEnd()) {
            chains.setNode(((AbsLoginResultChainNode) tempNode).clone());
            tempNode = tempNode.getNext();
        }
        return chains;
    }
}
