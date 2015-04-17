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
	TypeNode *tempNode = m_pPreHead;
	/*
		Так как по заданию numLevels -- это количество разреженных уровне,
		то на уровне m_nextjump[0] -- могут отказаться не все узлы из первоначального списка
		и существует еще более низкий уровень, где обращение идет по m_next
		Начнем искать элемент с самого верхнего уровня, а затем еще и проверим на уровне, где есть все элементы

		m_pPreHead := Sentinel element - placed before first and after last elements
		Значит, если мы дошли до этого элемента, то мы уже вышли за пределы списка на этом уровне.
	*/
	for (int i = m_pPreHead -> m_levelHighest; i >= 0; i--){
			/*
				на каждом уровне проверяем выход за границы и также, чтобы следующий элемент
				был строго меньше, чем key
			*/
			while (tempNode -> m_nextjump[i] != m_pPreHead
				&& tempNode -> m_nextjump[i] -> m_key < key)
			{
				//переходим к следующему узлу на этом уровне
				tempNode = tempNode -> m_nextjump[i];
			}
			//tempNode это последний элемент с m_key < key на текущем уровне
	}

	//доделываем поиск на заданном (не разреженный) связном списке.
	while (tempNode -> m_next != m_pPreHead && tempNode ->m_next -> m_key < key)
	{
		tempNode = tempNode -> m_next;
	}

	return tempNode;

}



template <class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels> * SkipList<Value,Key,numLevels>::findFirst(Key key) const{
	return nullptr;
}


template <class Value, class Key, int numLevels>
void SkipList<Value,Key,numLevels>:: remove(NodeSkipList<Value,Key, numLevels> * node) {

}

//=============================================================================
//== End of: SkipList =========================================================
//=============================================================================
