/*
 *    Copyright (C) 2020 Ren-Chu Wang, All Rights Reserved
 *    Unauthorized copying of this file, via any medium is strictly prohibited
 *    Proprietary and confidential
 */

#pragma once

#include <memory>
#include <unordered_map>

namespace safe {
template <typename K, typename V>
class unordered_map {
   public:
    unordered_map() : field(std::unordered_map<K, V>()) {}
    unordered_map(const unordered_map& hashmap) : field(hashmap.field) {}
    unordered_map(unordered_map&& hashmap) : field(std::move(hashmap.field)) {}

    unordered_map& operator=(const unordered_map& hashmap) {
        field = hashmap.field;
        return *this;
    }
    unordered_map& operator=(unordered_map&& hashmap) {
        field = std::move(hashmap.field);
        return *this;
    }

    void reserve(size_t capacity) { field.reserve(capacity); }

    void rehash(size_t new_size) { field.rehash(new_size); }
    size_t size() const { return field.size(); }

    const V& operator[](const K& key) const {
        assert(contains(key));
        return field[key];
    }
    V& operator[](const K& key) {
        assert(contains(key));
        return field[key];
    }

    size_t erase(const K& key) {
        assert(contains(key));
        return field.erase(key);
    }

    typename std::unordered_map<K, V>::iterator begin() {
        return field.begin();
    }
    typename std::unordered_map<K, V>::const_iterator begin() const {
        return field.begin();
    }

    typename std::unordered_map<K, V>::iterator end() { return field.end(); }
    typename std::unordered_map<K, V>::const_iterator end() const {
        return field.end();
    }

    typename std::unordered_map<K, V>::iterator find(const K& key) {
        return field.find(key);
    }
    typename std::unordered_map<K, V>::const_iterator find(const K& key) const {
        return field.find(key);
    }

    bool contains(const K& key) const { return find(key) != end(); }

   private:
    std::unordered_map<K, V> field;
};
}  // namespace safe
