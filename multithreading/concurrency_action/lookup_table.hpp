//
//  LookupTable.hpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 05/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef LookupTable_h
#define LookupTable_h

#include <functional>
#include <mutex>
#include <list>
#include <vector>
#include <memory>
#include "boost/thread/shared_mutex.hpp"

namespace concurrency
{
   template<typename Key, typename Value, typename Hash = std::hash<Key>>
   class lookup_table
   {
   private:
      
      class bucket_type
      {
      private:
         
         using bucket_value = std::pair<Key, Value>;
         using bucket_data = std::list<bucket_value>;
         using bucket_iterator = typename bucket_data::iterator;
         using bucket_const_iterator = typename bucket_data::const_iterator;

         using mutex_type = boost::shared_mutex;
         using lock = std::unique_lock<mutex_type>;
         using shared_lock = boost::shared_lock<mutex_type>;
         
         bucket_data data_;
         
      public:
         
         mutable mutex_type mutex_;

         const bucket_iterator find_entry_for(Key const& key)
         {
            auto f = [&](const bucket_value& item){ return item.first == key; };
            return std::find_if( data_.begin(), data_.end(), f);
         }
         
         
         std::size_t size() const
         {
            return data_.size();
         }
         
         Value value_for(Key const& key)
         {
            shared_lock lk{mutex_};
            bucket_iterator found_entry = find_entry_for(key);
            return (found_entry == data_.end()) ? Value() : found_entry->second;
         }
         
         Value value_for(Key const& key) const
         {
            shared_lock lk{mutex_};
            bucket_iterator found_entry = find_entry_for(key);
            return (found_entry == data_.end()) ? Value() : found_entry->second;
         }
         
         void add_update_bucket(Key const & key, Value const& value)
         {
            lock lk{mutex_};
            bucket_iterator const found_entry = find_entry_for(key);
            if( found_entry == data_.end() )
            {
               data_.push_back(bucket_value(key, value));
            }
            else
            {
               found_entry->second = value;
            }
         }
         
         void remove_key(Key const& key)
         {
            lock lk{mutex_};
            bucket_iterator const found_entry = find_entry_for(key);
            if( found_entry != data_.end() )
            {
               data_.erase(found_entry);
            }
         }
         
         
         bucket_data& get_bucket_data()
         {
            return data_;
         }
         
         const bucket_data& get_bucket_data() const
         {
            return data_;
         }
   };
      
      
   private:
      
      std::vector<std::unique_ptr<bucket_type>> buckets_;
      Hash hash_;
      
      bucket_type& get_bucket(Key const& key) const
      {
         const std::size_t bucket_index = hash_(key) % buckets_.size();
         return *buckets_[bucket_index];
      }
      
   public:
      
      using key_type = Key;
      using value_type = Value;
      using hash_type = Hash;
      
      lookup_table( unsigned nbuckets = 19, Hash const& hash = Hash() ) :
         buckets_(nbuckets), hash_(hash)
      {
         for(unsigned i = 0; i < buckets_.size(); ++i)
            buckets_[i] = std::make_unique<bucket_type>();
      }
      
      lookup_table(lookup_table const& x) = delete;
      lookup_table& operator=(lookup_table const& x) = delete;
      
      value_type get(key_type const& key) const
      {
         return get_bucket(key).value_for(key);
      }
      
      void add(key_type const& key, value_type const& value)
      {
         get_bucket(key).add_update_bucket(key, value);
      }
      
      void remove(key_type const& key)
      {
         get_bucket(key).remove_key(key);
      }
      
      std::size_t count(key_type const& key)
      {
         const auto& bucket = get_bucket(key);
         return bucket.size();
      }
      
      unsigned long bucket_id(key_type const& key) const
      {
         return hash_(key) % buckets_.size();
      }
      
      std::map<key_type, value_type> convert_to_map() const
      {
         std::vector<std::unique_lock<boost::shared_mutex>> locks;
         for(unsigned i = 0; i < buckets_.size(); ++i)
         {
            
            locks.push_back(
                            std::unique_lock<boost::shared_mutex>(buckets_[i]->mutex_));
         }
         std::map<key_type, value_type> result;

         using bucket_iterator = typename std::list<std::pair<key_type, value_type>>::iterator;
         for(unsigned i=0; i<buckets_.size(); ++i)
         {
            for(bucket_iterator it = buckets_[i]->get_bucket_data().begin();
                it != buckets_[i]->get_bucket_data().end();
                ++it)
               result.insert(*it);
         }
         
         return result;
      }
     
   };

}


#endif /* LookupTable_h */
