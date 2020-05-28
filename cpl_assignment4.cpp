#include<iostream>
#include<string> // for string class 
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */
#define MAXSIZE 10007

using namespace std;

typedef string datatype;

typedef struct hashnode_tag{
    datatype data;
    struct hashnode_tag*Next;
}HashNode;

class HashTable{
    public:

    HashNode**HashTableData;
    int size;
    int firstIndex;
    string firstString;
    //constructor
    HashTable()
    {
        HashTableData=new HashNode*[MAXSIZE];
        for(int i=0;i<MAXSIZE;++i)
        {
            HashTableData[i]=NULL;
        }
        size=0;
        firstIndex=MAXSIZE;
    }

    //Assignment operaTOR for class
    HashTable& operator=(const HashTable &H)
    {
        size=H.size;
        firstIndex=H.firstIndex;
        HashNode**newHashTableData=new HashNode*[MAXSIZE];
        for(int i=0;i<MAXSIZE;++i)
        {
            HashNode*start;
            newHashTableData[i]=NULL;
            
            if(H.HashTableData[i]!=NULL)
            {
                
                HashNode*ptr;
                ptr=H.HashTableData[i];
                HashNode*nptr=MakeNode(ptr->data);
                newHashTableData[i]=nptr;
                start=newHashTableData[i];
                ptr=ptr->Next;
                while(ptr!=NULL)
                {
                    HashNode*nptr=MakeNode(ptr->data);
                    start->Next=nptr;
                    start=start->Next;
                }
            }
        }
        HashTableData=newHashTableData;
        firstString=H.firstString;
        return*this;
    }

    // destructor 
    ~HashTable() 
    { 
        delete[] HashTableData; 
    } 

    // hash function definition 
    unsigned hashfunc(datatype s)
    {
        std::string::iterator it;
        /*unsigned h = FIRSTH;
        for (it=s.begin(); it!=s.end(); it++) 
        {
                h = (h * A) ^ ((int)*it * B);
                
        }
        return h%MAXSIZE; // or return h % C;*/
        long long int hash = 7;
        for (it=s.begin(); it!=s.end(); it++)
        {
            hash = hash*31 + (*it);
        }
        return hash%MAXSIZE;
    }

    HashNode* MakeNode(datatype data)
    {
        HashNode*newNode=new HashNode;
        newNode->data=data;
        newNode->Next=NULL;
        return newNode;
    }

    bool hash_add(datatype s)
    {
        int key=hashfunc(s);
        //cout<<"data="<<s<<" key="<<key<<endl;
        int flag=0;
        bool retval=true;
        HashNode*index=HashTableData[key];
        HashNode*ptr=index;
        if(index!=NULL)
        {
            
            while(ptr->Next!=NULL && flag==0)
            {
                if(ptr->data.compare(s)==0)             //if the element is already there then dont add it
                {
                    retval=false;
                    flag=1;
                }
                else{
                    ptr=ptr->Next;
                }
            }
            if(flag==0)
            {
                HashNode*newNode=MakeNode(s);
                ptr->Next=newNode;
                retval=true;
                size++;
                if(key<firstIndex)
                {
                    firstIndex=key;
                    firstString=s;
                }
            }
        }
        else{
            HashNode*newNode=MakeNode(s);
                HashTableData[key]=newNode;
                retval=true;
                size++;
                if(key<firstIndex)
                {
                    firstIndex=key;
                    firstString=s;
                }
        }
        return retval;
    } 

    bool hash_remove(datatype s) 
    {
        int key=hashfunc(s);
        int flag=0;
        int flag1=0;
        bool retval=false;
        HashNode*index=HashTableData[key];
        HashNode*ptr=index;
        HashNode*prev=index;
        if(index!=NULL)
        {
            
            while((ptr->Next!=NULL || ptr==index) && flag==0 )
            {
                
                if(ptr->data.compare(s)==0)
                {
                    retval=true;
                    size--;
                    if(prev!=ptr)
                    {
                        prev->Next=ptr->Next;
                    }
                    delete ptr;

                    if(firstIndex==key)                             //If the deleted string is at the first index then find then next string by looping through elements
                    {
                        for(int i=key+1;i<MAXSIZE && flag1==0;++i)
                        {
                            if(HashTableData[i]!=NULL)
                            {
                                firstIndex=i;
                                firstString=HashTableData[i]->data;
                                flag1=1;
                            }
                        }
                    }
                    flag=1;
                }
                else{
                    prev=ptr;
                    ptr=ptr->Next;
                }
            }
        }
        return retval;
    } 
    
    bool hash_search(datatype s)
    {
        int key=hashfunc(s);
        int flag=0;
        bool retval=false;
        HashNode*index=HashTableData[key];
        HashNode*ptr=index;
        if(index!=NULL)
        {
            
            while(ptr->Next!=NULL && flag==0)
            {
                if(ptr->data.compare(s)==0)
                {
                    retval=true;
                    flag=1;
                }
                else{
                    ptr=ptr->Next;
                }
            }
        }
        return retval;
    }

