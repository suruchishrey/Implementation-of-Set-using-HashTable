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
        HashNode*ptr,*temp;
        for(int i=0;i<MAXSIZE;++i)
        {
            ptr=HashTableData[i];
            while(ptr!=NULL)
            {
                temp=ptr;
                ptr=ptr->Next;
                delete temp;
            }
        }
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
        int key=hashfunc(s);                    //calculate the key from hash function 
        //cout<<"data="<<s<<" key="<<key<<endl;
        int flag=0;
        bool retval=true;
        HashNode*index=HashTableData[key];
        HashNode*ptr=index;
        HashNode*prev=ptr;
        if(index!=NULL)                         //see if there is a collision 
        {
            
            while(ptr!=NULL && flag==0)
            {
                if(ptr->data.compare(s)==0)             //if the element is already there then dont add it
                {
                    retval=false;
                    flag=1;
                }
                else{
                    prev=ptr;
                    ptr=ptr->Next;                      //else go to the last node in that list
                }
            }
            if(flag==0)                                 //if there's a collision and the element doesnot exist already 
            {
                HashNode*newNode=MakeNode(s);           //then make a new node for that data 
                prev->Next=newNode;                     //insert the new node at the last of the list
                retval=true;
                size++;
                if(key<firstIndex)                      //update the firstIndex and firstString (for traversing hashtable)
                {
                    firstIndex=key;
                    firstString=s;
                }
            }
        }
        else{                                           //if there is no collision make a new node for given data
            HashNode*newNode=MakeNode(s);
                HashTableData[key]=newNode;             //Insert the node at that index
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
        int key=hashfunc(s);                        //calculate the key(location)
        int flag=0;
        int flag1=0;
        bool retval=false;
        HashNode*index=HashTableData[key];
        HashNode*ptr=index;
        HashNode*prev=index;
        if(index!=NULL)                             //if that element is there in the hash table
        {
            
            while((ptr!=NULL) && flag==0 )
            {
                
                if(ptr->data.compare(s)==0)
                {
                    retval=true;
                    size--;
                    if(prev!=ptr)
                    {
                        prev->Next=ptr->Next;
                    }
                    else{
                        HashTableData[key]=ptr->Next;
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
            
            while(ptr!=NULL && flag==0)
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

    bool is_element_of(datatype s)          //checks if s is element of the set or not
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
    Set U,V;
    Set Result;
    cout<<"This program will show operation on two sets(of Strings)"<<endl;
    cout<<"Enter set 1 elements:"<<endl;
    string str;
    int enter=1,choice;
    while(enter==1)
    {
        cin>>str;
        U.add(str);
        cout<<"\nWant to enter more?(0 for NO and 1 for YES)"<<endl;
        cin>>enter;
    }
    cout<<"Enter set 2 elements:"<<endl;
    enter =1;
    while(enter==1)
    {
        cin>>str;
        V.add(str);
        cout<<"\nWant to enter more?(0 for NO and 1 for YES)"<<endl;
        cin>>enter;
    }
    cout<<"The entered elements are:"<<endl;
    cout<<"Set 1:"<<endl;
    U.printSet();
    cout<<"Set 2:"<<endl;
    V.printSet();
    do{
        cout<<"Operations:"<<endl;
        cout<<"1)Union\n2)Intersection\n3)Difference\n4)Subset\n5)Remove an element\n6)Show the set Elements\nEnter which operation would you like to perform?"<<endl;
    
        cin>>enter;
        switch (enter)
        {
        case 1:
            Result=Union(U,V);
            cout<<"\nUnion:"<<endl;
            Result.printSet();
            break;
        case 2:
            Result=Intersection(U,V);
            cout<<"\nIntersection:"<<endl;
            Result.printSet();
            break;
        case 3:
            Result=Difference(U,V);
            cout<<"\nDifference:"<<endl;
            Result.printSet();
            break;
        case 4:
            bool result;
            result=Subset(U,V);
            if(result)cout<<"\nU is a subset of V"<<endl;
            else cout<<"U is not a subset of V."<<endl;
            break;
        case 5:
            cout<<endl;
            cout<<"Set 1:"<<endl;
            U.printSet();
            cout<<"Set 2:"<<endl;
            V.printSet();
            cout<<"Enter the element which you want to delete:"<<endl;
            cin>>str;
            cout<<"Enter the set(1 or 2)from which you want to delete the element?"<<endl;
            cin>>choice;
            cout<<"Deleting the element from the set"<<endl;
            if(choice==1)U.remove(str);
            else if(choice==2)V.remove(str);
            cout<<"Set 1 is"<<endl;
            U.printSet();
            cout<<"Set 2 is"<<endl;
            V.printSet();
            break;
        case 6:
            cout<<"Set 1:"<<endl;
            U.printSet();
            cout<<"Set 2:"<<endl;
            V.printSet();
            break;
        default:
        cout<<"Invalid Input!"<<endl;
            break;
        }
        cout<<"\nWant to some more operations?(1 for yes and 0 for no)";
        cin>>enter;
    }while (enter);
    
    return 0;
}
