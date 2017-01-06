struct KeyNode
{
	int key;
	int value ;
	int freq ;
	KeyNode* next ;
	KeyNode* prev ;
	KeyNode(int k, int v, int f) : key(k), value(v), freq(f), next(NULL), prev(NULL) {}
} ;


struct FreqNode
{
	int freq ;
	FreqNode* next ;
	FreqNode* prev ;
	KeyNode* first ;
	KeyNode* last ;
	FreqNode(int f) : freq(f), next(NULL), prev(NULL), first(NULL), last(NULL) {}
} ;


class LFUCache 
{
	
	public:
    
		LFUCache(int capacity) 
		{
			Capacity = capacity ;
			head = new FreqNode(-1) ; // dummy head
		}
		
		int get(int key) 
		{
			if( KeyDict.count(key) )
			{
				FreqUpdater( KeyDict[key] ) ;
				return KeyDict[key]->value ;
			}	
			else
				return -1 ;
		}
		
		void put(int key, int value) 
		{
			if( Capacity <= 0 )
				return ;
			
			if( KeyDict.count(key) )
			{
				KeyDict[key]->value = value ;
				FreqUpdater( KeyDict[key] ) ;
			}	
			else
			{
				/* If the cache is full, remove the least frequence used item */
				if( KeyDict.size() == Capacity )
					keyRemover() ;				
				
				keyAdder( key, value ) ;
			}	
		}
	
		void keyRemover()
		{		
			KeyNode* killedCell = head->next->last ;
			head->next->last = killedCell->prev ;
					
			/* disable the link and map */
			killedCell->prev->next = NULL ;
			killedCell->prev = NULL ;
			KeyDict.erase( killedCell->key ) ;
			
			delete killedCell ;
			
			/* if no key nodes under this freq node, remove the entirte list */
			FreqNode* lowestCell = head->next ;
			if( lowestCell->first->next == NULL )
				freqCleaner( lowestCell ) ;
		}
		
		
		
		void freqCleaner(FreqNode* killedNode)
		{
			/* Unable the link and map */
			killedNode->prev->next = killedNode->next ;
			if( killedNode->next )
				killedNode->next->prev = killedNode->prev ;
			killedNode->next = killedNode->prev = NULL ;
			FreqDict.erase( killedNode->freq ) ;
			
			/* Remove dummy keynode head and itself */
			delete killedNode->first ;
			killedNode->first = killedNode->last = NULL ;
			delete killedNode ;
		}
		
		
		
		/* Add a key node under a freq node */
		void keyAdder(int key, int value)
		{
			KeyNode* kCell = new KeyNode(key, value, 1) ;
			KeyDict[key] = kCell ;
			
			if( FreqDict.count(1) )
			{
				KeyNode* dummy = FreqDict[1]->first ;
				KeyNode* oldCell = dummy->next ;
				
				/* dummy connects to new key node */
				dummy->next = kCell ;
				kCell->prev = dummy ;
				
				/* new key node connects to old key node */
				kCell->next = oldCell ;
				oldCell->prev = kCell ;
			}	
			else
			{
				FreqNode* fCell = new FreqNode(1) ;
				
				/* Connect to freq dummy head */
				FreqNode* oldNext = head->next ;
				head->next = fCell ;
				fCell->prev = head ;
				fCell->next = oldNext ;
				if( oldNext )
					oldNext->prev = fCell ;
				
				/* Construct dummy keyNode */
				KeyNode* dummy = new KeyNode(0, 0, 0) ;
				fCell->first = dummy ;
			
				/* Add the keyNode */
				dummy->next = kCell ;
				kCell->prev = dummy ;

				/* Map the freq node */	
				FreqDict[1] = fCell ;
				
				fCell->last = kCell ;
			}		
		}
		
		
		void FreqUpdater(KeyNode* kCell)
		{
			int freq = kCell->freq ;
			
			/* Freq node not exists */
			FreqNode* neighbor = FreqDict[freq]->next ;
			if( neighbor == NULL || neighbor->freq != freq+1 )
			{
				FreqNode* newFreqCell = new FreqNode( freq+1 ) ;
				FreqDict[ freq+1 ] = newFreqCell ;
				
				FreqDict[freq]->next = newFreqCell ;
				newFreqCell->prev = FreqDict[freq] ;
				
				newFreqCell->next = neighbor ;
				if( neighbor )
					neighbor->prev = newFreqCell ;
				
				/* Create dummy */
				newFreqCell->first = new KeyNode(0, 0, 0) ;
			}	

			keyMover( kCell ) ;			
		}	
		
		/* Move the kCell to the next freq node */
		void keyMover(KeyNode* kCell)
		{
			FreqNode* curFreqCell = FreqDict[kCell->freq] ;
			FreqNode* neighbor = curFreqCell->next ;
			
			if( kCell == curFreqCell->last )
				curFreqCell->last = kCell->prev ;
			
			kCell->prev->next = kCell->next ;
			if( kCell->next )
				kCell->next->prev = kCell->prev ; 
			
			/* If the list is empty */
			if( curFreqCell->first->next == NULL )
				freqCleaner( curFreqCell ) ;
			
			KeyNode* oldKey = neighbor->first->next ;
			neighbor->first->next = kCell ;
			kCell->prev = neighbor->first ;
			kCell->next = oldKey ;
			if( oldKey )
				oldKey->prev = kCell ;
			else
				neighbor->last = kCell ;
			
			kCell->freq ++ ;
		}
	
	
	private:
		unordered_map<int,FreqNode*> FreqDict ;
		unordered_map<int,KeyNode*> KeyDict ;
		FreqNode* head ;
		int Capacity ;
};