    bool hash_isEmpty()
    {
        bool retval=false;
        if(size==0){
            retval=true;
        }
        return retval;
    }

    int hash_cardinality()
    {
        return size;
    }

    HashNode* hash_enumerate()
    {
        
        HashNode*retval;
        if(size==0)
        {
            cout<<"Empty"<<endl;
            retval= NULL;
        }
        else{
            int key=firstIndex;
            retval=MakeNode(firstString);
            HashNode*rptr,*tptr=retval;
            for(int i=key+1;i<MAXSIZE;++i)
            {
                
                HashNode*ptr=HashTableData[i];
                while(ptr!=NULL)
                {
                    rptr=MakeNode(ptr->data);
                    tptr->Next=rptr;
                    tptr=tptr->Next;
                    ptr=ptr->Next;
                }
            }
        }
        return retval;
    }

    //Need to calculate n in function calling from set
    void hash_build(string*ArrStrings,int n)
    {
        for(int i=0;i<n;++i)
        {
            hash_add(ArrStrings[i]);
        }
    }
};

class Set{
    public:
    HashTable hash;
    int sizeSet;
    
    //constructor
    Set()
    {
        sizeSet=0;
    }

    //copy constructor
    Set(const Set &S)
    {
        hash=S.hash;
        sizeSet=S.sizeSet;
    }

    bool is_element_of(datatype s)
    {
        bool retval=false;
        if(hash.hash_search(s))
        {
            retval=true;
        }
        return retval;
    }

    bool is_Empty()
    {
        bool retval=false;
        if(hash.hash_isEmpty()){retval=true;}
        return retval;
    }

    int size()
    {
        int ans=hash.hash_cardinality();
        sizeSet=ans;
        return ans;
    }
    HashNode*enumerate()
    {
        return hash.hash_enumerate();
    }
    void build(string strArr[])
    {
        int i=0;
        while(!strArr[i].empty())
        {
            i++;
        }
        hash.hash_build(strArr,i);
    }
    void create()
    {
        Set();
    }
    bool add(datatype s)
    {
        bool retval=false;
        if(hash.hash_add(s)){retval=true;}
        return retval;
    }
    bool remove(datatype s)
    {
        bool retval=false;
        if(hash.hash_remove(s)){retval=true;}
        return retval;
    }
    void printSet()
    {
        int n=size();
        HashNode*list_ele=enumerate();
        HashNode*ptr=list_ele;
        while(ptr!=NULL)
        {
            cout<<ptr->data<<" ";
            ptr=ptr->Next;
        }
        cout<<endl;
    }

};

Set Union(Set U,Set V) 
{
    HashNode*ptr;
    Set Res;
    ptr=U.enumerate();
    while(ptr!=NULL)
    {
        Res.add(ptr->data);
        ptr=ptr->Next;
    }
    ptr=V.enumerate();
    while(ptr!=NULL)
    {
        Res.add(ptr->data);
        ptr=ptr->Next;
    }
    return Res;
}

Set Intersection(Set U,Set V)
{
    Set*ptr2;
    HashNode*ptr1;
    Set Res;
    if(U.sizeSet<V.sizeSet)
    {
        ptr1=U.enumerate();
        ptr2=&V;
    }
    else{
        ptr1=V.enumerate();
        ptr2=&U;
    }
    HashNode*ptr=ptr1;
    while(ptr!=NULL)
    {
        if(ptr2->is_element_of(ptr->data))
        {
            Res.add(ptr->data);
        }
        ptr=ptr->Next;
    }    
    return Res;

}

Set Difference(Set U,Set V)        //returns U-V
{
    HashNode*ptr1;
    ptr1=U.enumerate();
    Set Res;
    while(ptr1!=NULL)
    {
        if(!(V.is_element_of(ptr1->data)))
        {
            Res.add(ptr1->data);
        }
        ptr1=ptr1->Next;
    }
    return Res;
}

bool Subset(Set U,Set V)                //returns true if U is a subset of V
{
    HashNode*ptr=U.enumerate();
    bool retval=true;
    while(ptr!=NULL)
    {
        if(!(V.is_element_of(ptr->data)))
        {
            retval=false;
        }
        ptr=ptr->Next;
    }
    return retval;
}

int main()
{
    
    string str="GEEks";
    Set S,T;
    //Set T;
    S.add(str);
    S.add("Suruchi");
    T.add("Suruchi");
    T.add("Shrey");
    S.printSet();
    T.printSet();
    Set *uresptr;
    Set ires,diffres;
    //uresptr=&ures;
    Set ures;
    ures=Union(S,T);
    ures.printSet();
    ires=Intersection(S,T);
    ires.printSet();
    diffres=Difference(S,T);
    diffres.printSet();
    if(Subset(S,T)){cout<<"True!!"<<endl;}
    else{
        cout<<"false!!"<<endl;
    }
    return 0;
}