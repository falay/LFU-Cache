# Hardware simulation: LFU cache sim

class LFUCache:

    class cache_node(object):

        def __init__(self, key=None, value=None):

            self.key = key
            self.value = value
            self.frequence = 0
            self.prev = None
            self.next = None

    class frequence_node(object):

        def __init__(self, freq):

            self.freq = freq
            self.prev     = None
            self.next     = None
            self.dummy_head = LFUCache.cache_node()
            self.last_cache_node    = None

        def has_no_cache_nodes(self):

            return self.last_cache_node == None or self.last_cache_node == self.dummy_head


    def __init__(self, capacity):
        """
        :type capacity: int
        """
        self.capacity = capacity
        self.cache_node_map = {}
        self.freq_node_tail = None
        self.freq_dummy_head = self.frequence_node(0)
        self.freq_node_map  = { 0 : self.freq_dummy_head }

    def is_tail(self, freq_node):

        return self.freq_node_tail == freq_node

    def move_freq_tail(self):

        if self.freq_node_tail:
            self.freq_node_tail = self.freq_node_tail.next

    def set_new_freq_tail(self, freq_node, target_node):

        if self.freq_node_tail is None or target_node.frequence == 1:
            self.freq_node_tail = freq_node

    def set_new_cache_last(self, init, freq_node, target_cache_node):

        if init:
            if freq_node.last_cache_node is None:
                freq_node.last_cache_node = target_cache_node
        else:
            if freq_node.last_cache_node == target_cache_node:
                freq_node.last_cache_node = target_cache_node.prev


    def freq_updator(self, target_cache_node):

        freq_node = self.get_new_freq_node( target_cache_node.frequence+1 )     
        
        if target_cache_node.frequence > 0:
            self.remove_from_freq_node( target_cache_node )
        
        target_cache_node.frequence += 1 

        self.add_link( freq_node.dummy_head, target_cache_node )

        self.set_new_cache_last( True, freq_node, target_cache_node )

        self.set_new_freq_tail( freq_node, target_cache_node )


    def get_new_freq_node(self, frequence):

        if not frequence in self.freq_node_map:
            self.freq_node_map[ frequence ] = self.frequence_node( frequence )
            self.add_link( self.freq_node_map[ frequence-1 ], self.freq_node_map[ frequence ] )

        return self.freq_node_map[ frequence ]



    def remove_from_freq_node(self, target_cache_node):

        current_freq_node = self.freq_node_map[ target_cache_node.frequence ]

        self.set_new_cache_last( False, current_freq_node, target_cache_node )
        self.delete_link( target_cache_node )
        self.check_freq_node_empty( current_freq_node )


    def check_freq_node_empty(self, freq_node):
        
        if freq_node.has_no_cache_nodes():
            if self.is_tail( freq_node ):
                self.move_freq_tail()        
            self.delete_link( freq_node )  
            self.freq_node_map.pop( freq_node.freq )
            del freq_node.dummy_head 
            freq_node.dummy_head = None
            del freq_node


    def delete_link(self, target_node):

        target_node.prev.next = target_node.next        
        if target_node.next is not None:
            target_node.next.prev = target_node.prev
        target_node.next = target_node.prev = None


    def add_link(self, base_node, added_node):

        added_node_next = base_node.next
        added_node.next = added_node_next
        if added_node_next is not None:
            added_node_next.prev = added_node
        base_node.next = added_node
        added_node.prev = base_node 


    def remove_least_freq_node(self):

        removed_cache_node = self.freq_node_tail.last_cache_node
        self.remove_from_freq_node( removed_cache_node )
        self.cache_node_map.pop( removed_cache_node.key )
        removed_cache_node.prev = removed_cache_node.next = None
        del removed_cache_node



    def get(self, key):
        """
        :type key: int
        :rtype: int
        """
        if self.capacity == 0 or (key not in self.cache_node_map):
            return -1
        
        target_cache_node = self.cache_node_map[ key ]
        self.freq_updator( target_cache_node )

        return target_cache_node.value


    def put(self, key, value):
        """
        :type key: int
        :type value: int
        :rtype: void
        """
        if self.capacity == 0:
            return

        if not key in self.cache_node_map:
            self.cache_node_map[ key ] = self.cache_node(key, value)
            if len( self.cache_node_map ) > self.capacity:
                self.remove_least_freq_node() 

        target_cache_node = self.cache_node_map[ key ]
        self.freq_updator( target_cache_node )
        target_cache_node.value = value
    
