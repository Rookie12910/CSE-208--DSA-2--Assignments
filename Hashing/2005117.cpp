#include <bits/stdc++.h>
using namespace std;

bool is_prime(int n)
{
    if (n <= 1)
        return false;
    if (n == 2|| n==3)
        return true;

    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

int nearest_prime(int old_n)
{
    if (old_n <= 1)
        return 2;

    if (is_prime(old_n))
        return old_n;

    int new_n = old_n + 1;

    while (true)
    {
        if (is_prime(new_n))
            return new_n;
        new_n++;
    }

    return old_n;
}

bool Comparator(string& s_1, const string& s_2)
{
    return s_1 == s_2;
}


string word_generator()
{
    string word;
    int length = rand()%6 + 5;
    for(int i=0; i<length; i++)
    {

        int ascii = rand()%26 + 97;
        word+=static_cast<char>(ascii);

    }
    return word;
}

int hash_1(string s,int table_size) //polynomial rolling hash function
{
    int h = 0;
    for (char c : s)
    {
        h  = c + 37*h ;
    }

    h =  h % table_size;
    if(h<0) h+=table_size;
    return h;
}


int a = rand();
int b = rand();
int c = rand();

int hash_2(string s,int table_size) //universal hashing function
{
    int h = 0;
    for (char c : s)
    {
        h += (h * a + b * c) % table_size;
    }

    h =  h % table_size;
    return h;
}




int aux_hash(string s,int table_size) //checksum hashing function
{
    int h = 0;
    for(char c : s)
    {
        h += static_cast<unsigned int>(c);
    }
    h = h % table_size;
    return h;
}




/// HASHTABLE

class HashTable
{
public:
    vector<vector<pair<string,int>>> hash_table;
    int  table_size;
    int col_count;
    int probe_count;
    int crm; //collision resolution method. 1.Separate chaining, 2.Double hashing, 3.Custom probing
    int c1 = 20;
    int c2 = 17;

    HashTable(int n,int crm)
    {
        int N = nearest_prime(n);
        table_size = N;
        hash_table.resize(N);
        col_count = 0;
        probe_count = 0;
        this->crm = crm;
        if(crm==2 || crm==3)
        {
            hash_table[0].resize(N,make_pair("-1",-1));
        }
    }
    ///INSERT FUNCTION
    void Insert(string key,int val, int option)
    {
        if(crm==1)
        {
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            hash_table[h].push_back(make_pair(key,val));
            if(hash_table[h].size()>1) col_count = 1;
            else col_count = 0;
        }


        else if (crm==2)
        {
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i =0;
            while(true)
            {
                h = (h+(i*aux))%table_size;
                if(hash_table[0][h].first=="-1" || hash_table[0][h].first=="-2")
                {
                    hash_table[0][h] = make_pair(key,val);
                    col_count = i;
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    col_count = i;
                    break;
                }
            }
        }
        else
        {
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i =0;
            while(true)
            {
                h = (h+(i*c1*aux)+(i*i*c2))%table_size;
                if(hash_table[0][h].first=="-1" || hash_table[0][h].first=="-2")
                {
                    hash_table[0][h] = make_pair(key,val);
                    col_count = i;
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    col_count = i;
                    break;
                }
            }
        }

    }

    ///FIND FUNCTION
    bool Find(string key,int val, int option)
    {
        if(crm==1)
        {
            bool flag = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);

            for(int i =0; i<hash_table[h].size(); i++)
            {
                if(val==hash_table[h][i].second)
                {
                    flag = true;
                    probe_count = i+1;
                    break;
                }
            }
            return flag;
        }

        else if(crm==2)
        {
            bool flag = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i = 0;
            while(true)
            {
                h = (h+(i*aux))%table_size;
                if(hash_table[0][h].first==key && hash_table[0][h].second==val)
                {
                    flag = true;
                    probe_count = i+1;
                    break;
                }
                else if(hash_table[0][h].first=="-1")
                {
                    probe_count = i+1;
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    probe_count = i+1;
                    break;
                }
            }
            return flag;
        }
        else
        {
            bool flag = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i = 0;
            while(true)
            {
                h = (h+(i*c1*aux)+(i*i*c2))%table_size;
                if(hash_table[0][h].first==key && hash_table[0][h].second==val)
                {
                    flag = true;
                    probe_count = i+1;
                    break;
                }
                else if(hash_table[0][h].first=="-1")
                {
                    probe_count = i+1;
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    probe_count = i+1;
                    break;
                }
            }
            return flag;
        }

    }

 ///DELETE FUNCTION
    bool Delete(string key,int val, int option)
    {
        if(crm==1)
        {
            bool flag = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            if(hash_table[h].size()==0) return flag;
            int index;
            for(int i =0; i<hash_table[h].size(); i++)
            {
                if(val==hash_table[h][i].second)
                {
                    index = i;
                    flag = true;
                    break;
                }
            }
            for(int i = index; i<hash_table[h].size()-1; i++)
            {
                hash_table[h][i].first = hash_table[h][i+1].first;
                hash_table[h][i].second = hash_table[h][i+1].second;
            }
            hash_table[h].pop_back();
            return flag;
        }
        else if(crm==2)
        {
            bool flag  = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i = 0;
            while(true)
            {
                h = (h+(i*aux))%table_size;
                if(hash_table[0][h].first==key && hash_table[0][h].second==val)
                {
                    hash_table[0][h] = make_pair("-2",-1);
                    flag = true;
                    break;
                }
                else if(hash_table[0][h].first=="-1")
                {
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    break;
                }
            }
            return flag;
        }
        else
        {
            bool flag = false;
            int h;
            if(option==1) h = hash_1(key,table_size);
            else if(option==2)  h = hash_2(key,table_size);
            int aux = aux_hash(key,table_size);
            int i = 0;
            while(true)
            {
                h = (h+(i*c1*aux)+(i*i*c2))%table_size;
                if(hash_table[0][h].first==key && hash_table[0][h].second==val)
                {
                    hash_table[0][h] = make_pair("-2",-1);
                    flag = true;
                    break;
                }
                else if(hash_table[0][h].first=="-1")
                {
                    break;
                }
                else i++;
                if(i>=table_size)
                {
                    break;
                }
            }
            return flag;
        }

    }



};



