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

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable : 4996) // for unsafe getenv
#endif

namespace hazelcast {
namespace client {
namespace test {
extern std::shared_ptr<RemoteControllerClient> remoteController;

class ClientStatisticsTest : public ClientTest
{
protected:
    static const int STATS_PERIOD_SECONDS = 1;

    static void SetUpTestCase()
    {
        instance = new HazelcastServer(default_server_factory());
    }

    static void TearDownTestCase()
    {
        delete instance;

        instance = nullptr;
    }

    static Response get_client_stats_from_server()
    {
        constexpr const char* script_template =
          ("clients = instance_0.getClientService().getConnectedClients()\n"
           "for client in clients:\n"
           "    if client.getName() == '%1%':\n"
           "        result = client.getClientAttributes()\n"
           "        break\n");

        std::string script =
          boost::str(boost::format(script_template) % get_test_name());

        Response response;
        remote_controller_client().executeOnController(
          response,
          default_server_factory().get_cluster_id(),
          script,
          Lang::PYTHON);
        return response;
    }

    static std::string unescape_special_characters(const std::string& value)
    {
        std::string escapedValue = boost::replace_all_copy(value, "\\,", ",");
        boost::replace_all(escapedValue, "\\=", "=");
        boost::replace_all(escapedValue, "\\\\", "\\");
        return escapedValue;
    }

    std::unordered_map<std::string, std::string> get_stats_from_response(
      const Response& stats_response)
    {
        std::unordered_map<std::string, std::string> statsMap;
        if (stats_response.success && !stats_response.result.empty()) {
            std::vector<std::string> keyValuePairs;
            boost::split(
              keyValuePairs, stats_response.result, boost::is_any_of(","));

            for (const auto& pair : keyValuePairs) {
                std::vector<std::string> keyValuePair;
                auto input = unescape_special_characters(pair);
                boost::split(keyValuePair, input, boost::is_any_of("="));

                if (keyValuePair.size() > 1) {
                    statsMap[keyValuePair[0]] = keyValuePair[1];
                } else {
                    statsMap[keyValuePair[0]] = "";
                }
            }
        }

        return statsMap;
    }

    std::unordered_map<std::string, std::string> get_stats()
    {
        auto statsResponse = get_client_stats_from_server();

        return get_stats_from_response(statsResponse);
    }

    bool verify_client_stats_from_server_is_not_empty()
    {
        auto response = get_client_stats_from_server();
        return response.success && !response.result.empty();
    }

    std::unique_ptr<hazelcast_client> create_hazelcast_client()
    {
        client_config clientConfig;

        clientConfig.set_instance_name(get_test_name());

        clientConfig.set_property(client_properties::STATISTICS_ENABLED, "true")
          .set_property(client_properties::STATISTICS_PERIOD_SECONDS,
                        std::to_string(STATS_PERIOD_SECONDS))
          // add IMap Near Cache config
          .add_near_cache_config(config::near_cache_config(get_test_name()));

        clientConfig.get_connection_strategy_config()
          .get_retry_config()
          .set_cluster_connect_timeout(std::chrono::seconds(20))
          .set_initial_backoff_duration(std::chrono::milliseconds(100))
          .set_max_backoff_duration(std::chrono::seconds(4))
          .set_multiplier(3)
          .set_jitter(0.8);

        return std::unique_ptr<hazelcast_client>(
          new hazelcast_client{ new_client(std::move(clientConfig)).get() });
    }

    void wait_for_first_statistics_collection()
    {
        ASSERT_TRUE_EVENTUALLY_WITH_TIMEOUT(
          verify_client_stats_from_server_is_not_empty(),
          3 * STATS_PERIOD_SECONDS);
    }
