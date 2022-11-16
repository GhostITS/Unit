#include <iostream>
#include <vector>

using namespace std;

#define FMT_FORWARD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

class ClassStorage
{
    union StorageItem
    {
        int IntItem;
        double DoubleItem;
        long long LongLongItem;
        char CharItem;
        bool BoolItem;
        void* VoidPtrItem;
    };

    enum StorageItemType
    {
        IntItem,
        DoubleItem,
        LongLongItem,
        CharItem,
        BoolItem,
        VoidPtrItem,
        Other,
    };

    StorageItem Item;
    StorageItemType ItemType;
    
public:
    template<typename T>
    ClassStorage(const T other)
    {
        ItemType = Other;
    }

    ClassStorage(int a)
    {
        Item.IntItem = a;
        ItemType = IntItem;
    }
    ClassStorage(double c)
    {
        Item.DoubleItem = c;
        ItemType = DoubleItem;
    }
    ClassStorage(long long c)
    {
        Item.LongLongItem = c;
        ItemType = LongLongItem;
    }

    template<const char*>
    ClassStorage(const char* s)
    {
        Item.CharItem = *s;
        ItemType = CharItem;
    }
    ClassStorage(bool b)
    {
        Item.BoolItem = b;
        ItemType = BoolItem;
    }
    ClassStorage(void* p)
    {
        Item.VoidPtrItem = p;
        ItemType = VoidPtrItem;
    }

    string ToStr()
    {
        switch (ItemType)
        {
        case IntItem:
        {
            return to_string(Item.IntItem);
        }
        case DoubleItem:
        {
            return to_string(Item.DoubleItem);

        }
        case LongLongItem:
        {
            return to_string(Item.LongLongItem);
        }
        case CharItem:
        {
            return to_string(Item.CharItem);
        }
        case BoolItem:
        {
            return to_string(Item.BoolItem);
        }
        case VoidPtrItem:
        {
            return to_string((long long)Item.VoidPtrItem);
        }
        
        default:
            break;
        }

        return "user_defined_type";
    }
};

template <typename... Args>
void fmtPrint(string fmt, Args&&... args)
{
    std::string& dst_str = fmt;

    string s;
    vector<ClassStorage> v = {FMT_FORWARD(args)...};
    int i = 0;
    std::string::size_type pos = 0;
    std::string substr = "{}";
    while((pos = dst_str.find(substr)) != std::string::npos && i < v.size())   //替换所有指定子串
    {
        dst_str.replace(pos, substr.length(), v[i].ToStr());
        i++;
    }

    cout<< dst_str<<endl;
}

struct UserDefineType
{
    int a;
};


int main()
{
    int a=1;
    double c=2;
    void*p =&a;
    void*p2 =&c;

    cout<<typeid(a).name()<<" "<<typeid(c).name()
    <<" "<<typeid(p).name()
    <<" "<<typeid(p2).name()
    <<endl;
    UserDefineType userData;
    fmtPrint("int{} float{}", 1, 1.1);
    fmtPrint("int {} str{}", 1, "1.1");
    fmtPrint("int {} userData{}", 1, userData);
    return 0;
}