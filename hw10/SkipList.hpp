#include <cstdlib>
//Kirill Makarov
//=============================================================================
//== NodeSkipList =============================================================
//=============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value,Key,numLevels>::clear(void)
{
	for (int i = 0; i < numLevels; ++i)
	{
		m_nextjump[i] = 0;
	}
	m_levelHighest = -1;
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
	clear();
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(Key key)
{
	clear();

	m_key = key;
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(Key key, Value value)
{
	clear();

	m_key = key;
	m_value = value;
}
//=============================================================================
//== End of: NodeSkipList =====================================================
//=============================================================================

//=============================================================================
//== SkipList =================================================================
//=============================================================================
template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
	m_probability = probability;
	for (int i = 0; i < numLevels; ++i)
	{
		// Lets use m_pPreHead as a final sentinel element
		m_pPreHead->m_nextjump[i] = m_pPreHead;
	}
	m_pPreHead->m_levelHighest = numLevels-1;
}

// Put your code here

template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels> :: insert(Value value, Key key){

	//Node* temp = OrderedList::pPrehead;
	//for (int i =  m_levelHighest; i > 0; i--)
	//{
	//	temp = findLastLessThan(key);//после этого вставляем
	//}


}


template <class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels> * SkipList<Value,Key,numLevels>::findLastLessThan(Key key) const
{
	//	//this
	//	//Node* temp  = orderedList::m_pPreHead;
	//	//for (int i = m_levelHighest; i > 0; i--){
	//	//	//на каждом уровне
	//	//	while (run->m_next != m_pPreHead
	//	//		&& run->m_next->m_key < key)
	//	//	{
	//	//		run = run->m_next;
	//	//	}
	//	//}
	return nullptr;
}

//
//Node * findFirst(Key key){
//
//}
//
//void remove(Node * nodeBefore) {
//
//}

//=============================================================================
//== End of: SkipList =========================================================
//=============================================================================
