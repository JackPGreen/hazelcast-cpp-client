/*
 * Copyright (c) 2008-2025, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>
#include <unordered_set>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/future.hpp>

#include <gtest/gtest.h>

#ifdef HZ_BUILD_WITH_SSL
#include <openssl/crypto.h>
#endif

#include <hazelcast/client/client_config.h>
#include <hazelcast/client/cluster.h>
#include <hazelcast/client/connection/ClientConnectionManagerImpl.h>
#include <hazelcast/client/connection/Connection.h>
#include <hazelcast/client/entry_event.h>
#include <hazelcast/client/exception/protocol_exceptions.h>
#include <hazelcast/client/hazelcast_client.h>
#include <hazelcast/client/imap.h>
#include <hazelcast/client/impl/Partition.h>
#include <hazelcast/client/initial_membership_event.h>
#include <hazelcast/client/itopic.h>
#include <hazelcast/client/lifecycle_listener.h>
#include <hazelcast/client/membership_listener.h>
#include <hazelcast/client/multi_map.h>
#include <hazelcast/client/proxy/PNCounterImpl.h>
#include <hazelcast/client/reliable_topic.h>
#include <hazelcast/client/serialization/pimpl/data_input.h>
#include <hazelcast/client/serialization/pimpl/data_output.h>
#include <hazelcast/client/serialization/serialization.h>
#include <hazelcast/client/socket_interceptor.h>
#include <hazelcast/client/socket.h>
#include <hazelcast/client/spi/ClientContext.h>
#include <hazelcast/logger.h>
#include <hazelcast/util/AddressHelper.h>
#include <hazelcast/util/AddressUtil.h>
#include <hazelcast/util/MurmurHash3.h>
#include <hazelcast/util/Util.h>

#ifdef HZ_BUILD_WITH_SSL
#include <hazelcast/client/internal/socket/SSLSocket.h>
#include <hazelcast/util/SyncHttpsClient.h>
#endif // HZ_BUILD_WITH_SSL

#include "ClientTest.h"
#include "HazelcastServer.h"
#include "HazelcastServerFactory.h"
#include "TestHelperFunctions.h"
#include "ringbuffer/StartsWithStringFilter.h"
#include "serialization/Serializables.h"
#include "remote_controller_client.h"
