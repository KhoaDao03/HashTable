template<typename K, typename V>
HashTable<K,V>::HashTable(size_t size){
  if (prime_below(size) <= 0){
    theLists.resize(default_capacity);
  } else {
    theLists.resize(prime_below(size));
  }
  currentSize =  0;
  
}


template<typename K, typename V>
HashTable<K,V>::~HashTable(){
  makeEmpty();
}


/////////////////////////////
template<typename K, typename V>
bool HashTable<K,V>::contains(const K& k)const{
  auto & whichList = theLists[myhash(k)];
  for (const auto & i : whichList){
    if (i.first == k){
      return true;
    }
  }
  return false;

  //return find(whichList.begin() ,whichList.end() , k) != whichList.end();
}

/////////////////////////////
template<typename K, typename V>
bool HashTable<K,V>::match(const std::pair<K,V> &kv) const{
  auto & whichList = theLists[myhash(kv.first)];
  for (const auto & i : whichList){
    if (i.first == kv.first && i.second == kv.second){
      return true;
    }
  }
  return false;
}

/////////////////////////////
template<typename K, typename V>
bool HashTable<K,V>::insert(const std::pair<K,V> & kv){

  bool available = false;


  auto & whichList = theLists[myhash(kv.first)];
  for (const auto & i : whichList){
    if (i.first == kv.first){
      available = true;
    }
  }
  if(available){
    return false;
  }

  whichList.push_back(kv);

  if(++currentSize > theLists.size())
    rehash();
  return true;  
}

/////////////////////////////
template<typename K, typename V>
bool HashTable<K,V>::insert(std::pair<K,V> && kv){
  auto & whichList = theLists[myhash(kv.first)];
  bool available = false;
  
  for (const auto & i : whichList){
    if (i.first == kv.first){
      available = true;
    }
  }
  if(available){
    return false;
  }
  whichList.push_back(std::move(kv));

  if(++currentSize> theLists.size())
    rehash();
  return true;  
}


/////////////////////////////
template<typename K, typename V>
bool HashTable<K,V>::remove(const K &k){
  auto & whichList = theLists[myhash(k)];
  
  auto itr = whichList.begin();
  for (; itr != whichList.end(); itr++){
    if (itr->first == k){
      break;
    }
  }

  if(itr == whichList.end())
    return false;

  whichList.erase(itr);
  --currentSize;
  return true;
  

}

///////////////////
template<typename K, typename V>
void HashTable<K,V>::clear(){
  makeEmpty();
}

///////////////////
template<typename K, typename V>
bool HashTable<K,V>::load(const char *filename){
  ifstream in(filename); 
  if (!in) { 
    return false; 
  } 
  pair<K, V> item; 
  while (in >> item.first >> item.second) { 
    insert(item); 
  } 
  
  in.close(); 
  return true;
}

///////////////////
template<typename K, typename V>
void HashTable<K,V>::dump() const{
  cout << endl;
  for (size_t i = 0; i < theLists.size(); ++i) {
      cout << "v[" << i << "]: ";
      int z = 0;
      for (const auto &kv : theLists[i]) {
        if(z > 0){
          cout << ":";
        }
        cout << "" << kv.first << " " << kv.second;
        z++;
      }
      cout << endl;
    
  }
}

template<typename K, typename V>
size_t HashTable<K,V>::size() const{
  return currentSize;
}

///////////////////
template<typename K, typename V>
bool HashTable<K,V>::write_to_file(const char *filename) const{
  ofstream out(filename);
  if (!out) {
      return false;
  }

  for (const auto &thisList : theLists) {
    for (const auto &kv : thisList) {
        out << kv.first << " " << kv.second << endl;
    }
  }

  out.close();
  return true;
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long> & vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}


template <typename K, typename V>
void HashTable<K,V>::makeEmpty(){
  // for(int i = 0; i < theLists.size();i++){
  //   theLists[i].clear();
  // }
  // theLists.clear();

  for (auto & thisList : theLists){
    thisList.clear();
  }
  currentSize = 0;
  //theLists.clear();
}


template<typename K, typename V>
size_t HashTable<K,V>::myhash(const K & k) const{
  static std::hash<K> hf;
  return hf(k)% theLists.size();
}

///////////////////
template <typename K, typename V>
void HashTable<K, V>::rehash() {
  vector<list<pair<K, V>>> oldLists = theLists;
  for (auto &thisList : theLists) {
    thisList.clear();
  }
  theLists.resize(prime_below(2 * theLists.size()));
  

  currentSize = 0;
  for (const auto &thisList : oldLists) {
    for (const auto &kv : thisList) {
        insert(std::move(kv));
    }
  }
}