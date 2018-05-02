package com.iutils.network.bean;

public class CheckIpConnTask {
    private String ip;
    private int port;
    private int timeout;
    private boolean isTimeout;
    private long takeTimes;


    private CheckIpConnTask(Builder builder){
        this.ip = builder.ip;
        this.port = builder.port;
        this.timeout = builder.timeout;
        this.isTimeout = builder.isTimeout;
        this.takeTimes = builder.takeTimes;
    }

    public String getIp() {
        return ip;
    }

    public int getPort() {
        return port;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(boolean timeout) {
        isTimeout = timeout;
    }

    public void setTakeTimes(long takeTimes) {
        this.takeTimes = takeTimes;
    }


    @Override
    public String toString() {
        return "CheckIpConnTask{" +
                "ip='" + ip + '\'' +
                ", port=" + port +
                ", timeout=" + timeout +
                ", isTimeout=" + isTimeout +
                ", takeTimes=" + takeTimes +
                '}';
    }

    public static class Builder{
        private String ip;
        private int port;
        private int timeout = 10 *1000;//默认超时时间
        private boolean isTimeout = false;
        private long takeTimes=Long.MAX_VALUE;

        public Builder(String ip, int port){
            this.ip = ip;
            this.port = port;
        }

        public Builder timeout(int timeout){
            this.timeout = timeout;
            return this;
        }

        public Builder isTimeout(boolean isTimeout){
            this.isTimeout = isTimeout;
            return this;
        }

        public Builder takeTimes(long takeTimes){
            this.takeTimes = takeTimes;
            return this;
        }

        public CheckIpConnTask build(){
            return new CheckIpConnTask(this);
        }
    }
}
