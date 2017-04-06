#ifndef MAP_THREAD_SAFE
#define MAP_THREAD_SAFE

#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>

namespace std
{
    namespace threadsafe
    {
        /*
        Concurrent Map prototype, it allows concurrent reads and safe writes.

        */
        template<class K,class V>
        class map
        {
            std::map<K,V> internal_map;

            std::shared_mutex mutex;

            public:
                map(){}
                map(const std::map<K,V> &m) : internal_map(m){}
                map(const std::threadsafe::map &m) = delete;

                V* get(const K& key)
                {
                    std::shared_lock<std::shared_mutex> lock(mutex);
                    auto it = internal_map.find(key);
                    if (it == internal_map.end())
                    {
                        return nullptr;
                    }
                    else{
                        return new V(it->second);
                    }
                }

                auto put(const K & key,V & value){
                    std::unique_lock<std::shared_mutex> lock(mutex);
                    return internal_map.insert(std::make_pair(key,value));
                }

                size_t size(){
                    std::shared_lock<std::shared_mutex> lock(mutex);
                    return internal_map.size();
                }

        };
    }
}

#endif
