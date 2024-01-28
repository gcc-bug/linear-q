#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <map>
#include <set>
#include <unordered_set>
#include <stdexcept>

template <typename K>
class BiMap {
    public:
        void insert(const K& key, int value);
        K getKey(int value) const;
        int getValue(const K& key) const;
        BiMap(const std::set<K>& keys);
        BiMap(const std::unordered_set<K>& keys);
        void message();

    private:
        std::map<K, int> keyToValue;
        std::map<int, K> valueToKey;

        bool isValuePresent(int value) const;
};

template <typename K>
void BiMap<K>::insert(const K& key, int value) {
    if (keyToValue.find(key) != keyToValue.end() || isValuePresent(value)) {
        throw std::invalid_argument("Duplicate key or value");
    }

    keyToValue[key] = value;
    valueToKey[value] = key;
}

template <typename K>
K BiMap<K>::getKey(int value) const {
    auto it = valueToKey.find(value);
    if (it == valueToKey.end()) {
        throw std::out_of_range("Value not found");
    }
    return it->second;
}

template <typename K>
int BiMap<K>::getValue(const K& key) const {
    auto it = keyToValue.find(key);
    if (it == keyToValue.end()) {
        throw std::out_of_range("Key not found");
    }
    return it->second;
}

template <typename K>
bool BiMap<K>::isValuePresent(int value) const {
    return valueToKey.find(value) != valueToKey.end();
}

template <typename K>
BiMap<K>::BiMap(const std::set<K>& keys) {
    int value = 0;
    for (const auto& key : keys) {
        insert(key, value++);
    }
}

template <typename K>
BiMap<K>::BiMap(const std::unordered_set<K>& keys) {
    int value = 0;
    for (const auto& key : keys) {
        insert(key, value++);
    }
}

template <typename K>
void BiMap<K>::message(){
    for (const auto& pair : keyToValue) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

#endif // BIMAP_HPP