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
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable : 4251) // for dll export
#endif

namespace hazelcast {
namespace client {
/**
 * This marker interface is needed to make template matching work during
 * serialisation.
 */
class HAZELCAST_API partition_aware_marker
{};

/**
 * PartitionAware means that data will be based in the same member based on the
 * partition key and implementing tasks will be executed on the {@link
 * #getPartitionKey()}'s owner member. <p/> This achieves data affinity. data
 * and execution occurs on the same partition. <p/> In Hazelcast, disparate data
 * structures will be stored on the same partition, based on the partition key.
 * For example, if "Steve" was used, then the following would be on one
 * partition. <ul> <li>a customers IMap with an entry of key "Steve"</li> <li>an
 * orders IMap using a customer key type implementing PartitionAware with key
 * "Steve</li> <li>any queue named "Steve"</li> <li>any PartitionAware object
 * with partition key "Steve"</li>
 * </ul>
 *
 * If you have a {@link com.hazelcast.core.IExecutorService} which needs to deal
 * with a customer and a customer's orders, you can achieve optimal performance
 * by putting them on the same partition. <p/>
 * {@link DistributedObject} also has a notion of the partition key which is of
 * type String to ensure that the same partition as distributed Objects Strings
 * is used for the partition key. <p/>
 * @see DistributedObject
 * @param <T> key type
 */
template<typename T>
class partition_aware : public partition_aware_marker
{
public:
    typedef T KEY_TYPE;

    /**
     * The key that will be used by Hazelcast to specify the partition.
     * You should give the same key for objects that you want to be in the same
     * partition.
     *
     * @return the key that specifies the partition.
     * If returned object is a NULL pointer, it will not be used for partition
     * calculation.
     */
    virtual const T* get_partition_key() const = 0;
};
} // namespace client
} // namespace hazelcast

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(pop)
#endif
