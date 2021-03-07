function prepare() {
    echo "local1.* /var/log/np.log" > /etc/rsyslog.d/01-np.conf
    > /var/log/np.log
    chmod a+rw /var/log/np.log

    rsyslogd
}

function teardown() {
    kill -9 `pidof rsyslogd`
}

function runtest() {
    LD_LIBRARY_PATH=`pwd`/service/output/lib:$LD_LIBRARY_PATH ./service/output/bin/ut_test_common --gtest_output=xml:.test/test_common.xml
}

function main() {
    prepare
    runtest
    teardown
}

main