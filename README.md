# LFU Cache

Introduction
------------
Hardware simulation: LFU cache simulation implemented by C++/python

Inspired by the leetcode OJ: https://leetcode.com/problems/lfu-cache/

Design and implement a data structure for Least Frequently Used (LFU) cache. It should support the following operations: get and put.

Compatibility
-------------
Functions:
* get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
* put(key, value) - Set or insert the value if the key is not already present. When the cache reaches its capacity, it should invalidate the least frequently used item before inserting a new item. For the purpose of this problem, when there is a tie (i.e., two or more keys that have the same frequency), the least recently used key would be evicted.

Features
-------------
1. Both operations are executed in O(1) time complexity
2. Runtime beats 95.58% of cpp submissions