///Main FUNCTION
int main()
{
    srand(1);
    vector<string> words;
    vector<pair<string,int>> data;
    for(int i =0; i<10000; i++)
    {
        string key = word_generator();
        words.push_back(key);
    }
    sort(words.begin(), words.end());
    auto last = unique(words.begin(), words.end(), Comparator);
    words.erase(last, words.end());
    for(int i = 1; i<=words.size(); i++)
    {
        string key = words[i];
        data.push_back(make_pair(key,i));
    }


    cout<<"Hash Table \t"<<"Collision Resolution method\t\t\t"<<"Hash1\t\t\t\t"<<"\tHash2"<<endl;
    cout<<"Size\t\t\t\t\t\t # of Collision\t  Average Probes\t"<<"# of Collision\t  Average probes"<<endl;
    cout<<endl<<endl;

    vector<int> ts = {5000,10000,20000};
    for(int i =0; i<ts.size(); i++)
    {
        int N,Insertion_num;
        N = ts[i];
        Insertion_num = (N <=data.size()) ? N:data.size();


        ///SEPARATE CHAINING
        HashTable SC_1(N,1);
        int c1 = 0;
        int p1 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            SC_1.Insert(data[i].first,data[i].second,1);
            c1+=SC_1.col_count;
        }

        for(int i = 0; i<1000; i++)
        {
            int index = rand()% Insertion_num;
            SC_1.Find(data[index].first,data[index].second,1);
            p1+=SC_1.probe_count;
        }

        HashTable SC_2(N,1);
        int c2 = 0;
        int p2 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            SC_2.Insert(data[i].first,data[i].second,2);
            c2+= SC_2.col_count;
        }

        for(int i = 0; i<1000; i++)
        {

            int index = rand()% Insertion_num;
            SC_2.Find(data[index].first,data[index].second,2);
            p2+=SC_2.probe_count;
        }


        ///DOUBLE HASHING
        HashTable DH_1(N,2);
        int c3 = 0;
        int p3 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            DH_1.Insert(data[i].first,data[i].second,1);
            c3+=DH_1.col_count;
        }
        for(int i = 0; i<1000; i++)
        {

            int index = rand()% Insertion_num;
            DH_1.Find(data[index].first,data[index].second,1);
            p3+=DH_1.probe_count;
        }

        HashTable DH_2(N,2);
        int c4 = 0;
        int p4 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            DH_2.Insert(data[i].first,data[i].second,2);
            c4+= DH_2.col_count;
        }

        for(int i = 0; i<1000; i++)
        {
            int index = rand()% Insertion_num;
            DH_2.Find(data[index].first,data[index].second,2);
            p4+=DH_2.probe_count;
        }



        ///CUSTOM PROBING
        HashTable CP_1(N,3);
        int c5 = 0;
        int p5 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            CP_1.Insert(data[i].first,data[i].second,1);
            c5+=CP_1.col_count;
        }

        for(int i = 0; i<1000; i++)
        {
            int index = rand()% Insertion_num;
            CP_1.Find(data[index].first,data[index].second,1);
            p5+=CP_1.probe_count;
        }


        HashTable CP_2(N,3);
        int c6 = 0;
        int p6 = 0;
        for(int i = 0; i<Insertion_num; i++)
        {
            CP_2.Insert(data[i].first,data[i].second,2);
            c6+= CP_2.col_count;
        }
        for(int i = 0; i<1000; i++)
        {
            int index = rand()% Insertion_num;
            CP_2.Find(data[index].first,data[index].second,2);
            p6+=CP_2.probe_count;
        }


        cout<<"\t\tChaining Method\t\t\t\t"<<c1<<"\t\t"<<p1/1000.0<<"\t\t\t"<<c2<<"\t\t"<<p2/1000.0<<endl;
        cout<<N<<"\t\tDouble hashing\t\t\t\t"<<c3<<"\t\t"<<p3/1000.0<<"\t\t\t"<<c4<<"\t\t"<<p4/1000.0<<endl;
        cout<<"\t\tCustom Probing\t\t\t\t"<<c5<<"\t\t"<<p5/1000.0<<"\t\t\t"<<c6<<"\t\t"<<p6/1000.0<<endl;
        cout<<endl;
    }
    return 0;
}
