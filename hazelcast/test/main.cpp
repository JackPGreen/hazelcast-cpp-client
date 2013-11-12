#include "SimpleMapTest.h"
#include "HazelcastInstanceFactory.h"
#include "ClientSerializationTest.h"
#include "ClientMapTest.h"
#include "ClientQueueTest.h"
#include "testUtil.h"
#include "IAtomicLongTest.h"
#include "IdGeneratorTest.h"
#include "ICountDownLatchTest.h"
#include "ClientLockTest.h"
#include "ClientSemaphoreTest.h"
#include "ClientTopicTest.h"
#include "IExecutorServiceTest.h"
#include "ClientMultiMapTest.h"
#include "ClientListTest.h"
#include "ClientSetTest.h"
#include "ClientTxnListTest.h"
#include "ClientTxnMapTest.h"
#include "ClientTxnMultiMapTest.h"
#include "ClientTxnQueueTest.h"
#include "ClientTxnSetTest.h"
#include "ClientTxnTest.h"
#include "ClientMapIssueTest.h"

using namespace hazelcast::client::test;

int testSpeed() {
    SimpleMapTest s(SERVER_ADDRESS, SERVER_PORT);
    s.run();
    return 0;
};


int main(int argc, char **argv) {
    HazelcastInstanceFactory factory;
    ClientSerializationTest serializationTest;
    serializationTest.executeTests();
    ClientMapTest mapTest(factory);
    mapTest.executeTests();
    ClientMapIssueTest mapIssueTest(factory);
    mapIssueTest.executeTests();
    ClientMultiMapTest multiMapTest(factory);
    multiMapTest.executeTests();
    ClientQueueTest queueTest(factory);
    queueTest.executeTests();
    ClientListTest listTest(factory);
    listTest.executeTests();
    ClientSetTest setTest(factory);
    setTest.executeTests();
    IAtomicLongTest atomTest(factory);
    atomTest.executeTests();
    IdGeneratorTest generatorTest(factory);
    generatorTest.executeTests();
    ICountDownLatchTest latchTest(factory);
    latchTest.executeTests();
    ClientLockTest lockTest(factory);
    lockTest.executeTests();
    ClientSemaphoreTest semaphoreTest(factory);
    semaphoreTest.executeTests();
    ClientTopicTest topicTest(factory);
    topicTest.executeTests();
    IExecutorServiceTest executorServiceTest(factory);
    executorServiceTest.executeTests();
    ClientTxnListTest clientTxnListTest(factory);
    clientTxnListTest.executeTests();
    ClientTxnMapTest clientTxnMapTest(factory);
    clientTxnMapTest.executeTests();
    ClientTxnMultiMapTest clientTxnMultiMapTest(factory);
    clientTxnMultiMapTest.executeTests();
    ClientTxnQueueTest clientTxnQueueTest(factory);
    clientTxnQueueTest.executeTests();
    ClientTxnSetTest clientTxnSetTest(factory);
    clientTxnSetTest.executeTests();
    ClientTxnTest clientTxnTest(factory);
    clientTxnTest.executeTests();
//    testSpeed();
    return 0;
};

